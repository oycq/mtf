cmake_minimum_required(VERSION 3.14)

# 项目信息
project(mtf)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 mtf 可执行文件的源文件和头文件
file(GLOB_RECURSE MTF_INCLUDE "src/common/include/*.h" "src/mtf/include/*.h")
file(GLOB_RECURSE MTF_SRC "src/mtf/*.c" "src/common/*.c")

# 查找 dirty_lens 可执行文件的源文件和头文件
file(GLOB_RECURSE DIRTY_LENS_INCLUDE "src/common/include/*.h" "src/dirty_lens/include/*.h")
file(GLOB_RECURSE DIRTY_LENS_SRC "src/dirty_lens/*.c" "src/common/*.c")

# 添加 mtf 可执行文件
add_executable(mtf ${MTF_SRC} ${MTF_INCLUDE})

# 添加 dirty_lens 可执行文件
add_executable(dirty_lens ${DIRTY_LENS_SRC} ${DIRTY_LENS_INCLUDE})

# 指定头文件目录
target_include_directories(mtf PRIVATE "src/common/include" "src/mtf/include")
target_include_directories(dirty_lens PRIVATE "src/common/include" "src/dirty_lens/include")

#预处理宏
target_compile_definitions(mtf PRIVATE MSDOS)
target_compile_definitions(dirty_lens PRIVATE MSDOS)

# 设置编译选项
if(MSVC)
    target_compile_options(mtf PRIVATE /W0)
    target_compile_options(dirty_lens PRIVATE /W0)
else()
    target_compile_options(mtf PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(dirty_lens PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
