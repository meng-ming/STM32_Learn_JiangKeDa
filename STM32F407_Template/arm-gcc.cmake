# =======================================================
# arm-gcc.cmake - 工具链定义（锁定 13.2.Rel1，永不翻车）
# =======================================================
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 工具链绝对路径（改成你自己的路径！！！）
set(TOOLCHAIN_DIR "D:/ARM_Development_Toolchains/arm-gnu-toolchain-13.2.Rel1/13.2.Rel1/bin")

get_filename_component(TOOLCHAIN_DIR_ABS "${TOOLCHAIN_DIR}" ABSOLUTE)

# 指定交叉编译器
set(CMAKE_C_COMPILER   "${TOOLCHAIN_DIR_ABS}/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_DIR_ABS}/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_DIR_ABS}/arm-none-eabi-gcc.exe")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_DIR_ABS}/arm-none-eabi-objcopy")
set(CMAKE_SIZE         "${TOOLCHAIN_DIR_ABS}/arm-none-eabi-size")

# 裸机项目必须强制跳过编译器检测（否则会卡死）
set(CMAKE_C_COMPILER_WORKS   TRUE CACHE BOOL "Force C compiler check pass")
set(CMAKE_CXX_COMPILER_WORKS TRUE CACHE BOOL "Force CXX compiler check pass")
set(CMAKE_ASM_COMPILER_WORKS TRUE CACHE BOOL "Force ASM compiler check pass")

# 禁止 CMake 在主机系统里找库和头文件
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY  ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE  ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE  ONLY)

# 裸机项目专用：告诉 CMake 我们是静态库
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)