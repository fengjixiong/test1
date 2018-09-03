add_executable(test_lib2 test_lib2.cpp)

include_directories("../lib2")
link_directories("../lib2" "../lib1")
target_link_libraries(test_lib2 lib1 lib2)