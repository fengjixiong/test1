add_executable(test_lib3 test_lib3.cpp)

include_directories("../lib3")
link_directories("../lib3")
target_link_libraries(test_lib3 lib3)