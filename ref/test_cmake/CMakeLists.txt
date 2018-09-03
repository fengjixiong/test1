cmake_minimum_required (VERSION 2.6)
project(test_cmake)

# 通过在主工程文件CMakeLists.txt中修改ADD_SUBDIRECTORY (lib) 指令来指定一个编译输出位置;

# 指定本工程中静态库libhello.so生成的位置，即 build/lib;

add_subdirectory(lib1)
add_subdirectory(lib2)
add_subdirectory(lib3)
add_subdirectory(test_lib2)
add_subdirectory(test_lib3)