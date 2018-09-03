SET (LIB_SRC lib2.cpp)

# SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
include_directories("../lib1")
# three kinds of libraries:
# 1. shared, i.e., dynamic library，动态库文件扩展名常为 "*.so"；
# 2. static, i.e., static library， 静态库文件扩展名常为 "*.a";
# 而且，我们通常希望静态库和动态库文件名保持一致，只是扩展名不同；
# 3. module, this parameter is valid only when dyld is supported;
# otherwise, will be considered as shared

# 添加动态库，关键词为shared，你不需要写全libhello.so，
# 只需要填写hello即可，cmake系统会自动为你生成 libhello.X
#ADD_LIBRARY (hello SHARED ${LIBHELLO_SRC})

# 添加静态库，关键词为static，
# ADD_LIBRARY (hello STATIC ${LIBHELLO_SRC})
# 仍然用hello作为target名时，是不能成功创建所需的静态库的，
# 因为hello作为一个target是不能重名的， 故把上面的hello修改为hello_static
# 同理，你不需要写全libhello_static.a
# 只需要填写hello即可，cmake系统会自动为你生成 libhello_static.X
ADD_LIBRARY (lib2 STATIC ${LIB_SRC})


# 按照一般的习惯，静态库名字跟动态库名字应该是一致的，只是扩展名不同；
# 即：静态库名为 libhello.a； 动态库名为libhello.so ；
# 所以，希望 "hello_static" 在输出时，不是"hello_static"，而是以"hello"的名字显示，故设置如下：
#SET_TARGET_PROPERTIES (hello_static PROPERTIES OUTPUT_NAME "hello")
