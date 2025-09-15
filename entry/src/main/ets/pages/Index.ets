import { SoundTouch, SoundTouchOptions } from '@devzeng/soundtouch';
import { fileIo } from '@kit.CoreFileKit';
import { media } from '@kit.MediaKit';
import { common } from '@kit.AbilityKit';
import { audio } from '@kit.AudioKit';
import { BusinessError } from '@kit.BasicServicesKit';
import { promptAction } from '@kit.ArkUI';

const TAG = 'Index';

@Entry
@Component
struct Index {
  @State input: string = '';
  @State output: string = '';
  @State loading: boolean = false;
  private count: number = 0;
  private isSeek: boolean = true; // 用于区分模式是否支持seek操作
  private avPlayer?: media.AVPlayer;

  build() {
    Column() {
      Button('转换')
        .fontSize(16)
        .fontWeight(FontWeight.Regular)
        .backgroundColor('#344F9C')
        .width('80%')
        .margin({ top: 40 })
        .onClick(() => {
          this.processAudioFile()
        })

      Row() {
        Button('开始播放')
          .fontSize(16)
          .fontWeight(FontWeight.Regular)
          .backgroundColor('#344F9C')
          .width('45%')
          .onClick(() => {
            this.startPlayAudioFile()
          })

        Button("停止播放")
          .fontSize(16)
          .fontWeight(FontWeight.Regular)
          .backgroundColor('#344F9C')
          .width('45%')
          .onClick(() => {
            this.stopPlayAudioFile()
          })
      }
      .margin({ top: 20 })
      .justifyContent(FlexAlign.SpaceBetween)
      .width('80%')
    }
    .justifyContent(FlexAlign.Start)
    .alignItems(HorizontalAlign.Center)
    .expandSafeArea([SafeAreaType.SYSTEM, SafeAreaType.KEYBOARD])
    .height('100%')
    .width('100%')
  }

  private processAudioFile() {
    const context = getContext();
    context.resourceManager.getRawFd('audio.wav', (err, data) => {
      if (err != null) {
        console.error(TAG, `getRawFd error: [${ err.code }]${ err.message }`);
        return;
      }
      // 拷贝audio.wav
      let filePath = context.tempDir + "/audio.wav";
      if (fileIo.accessSync(filePath)) {
        fileIo.unlinkSync(filePath);
      }
      console.log(TAG, `copy audio.wav to ${ filePath }`);
      const dest = fileIo.openSync(filePath, fileIo.OpenMode.CREATE | fileIo.OpenMode.READ_WRITE);
      let bufsize = 4096;
      let buf = new ArrayBuffer(bufsize);
      let off = 0, len = 0, readedLen = 0;
      while (len = fileIo.readSync(data.fd, buf, { offset: data.offset + off, length: bufsize })) {
        readedLen += len;
        fileIo.writeSync(dest.fd, buf, { offset: off, length: len });
        off = off + len;
        if ((data.length - readedLen) < bufsize) {
          bufsize = data.length - readedLen;
        }
      }
      fileIo.close(dest.fd);

      let filePath1 = context.tempDir + "/audio_1.wav";
      if (fileIo.accessSync(filePath1)) {
        fileIo.unlinkSync(filePath1);
      }
      const options: SoundTouchOptions = {
        tempo: 15,
        pitch: 3
      }
      SoundTouch.processSync(filePath, filePath1, options);
      promptAction.showToast({message: '转换完成'});
    })
  }

  private async startPlayAudioFile() {
    try {
      // 创建avPlayer实例对象
      this.avPlayer = await media.createAVPlayer();
      // 创建状态机变化回调函数
      this.setAVPlayerCallback(this.avPlayer!);
      let fdPath = 'fd://';
      // 通过UIAbilityContext获取沙箱地址filesDir，以Stage模型为例
      let context = getContext(this) as common.UIAbilityContext;
      let pathDir = context.tempDir;
      let path = pathDir + "/audio_1.wav";
      // 打开相应的资源文件地址获取fd，并为url赋值触发initialized状态机上报
      let file = await fileIo.open(path);
      fdPath = fdPath + '' + file.fd;
      this.isSeek = true; // 支持seek操作
      this.avPlayer!.url = fdPath;
    } catch (e) {
      console.error(TAG, `playAudioFile error: ${ JSON.stringify(e) }`);
    }
  }

  private async stopPlayAudioFile() {
    this.avPlayer!.stop()
  }

  // 注册avplayer回调函数
  setAVPlayerCallback(avPlayer: media.AVPlayer) {
    // seek操作结果回调函数
    avPlayer.on('seekDone', (seekDoneTime: number) => {
      console.info(`AVPlayer seek succeeded, seek time is ${seekDoneTime}`);
    });
    // error回调监听函数,当avPlayer在操作过程中出现错误时调用 reset接口触发重置流程
    avPlayer.on('error', (err: BusinessError) => {
      console.error(`Invoke avPlayer failed, code is ${err.code}, message is ${err.message}`);
      avPlayer.reset(); // 调用reset重置资源，触发idle状态
    });
    // 状态机变化回调函数
    avPlayer.on('stateChange', async (state: string, reason: media.StateChangeReason) => {
      switch (state) {
        case 'idle': // 成功调用reset接口后触发该状态机上报
          console.info('AVPlayer state idle called.');
          avPlayer.release(); // 调用release接口销毁实例对象
          break;
        case 'initialized': // avplayer 设置播放源后触发该状态上报
          console.info('AVPlayer state initialized called.');
          avPlayer.audioRendererInfo = {
            usage: audio.StreamUsage.STREAM_USAGE_MUSIC,
            rendererFlags: 0
          };
          avPlayer.prepare();
          break;
        case 'prepared': // prepare调用成功后上报该状态机
          console.info('AVPlayer state prepared called.');
          avPlayer.play(); // 调用播放接口开始播放
          break;
        case 'playing': // play成功调用后触发该状态机上报
          console.info('AVPlayer state playing called.');
          if (this.count !== 0) {
            if (this.isSeek) {
              console.info('AVPlayer start to seek.');
              avPlayer.seek(avPlayer.duration); //seek到音频末尾
            } else {
              // 当播放模式不支持seek操作时继续播放到结尾
              console.info('AVPlayer wait to play end.');
            }
          } else {
            setTimeout(() => {
              console.info('AVPlayer is playing wait to pause');
              avPlayer.pause(); // 播放3s后调用暂停接口暂停播放
            }, 3000);
          }
          this.count++;
          break;
        case 'paused': // pause成功调用后触发该状态机上报
          console.info('AVPlayer state paused called.');
          setTimeout(() => {
            console.info('AVPlayer paused wait to play again');
            avPlayer.play(); // 暂停3s后再次调用播放接口开始播放
          }, 3000);
          break;
        case 'completed': // 播放结束后触发该状态机上报
          console.info('AVPlayer state completed called.');
          avPlayer.stop(); //调用播放结束接口
          break;
        case 'stopped': // stop接口成功调用后触发该状态机上报
          console.info('AVPlayer state stopped called.');
          avPlayer.reset(); // 调用reset接口初始化avplayer状态
          break;
        case 'released':
          console.info('AVPlayer state released called.');
          break;
        default:
          console.info('AVPlayer state unknown called.');
          break;
      }
    });
  }
}