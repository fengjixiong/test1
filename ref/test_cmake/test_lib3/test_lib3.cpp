// test_lib.cpp : 定义控制台应用程序的入口点。
// 这个程序的意义在于证明，dll项目生成的lib可以不再引用其他依赖项
// 2018.8.28

#include "stdafx.h"
#include "lib3.h"

int main(int argc, char* argv[])
{
	int res = compute3(3,4);
	printf("res=%d\n", res);
	getchar();
	return 0;
}

