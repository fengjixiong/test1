import sys,os

header_str = '''
#ifndef _%s
#define _%s

#include "my_dll_exports.h"\n
'''
prefix_str = 'DLL_API '
define_str = '#define DLL_EXPORTS 1\n'

def parse_cpp(filename):
	f = open(filename, 'r')
	buffer = f.read()
	f.close()
	f = open(filename +'.ctemp', 'w')
	f.write(define_str)
	f.write(buffer)
	f.close()
	
	name = filename.split('/')[-1].split('.')[0]
	fdir = '/'.join(filename.split('/')[0:-1])
	os.system('mv %s %s.cbak'%(filename, fdir + '/' + name))
	os.system('mv %s %s.cpp'%(filename+'.ctemp', fdir + '/' + name))
	
	pass

def parse_line(line):
	if len(line) == 0:
		line = line
	elif not (line[0].isalpha() or line[0] == '_'):
		line = line
	elif line.startswith('class ') or line.startswith('template ') or line.startswith('struct ') or line.startswith('typedef struct'):
		line = prefix_str + line
	elif line.find('(') != -1 and line.find(')') != -1 and (line.endswith(';') or line.strip().endswith(',')):
		line = prefix_str + line
	return line
	
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
		line = parse_line(line)
		f.write(line + '\n')
	f.write('\n#endif\n')
	f.close()
	
	name = filename.split('/')[-1].split('.')[0]
	fdir = '/'.join(filename.split('/')[0:-1])
	os.system('mv %s %s.hbak'%(filename, fdir + '/' + name))
	os.system('mv %s %s.h'%(filename+'.htemp', fdir + '/' + name))
	
	pass


def main(libdir):
	dirs = os.walk(libdir)
	for [curr_dir,sub_dir,files] in dirs:
		for f in files:
			fullname = curr_dir + '/' + f
			print(fullname)
			if f.endswith('.hbak'):
				parse_h(fullname)
			elif f.endswith('.cbak'):
				parse_cpp(fullname)
	pass

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print('usage: python generate_dll_exports.py bdir')
	else:
		main(sys.argv[1])