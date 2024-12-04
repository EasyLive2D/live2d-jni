# Use Live2D in Any Programming Language

## 思路

参考 [live2d-py](https://github.com/Arkueid/live2d-py)，所有 live2d 的功能都可以通过两个主要的类来实现：
* `Live2D`：live2d 模块管理，用于初始化
* `Live2D.LAppModel`: 用于加载和操作模型

其他相关工具: 
`Live2D.utils`: 比如口型同步，面捕等

`live2d-py` 的核心模块 `Core`，`Framework`，`glew`，`Main` 和 Python 无关，可以用于封装其他编程语言的 live2d 库。

## 例子

目前给出一个用于 Java 桌面环境的 Live2D 封装库的示例。

* 使用 CMake 构建

* JNI 头文件: 通过 `javac -h` 生成，类文件：[Live2D.java](./com/arkueid/live2d/Live2D.java)

* Output DLL: `live2d.dll`

## 缺点

* 对 Cubism Framework 进行了修改，当 Cubism 官方进行大版本更新的时候，如果想同步更新会比较麻烦
* 整体比较冗余

当然，也可以直接从 Cubism Core 开始封装。