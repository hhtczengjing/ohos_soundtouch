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