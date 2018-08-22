# -*- coding: utf-8 -*-
# DEM类
# fengjx
# 2017.12.11
import math
import codecs
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt

def ExpandRange(org_range, x, y):
	'用新的点扩展范围 range[xmin,xmax,ymin,ymax]'
	[x_min, x_max, y_min, y_max] = org_range
	if x < x_min:
		x_min = math.floor(x)
	if x > x_max:
		x_max = math.ceil(x)
	if y < y_min:
		y_min = math.floor(y)
	if y > y_max:
		y_max = math.ceil(y)
	return [x_min, x_max, y_min, y_max]

def FloorList(x):
	return [math.floor(i) for i in x]

def Smooth(np_array, window_size=[5,5]):
	xn, yn = np_array.shape
	dx = round((window_size[0]-1)/2)
	dy = round((window_size[1]-1)/2)
	#count = window_size[0]*window_size[1]
	zero_pts = 0
	filter55 = np.array([[ 0.01209244,  0.02561806,  0.0329631 ,  0.02561806,  0.01209244],
       				  [ 0.02561806,  0.06153708,  0.06977786,  0.06153708,  0.02561806],
       				  [ 0.0329631 ,  0.06977786,  0.08957363,  0.06977786,  0.0329631 ],
       				  [ 0.02561806,  0.06153708,  0.06977786,  0.06153708,  0.02561806],
       				  [ 0.01209244,  0.02561806,  0.0329631 ,  0.02561806,  0.01209244]])

	print([xn, yn])
	for i in range(dx, xn-dx-1):
		for j in range(dy, yn-dy-1):
			# if math.fabs(np_array[i,j]) > 0.001:
			# 	continue
			patch = np_array[i-dx:i+dx+1, j-dy:j+dy+1]
			patch_max = np.max(patch)
			patch_min = np.min(patch)
			# if math.fabs(patch_max) < 0.001 and math.fabs(patch_min) < 0.001:
			# 	zero_pts += 1
			# 	continue
			# np_array[i,j] = patch_max*0.9 + patch_min*0.1
			np_array[i,j] = np.sum(patch*filter55)
	return zero_pts

