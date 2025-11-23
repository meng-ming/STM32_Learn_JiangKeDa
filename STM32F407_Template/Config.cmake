# =======================================================
# Config.cmake - 项目核心配置（芯片型号、优化等级、链接脚本）
# =======================================================

# 芯片型号（F407ZGT6、F407ZET6、F407VGT6 等都通用）
set(CHIP_MODEL "STM32F407ZGTX")

# 编译优化等级
# -Os  : 体积优化（推荐裸机/量产）
# -O0  : 调试最友好
# -O2/-O3 : 性能最强，但体积会大一点
set(OPTIMIZATION_FLAGS "-Os")

# 链接脚本绝对路径（决定了 Flash、RAM 布局，千万别改错！）
set(LINKER_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/Core/linker/STM32F407ZGTX_FLASH.ld")