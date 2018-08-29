#ifndef _MY_DLL_EXPORTS_H
#define _MY_DLL_EXPORTS_H

// reference: http://geoffair.net/ms/declspec.htm

#ifdef _MSC_VER
	#ifdef MAKE_STATIC
		#define DLL_API
	#elif defined DLL_EXPORT
		#define DLL_API __declspec(dllexport)
	#else
		#define DLL_API __declspec(dllimport)
	#endif
#endif

#endif