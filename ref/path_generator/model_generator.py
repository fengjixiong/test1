# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import os, sys
import math
import matplotlib.pyplot as plt
from path_model import PathModel
from tri_helper import TriHelper
from dem_model import DEMModel, ExpandRange

def GetSectionLicheng(lichengFile):
	file = open(lichengFile)
	licheng = []
	for line in file:
		if line == "":
			continue
		line = line.strip('\n')
		licheng.append(list(eval(line)))
	file.close()
	return licheng

def GetSectionType(sectionFile):
	'获取路段类型，1-路基，2-隧道，3-桥梁'
	file = open(sectionFile)
	line = file.readline().strip('\n')
	temp = line.split(' ')
	secType = []
	for i in temp:
		secType.append(int(i))
	return secType

def ModifyLicheng(licheng, secType, outfile):
	new_licheng = []
	f = open(outfile, 'w')
	for i in range(0, len(licheng)):
		licheng_len = licheng[i][1] - licheng[i][0]
		if i > 0:
			licheng[i][0] = licheng[i-1][1]
		if secType[i] != 1 and licheng_len > 1000:
			licheng[i][1] = licheng[i][0] + 1000
		else:
			licheng[i][1] = licheng[i][0] + licheng_len
	
		f.write(str(licheng[i]) + '\n')
	f.close()
	return licheng
	
def LoadModel(modelFile='', interpFile=''):
	'加载路线模型'
	if modelFile == '':
		modelFile = '../pathmodel.xml'
		interpFile = '../interp_points.txt'
	my_path = PathModel()
	my_path.Load(modelFile)
	my_path.LoadInterpPoints(interpFile)
	return my_path
	#PlotModel(my_path)

def IsSimilar(pt1, pt2, threshold=0.1):
	'判断两个点是否相似'
	if len(pt1) != len(pt2):
		return False
	sum = 0
	for i in range(0, len(pt1)):
		sum += math.fabs(pt1[i]-pt2[i])
	if sum < threshold:
		return True
	else:
		return False

def LoadGroundPoints():
	'加载原始的地面数据'
	ground_origin_file = '../ground-origin.hdm'
	ground2D_file = '../ground-origin-2D.txt'
	f_in = open(ground_origin_file,'r')
	f_out = open(ground2D_file, 'w')
	
	y_ref = 9800.0
	z_ref = 1500.0

	lc = ''
	for line in f_in:
		pt = line.split(',')
		if len(pt) < 2:
			continue
		#print(pt)
		if pt[0] == "AK":
			lc = eval(pt[1])
		else:
			x = eval(pt[0])
			z = eval(pt[1].strip('\n'))
			f_out.write('%.3f, %.3f, %.3f\n'%(x, lc-y_ref, z-z_ref))
	f_out.close()
	f_in.close()

def GenerateGroundPoints3D(my_path, ground2D_file='', ground3D_file='', licheng_file='', licheng_new_file=''):
	'生成地面点'
	if ground2D_file == '':
		ground2D_file = '../ground.txt'
		ground3D_file = '../ground3D.txt'
		licheng_file = '../licheng.txt'
		licheng_new_file = '../newlicheng.txt'

	lcArray = GetSectionLicheng(licheng_file)
	lcArrayNew = GetSectionLicheng(licheng_new_file)

	f_in = open(ground2D_file, 'r')
	f_out = open(ground3D_file, 'w')
	sec_index = 0 # 当前所处的路段
	pt_count = 0
	for line in f_in:
		pt_count += 1
		line.strip('\n')
		pt = list(eval('['+line+']'))
		licheng = pt[1]
		if licheng > lcArray[sec_index][1]: #地面点的里程是增长的
			sec_index += 1
		if sec_index == len(lcArrayNew):
			break
		newlicheng_len = lcArrayNew[sec_index][1] - lcArrayNew[sec_index][0]
		licheng_len = licheng - lcArray[sec_index][0]
		if licheng_len > newlicheng_len:
			continue # 如果该地面点里程过大，丢弃
		newlicheng = lcArrayNew[sec_index][0] + licheng_len
		pt2D = my_path.GetLichengPoint(newlicheng, pt[0])
		pt3D = [pt2D[0], pt2D[1], pt[2]]
		# 保存里程数据，便于检索
		f_out.write("%.3f, %.3f, %.3f, %.3f\n"%(newlicheng, pt3D[0], pt3D[1], pt3D[2]))
	print('Finish! %d points in total'%pt_count)
	f_out.close()
	f_in.close()



