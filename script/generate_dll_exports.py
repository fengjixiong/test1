import sys,os

header_str = '''
#ifndef _%s
#define _%s

#include "my_dll_exports.h"\n
'''
prefix_str = 'DLL_API'
define_str = 'DLL_EXPORTS'

def parse_cpp(filename):
	f = open(filename, 'r')
	buffer = f.read()
	f.close()
	f = open(filename +'.ctemp', 'w')
	f.write(define_str)
	f.write(buffer)
	f.close()
	pass

def parse_h(filename):
	'给.h文件添加my_dll_exports.h'
	'同时给每个类声明添加DLL_API'
	fname = filename.split('/')[-1].replace('.','_')
	f = open(filename, 'r')
	buffer = f.read()
	f.close()
	f = open(filename +'.htemp', 'w')
	f.write(header_str%(fname.upper(),fname.upper()))
	for line in buffer.split('\n'):
		if line.starswith('class ') or \
		line.starswith('template '):
			line = prefix_str + line
		f.write(line)
	f.write('\n#endif\n')
	f.close()
	
	pass


def main(libdir):
	dirs = os.walk(libdir)
	for [curr_dir,sub_dir,files] in dirs:
		for f in files:
			fullname = libdir + curr_dir + f
			if f.endswith('.h'):
				parse_h(fullname)
			elif f.endswith('.cpp'):
				parse_cpp(fullname)
	pass

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print('usage: python generate_dll_exports.py bdir')
		return
	main(sys.argv[1])