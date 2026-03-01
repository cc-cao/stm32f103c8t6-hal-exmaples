# stm32f103c8t6最小系统板 江科大入门教程示例代码 

## CMake构建
通用配置在 `CMakePresets.json`，用户差异（如工具链路径）在 `CMakeUserPresets.json`。  
工具链文件位于 `cmake/toolchains/arm-none-eabi.cmake`，每个示例位于 `examples/*/CMakeLists.txt`。

1. 编辑 `CMakeUserPresets.json`，设置 `ARM_GCC_BIN_DIR` 为 `arm-none-eabi-gcc` 所在目录。
2. 配置并编译:
```sh
cmake --preset debug
cmake --build --preset debug
```
4. 单独编译某个示例:
```sh
cmake --build --preset debug --target can
```