def GenerateDEMfromTin(ground3D_file):
	'根据分辨率生成DEM'
	# 1.随机生成一个DEM网格，大小包括所有地面点，高度是平均值
	# 2.地面点是随着里程增长的，所以可以考虑每30个点一批，
	#   更新路径附近的DEM。散点当然要精简
	# 3.因为地面点是稀疏的，所以计算量会小很多
	# 4.三角化TIN的时候，利用DEM的点补充一些，不然三角形会奇异
	# 5.显示还是需要TIN的，因为地面要豁口，没有TIN是做不到的
	# 综上，顺序是随机DEM-散点更新DEM-DEM补充散点-散点三角化
	# DEM用来计算插值，TIN用来显示
	
	# 0. 配置参数
	if ground3D_file == '':
		ground3D_file = '../ground3D-clean.txt'
	# 1. 初始化DEM
	dem = DEMModel(res=[10,10])
	# 2. 读取地形点
	print('DEM: Reading ground points...')
	ground_pts = []
	ground_licheng = []
	f_ground = open(ground3D_file)
	last_pt = [0,0]
	for line in f_ground:
		line.strip('\n')
		pt = list(eval('['+line+']'))
		if IsSimilar(pt, last_pt):
			continue
		last_pt = pt
		ground_licheng.append(pt[0])
		ground_pts.append(pt[1:])
		# 更新DEM范围
		dem.Expand(pt[1:3])

	# 3. 分段对地形点进行三角化剖分、生成DEM
	print('DEM: Generating...')
	temp_points = []
	temp_range = [0,0,0,0]
	lc_range = [940, 940]
	lc_max = 100
	th = TriHelper()
	for lc, pt in zip(ground_licheng, ground_pts):
		lc_range[1] = lc
		temp_range = ExpandRange(temp_range, pt[0], pt[1])
		# 根据里程进行分段
		if lc_range[1] - lc_range[0] < lc_max:
			temp_points.append(pt)
			continue
		print('DEM: Generating between ', lc_range, 
			  ' ...totoal %d pts'%len(temp_points))
		# 三角剖分
		th.Delaunay(temp_points)
		print('DEM: Updating points...')
		dem.UpdatePoints(th, temp_range)
		# 重新开始
		temp_points.clear()
		temp_range = [0,0,0,0]
		lc_range[0] = lc_range[1]
	print('DEM: Generating OK! Saving')
	dem.Save()
	#dem.Plot()
	print('DEM: Saveing OK! Making grid...')
	#dem.Load()
	dem.MakeGrid()
	dem.Save('../dem-grid.txt', 'grid')
	print('DEM: ploting')
	#dem.Plot(plot_type='grid')
	print('DEM: Finish')

def GenerateDEM():
	# 0. 配置参数
	ground3D_file = '../ground3D-clean.txt'
	# 1. 初始化DEM
	res = [10,10]
	dem = DEMModel(res)
	# 2. 读取地形点
	print('DEM: Reading ground points...')
	ground_pts = []
	ground_licheng = []
	f_ground = open(ground3D_file)
	last_pt = [0,0]
	for line in f_ground:
		line.strip('\n')
		pt = list(eval('['+line+']'))
		if IsSimilar(pt, last_pt):
			continue
		last_pt = pt
		ground_licheng.append(pt[0])
		ground_pts.append(pt[1:])
		# 更新DEM范围
		dem.Expand(pt[1:3])

	# 修饰地形点
	ground_pts.append([-3000, 0, 10])
	ground_pts.append([-3000, 1000, 15])
	ground_pts.append([-3000, 3000, 20])
	ground_pts.append([-3000, 4000, 10])
	ground_pts.append([-1000, 0, 5])
	ground_pts.append([-1000, 3000, 15])
	ground_pts.append([-1000, 6000, 10])
	ground_pts.append([0, 0, 5])
	ground_pts.append([0, 1000, 10])
	ground_pts.append([0, 3000, 15])
	ground_pts.append([0, 4000, 20])
	ground_pts.append([0, 6000, 15])
	ground_pts.append([1000, 0, 5])
	ground_pts.append([1000, 3000, 10])
	ground_pts.append([1000, 5000, 15])
	ground_pts.append([2000, 0, 5])
	ground_pts.append([2000, 2000, 15])
	ground_pts.append([2000, 3000, 10])
	ground_pts.append([2000, 4000, 20])
	ground_pts.append([3000, 0, 5])
	ground_pts.append([3000, 1000, 10])
	ground_pts.append([3000, 3000, 15])
	ground_pts.append([3000, 6000, 10])
	
	# 3. 对地形点进行三角化剖分、生成DEM
	th = TriHelper()
	print('DEM: Making Delaunay')
	th.Delaunay(ground_pts)	
	dem.UpdatePoints(th, None)
	print('DEM: Making OK! Plotting Delaunay')
	#th.Plot()
	# 4. 保存
	print('DEM: Generating OK! Saving')
	filename = '../dem-global-%dx%d'%(res[0], res[1])
	dem.Save(filename, 'sparse')
	
	print('DEM: Saveing OK! Making grid...')
	dem.MakeGrid()
	dem.SmoothGrid()
	dem.Save('../dem-global-grid-%dx%d.txt'%(res[0], res[1]), 'grid')
	print('DEM: ploting')
	#dem.Plot(plot_type='grid')
	print('DEM: Finish')

def ModifyDEM(dem_file = '../dem-10x10.txt'):
	'完善DEM，使数据看起来更真实'
	dem = DEMModel()
	dem.Load(dem_file)

	dem.MakeGrid()
	# 这样效果非常不好
	# dem.LiftGrid([250, 300, 50])
	# dem.LiftGrid([540, 600, 40])
	# dem.LiftGrid([0, 0, 40])
	dem.Save('../dem-grid-mod.txt', 'grid')
	dem.Plot(plot_type='grid')

def GeneratePathHight():
	'路线的高程'
	pass
def Ground_Clean(ground3D_file='../ground3D.txt', out_file='../ground3D-clean.txt'):
	'清洗地形数据, 去除相同的点'
	f_ground = open(ground3D_file)
	out_ground = open(out_file, 'w')
	last_pt = [0.,0.]
	for line in f_ground:
		line.strip('\n')
		pt = list(eval('['+line+']'))
		if IsSimilar(pt, last_pt):
			continue
		last_pt = pt
		out_ground.write('%.3f, %.3f, %.3f, %.3f\n'%(pt[0],pt[1],pt[2],pt[3]))
	out_ground.close()
	f_ground.close()