class DEMModel:
	def __init__(self, res=[10,10]):
		self.model = {}.fromkeys(['id','range', 'count', 'resolution','grid', 'grid_map',
								  'value', 'value_default', 'value_type', 'value_range'])
		# 在本模型中res是整数，因为要把真实的(x,y)作为稀疏表示的hash键，方便索引
		# 这样做的原因是这个DEM是一直变化的，不能把行序号和列序号作为(x,y)
		self.model['id'] = 'www.drawleader.com.cn'
		# 要求res和range都是整数，方便索引
		self.model['resolution'] = res
		self.model['range'] = [0, 0, 0, 0] #[xmin, xmax, ymin, ymax]
		self.model['value'] = {} # {(1,2):3.6}
		self.model['grid'] = None
		self.model['grid_map'] = {}
		self.model['count'] = [0,0]
		self.model['value_type'] = 'sparse' # 稀疏的方式保存值
		self.model['value_default'] = 0
		self.model['value_range'] = [1000.0, -1000.0] # z值得范围

	def Expand(self, pt_or_range):
		'根据pt2D扩展DEM范围'
		[x_min, x_max, y_min, y_max] = self.model['range']
		[x_res, y_res] = self.model['resolution']
		if len(pt_or_range) == 2:
			[x, y] = pt_or_range
			if x < x_min:
				x_min += x_res * math.floor((x - x_min)/x_res)
			if x > x_max:
				x_max += x_res * math.floor((x - x_max)/x_res)
			if y < y_min:
				y_min += y_res * math.floor((y - y_min)/y_res)
			if y > y_max:
				y_max += y_res * math.floor((y - y_max)/y_res)
			#self.model['range'] = [x_min, x_max, y_min, y_max]
			if self.model['range'][0] != x_min or self.model['range'][1] != x_max:
				self.model['count'][0] = (x_max - x_min)/x_res + 1
			if self.model['range'][2] != y_min or self.model['range'][3] != y_max:
				self.model['count'][1] = (y_max - y_min)/y_res + 1
			self.model['range'] = [x_min, x_max, y_min, y_max]
		elif len(pt_or_range) == 4:
			[x1, x2, y1, y2] = pt_or_range
			self.Expand([x1, y1])
			self.Expand([x2, y2])

	# def MergeDEM(self, new_dem):
	# 	'合并DEM'
	# 	[x_res1, y_res1] = self.model['resolution']
	# 	[x_res2, y_res2] = new_dem['resolution']
	# 	if x_res1 != x_res2 or y_res1 != y_res2:
	# 		raise(Exception('x resolution not equal, cannot merge'))
	# 		return
	# 	# 合并范围
	# 	if self.model['range'][0] > new_dem['range'][0]:
	# 		self.model['range'][0] = new_dem['range'][0]
	# 	if self.model['range'][1] < new_dem['range'][1]:
	# 		self.model['range'][1] = new_dem['range'][1]
	# 	if self.model['range'][2] > new_dem['range'][2]:
	# 		self.model['range'][2] = new_dem['range'][2]
	# 	if self.model['range'][3] < new_dem['range'][3]:
	# 		self.model['range'][3] = new_dem['range'][3]
	# 	# 合并值
	# 	for key in new_dem['value'].keys():
	# 		if key in self.model['value'].keys():
	# 			continue
	# 		self.model['value'][key] = new_dem['value'][key]

	def GetDemPoints(self, pt):
		'获取pt邻近的DEM网格点'
		[x_res, y_res] = self.model['resolution']
		x = x_res*math.floor(pt[0]/x_res)
		y = y_res*math.floor(pt[1]/y_res)
		return [x,y]

	def UpdatePoints(self, tri, pts_range):
		'获取区域内的点'

		# 扩展区域
		if pts_range == None:
			pts_range = self.model['range']
		else:
			self.Expand(pts_range)

		# 把要更新的点找出来
		[x_min, y_min] = self.GetDemPoints([pts_range[0], pts_range[2]])
		[x_max, y_max] = self.GetDemPoints([pts_range[1], pts_range[3]])
		[z_min, z_max] = self.model['value_range']
		[x_res, y_res] = self.model['resolution']
		pts = []
		for x in range(x_min, x_max+x_res, x_res):
			for y in range(y_min, y_max+y_res, y_res):
				if (x, y) not in self.model['value'].keys():
					pts.append([x, y])
		if len(pts) == 0:
			return

		# 用TIN获得点的高程
		print('DEM: total ', len(pts), ' to be updated...')
		pts_z = tri.Get3D(pts, default_value=0)
		[v_min, v_max] = [np.min(pts_z), np.max(pts_z)]
		if z_min > v_min:
			z_min = v_min
		if z_max < v_max:
			z_max = v_max
		self.model['value_range'] = [z_min, z_max]

		# 更新模型
		pts_count = 0
		for i, pt in enumerate(pts):
			if math.fabs(pts_z[i]) < 0.001:
				continue
			self.model['value'][(pt[0], pt[1])] = pts_z[i]
			pts_count += 1
		print('DEM: ', pts_count, ' points are updated')

	def MakeGrid(self):
		'栅格化值'
		[x_min, x_max, y_min, y_max] = self.model['range']
		[x_res, y_res] = self.model['resolution']
		xn = math.floor((x_max - x_min)/x_res) + 1
		yn = math.floor((y_max - y_min)/y_res) + 1
		self.model['count'] = [xn, yn]
		self.model['grid'] = np.zeros([xn, yn])
		for x in range(x_min, x_max+x_res, x_res):
			i = round((x - x_min) / x_res)
			for y in range(y_min, y_max+y_res, y_res):
				j = round((y - y_min) / x_res)
				key = (x,y)
				value = self.model['value'].get(key)
				if value == None:
					value = self.model['value_default']
				self.model['grid'][i,j] = value
				self.model['grid_map'][(i,j)] = (x,y)
		#print(self.model['grid'].shape)
		[xn, yn] = self.model['grid'].shape
		self.model['count'] = [xn, yn]
				
	def Info(self):
		dem_info = ''
		dem_info += 'DEM file: %s\n'%self.model['id']
		[x_res, y_res] = self.model['resolution']
		dem_info += 'resolution: [%d, %d]\n'%(x_res, y_res)
		[xn, yn] = self.model['count']
		dem_info += 'count: [%d, %d]\n'%(xn, yn)
		[x_min, x_max, y_min, y_max] = self.model['range']
		dem_info += 'range: [%d, %d, %d, %d]\n'%(x_min, x_max, y_min, y_max)
		dem_info += 'value_type: "%s"\n'%self.model['value_type']
		dem_info += 'value_default: %.2f\n'%self.model['value_default']
		[z_min, z_max] = self.model['value_range']
		dem_info += 'value_range: [%.2f, %.2f]\n'%(z_min, z_max)
		dem_info += 'values: %d points\n'%len(self.model['value'].keys())
		return dem_info

	def Save(self, filename='../dem.txt', mode='sparse'):
		'保存DEM'
		self.model['value_type'] = mode
		dem_info = self.Info()
		print(dem_info)

		# 写文件头
		file = codecs.open(filename, 'w', encoding='utf-8')
		file.write(dem_info)

		# 写数值
		if mode == 'sparse': #稀疏保存
			pts = self.model['value']
			for key in pts.keys():
				value = pts.get(key)
				file.write('%d, %d, %.2f\n'%(key[0],key[1],value))
		elif mode == 'grid': # 栅格保存
			pts = self.model['grid']
			[xn, yn] = self.model['count']
			for i in range(0, xn):
				for j in range(0, yn):
					file.write('%.2f,'%pts[i][j])
				file.write('\n')
		else:
			print('No such value type: %s\n'%mode)

		file.close()

	def Load(self, filename='../dem.txt'):
		'加载DEM'
		file = codecs.open(filename, 'r', encoding='utf-8')
		# 读文件头
		line = file.readline()
		if line != 'DEM file: %s\n'%self.model['id']:
			print('This is not a valid DEM file')
			return
		for line in file:
			key = line.split(':')[0]
			if (key == 'values'):
				break
			value = eval(line.split(':')[1].strip('\n'))
			self.model[key] = value

		self.model['value'] = {}
		if self.model['value_type'] == 'grid':
			[xn, yn] = self.model['count']
			self.model['grid'] = np.zeros([xn, yn])
		x_index = 0
		for line in file:
			if self.model['value_type'] == 'sparse':
				temp = line.split(',')
				key = eval('(' + temp[0] + ',' + temp[1] + ')')
				value = eval(temp[2].strip('\n'))
				self.model['value'][key] = value
			elif self.model['value_type'] == 'grid':
				y_values = line.split(',')
				for y_index in range(0, yn):
					self.model['grid'][x_index][y_index] = eval(y_values[y_index])
				x_index += 1
			else:
				print('not valid type')
				break
		file.close()
		print(self.Info())

	def Plot(self, plot_type='sparse'):
		'绘制DEM'
		if plot_type == 'sparse':
			keys = self.model['value'].keys()
			for x, y in keys:
				plt.plot(x, y, 'r*')
		elif plot_type=='grid':
			[x_res, y_res] = self.model['resolution']
			[xn, yn] = self.model['count']
			#x = np.arange(0, xn)
			#y = np.arange(0, yn)
			
			z = self.model['grid'].T
			x = np.arange(0, z.shape[1])*x_res
			y = np.arange(0, z.shape[0])*y_res
			x, y = np.meshgrid(x,y)
			fig = plt.figure()
			ax = Axes3D(fig)
			ax.plot_surface(x, y, z, rstride=10, cstride=10, cmap='rainbow')

		plt.show()

	def SmoothGrid(self):
		'平滑DEM点'
		iter_count = 0
		while True:
			zero_pts = Smooth(self.model['grid'])
			break
			print('iter-',iter_count, ': ', zero_pts, ' points left...')
			#print(self.model['grid'][0:10, 0:10])
			if zero_pts == 0:
				break
			iter_count += 1
		print('smooth finish')

	def LiftGrid(self, pt, delta=200, radius=400.0, spread_type='Guassian'):
		'设(x,y)点为z，然后在半径radius内用Guassian分布赋值'

		# 滤除无效参数
		[xn, yn] = self.model['count']
		print([xn, yn])
		if pt[0] >= xn or pt[0] < 0 or pt[1] > yn or pt[1] < 0:
			return

		for i in range(0, xn):
			for j in range(0, yn):
				# 如果距离超出半径范围，则不修改
				dist = 0.7*(math.fabs(i*1.0-pt[0]) + math.fabs(j*1.0-pt[1]))
				if dist > radius:
					continue
				# 如果当前点是由Delaunay获得的，不允许修改
				(x,y) = self.model['grid_map'][(i,j)]
				if (x,y) in self.model['value'].keys():
					continue
				di = (i-pt[0])*1.0/delta
				dj = (j-pt[1])*1.0/delta
				if spread_type == 'Guassian':
					z_ij = pt[2] * math.e**(-di*di - dj*dj)
					self.model['grid'][i,j] += z_ij
				if i%100 == 0 and j %100 == 0:
					print([i, j, z_ij])
