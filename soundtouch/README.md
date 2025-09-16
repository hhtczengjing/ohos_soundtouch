# ohos_soundtouch

SoundTouch for HarmonyOS.

## 安装

```shell
ohpm i @devzeng/soundtouch
```

OpenHarmony ohpm 环境配置等更多内容，请参考[如何安装 OpenHarmony ohpm 包](https://ohpm.openharmony.cn/#/cn/help/downloadandinstall)

## 使用

```
// 设置参数
const options: SoundTouchOptions = {
    tempo: 15,
    pitch: 3
};

// 同步方法
SoundTouch.processSync(inputPath, outputPath, options);

// 异步方法
SoundTouch.process(inputPath, outputPath, options).then((res) => {
  promptAction.showToast({message: '转换完成'});
}).catch((error: BusinessError) => {
  promptAction.showToast({message: '转换失败'});
});
```

> 说明：仅支持wav格式文件