# Use Live2D in Any Programming Language

[[中文]](./README.cn.md)

## Details  

Inspiration: Based on `live2d-py`, where all Live2D features are implemented through two primary classes:

* `Live2D`: Manages Live2D modules, used for initialization.
* `Live2D.LAppModel`: Handles loading and operating models.
Other utilities:

`Live2D.utils`: Includes features like lip-syncing and facial capture.  

The core modules of `live2d-py` (`Core`, `Framework`, `glew`, and `Main`) are language-independent and can be used to wrap Live2D functionalities for other programming languages.

## Example

* Build with CMake

* JNI headers: Generated via `javac -h` from [Live2D.java](./com/arkueid/live2d/Live2D.java)

* Output DLL: `live2d.dll`

## Limitations
* Modifications were made to the Cubism Framework, which can make it cumbersome to update when a major version is released by Cubism.
* The implementation is somewhat redundant.

Alternatively, you could start by wrapping directly from the Cubism Core.