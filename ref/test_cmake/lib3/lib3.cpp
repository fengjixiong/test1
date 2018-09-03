
#define DLL_EXPORTS 1
#include "lib3.h"
#include "lib1.h"

int DLL_API compute3(int a, int b)
{
	return add(a, b);
}