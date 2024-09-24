cmake_minimum_required(VERSION 3.14)

# 项目信息
project(mtf)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# 查找所有源文件和头文件
file(GLOB_RECURSE INCLUDE "src/common/include/*.h" "src/dirty_lens/include/*.h")
#file(GLOB_RECURSE SRC "src/mtf/*.c" "src/common/*.c")
file(GLOB_RECURSE SRC "src/dirty_lens/*.c" "src/common/*.c")

# 添加可执行文件
add_executable(mtf ${SRC} ${INCLUDE})

# 设置编译选项
if(MSVC)
    target_compile_options(mtf PRIVATE /W0)
else()
    target_compile_options(mtf PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# 指定头文件目录
target_include_directories(mtf PRIVATE "src/common/include" "src/dirty_lens/include")

#预处理宏
target_compile_definitions(mtf PRIVATE MSDOS)