# -*- coding: utf-8 -*-
# fengjx
# www.qhull.org
# 2017.12.6

import numpy as np
import scipy.spatial as sp
import matplotlib.pyplot as plt

class TriHelper:
	def __init__(self):
		self.tri = None
		self.pts = None
	def Delaunay(self, points_3d):
		'三角剖分,输入参数是list'
		self.pts = np.array(points_3d)
		self.tri = sp.Delaunay(self.pts[:,:2]) #用前两列做2D三角剖分
	def FindInTriangle(self, pt_2d):
		'查找pt_2d所在的三角形序号，没有返回-1'
		pt = np.array(pt_2d)
		if self.tri == None:
			return -1
		else:
			return self.tri.find_simplex(pt)
	def Get3D(self, pts_2d, default_value=0):
		'获取任何一个2D点的插值3D值输入可以是一个矩阵'
		'每一行代表一个点，矩阵有2列'
		pts = np.array(pts_2d)
		pts_z = [] # 返回的结果
		last_valid_index = 0 # 以防有点不在任意一个三角形中，取上一个三角形
		triangle_index = self.tri.find_simplex(pts) # 获取每个点所在的三角形索引
		if type(triangle_index) != np.ndarray:
			triangle_index = (triangle_index,)
		for i, index in enumerate(triangle_index):
			if index == -1: # 如果没在任意一个三角形中，设置默认值
				pt_z = default_value
				pts_z.append(pt_z)
				continue
			else:
				last_valid_index = index
			# 获取重心坐标计算矩阵和偏移向量
			transform = self.tri.transform[index,:2]
			base_vector = self.tri.transform[index, 2]
			pt_2d = pts[i]
			pt_2d_base = pt_2d - base_vector #归一化
			# 计算三角形前两个顶点的权重
			weight_xy = transform.dot(np.transpose(pt_2d_base))
			# 计算第三个顶点的权重，是1-前两个顶点的权重
			weight_z = 1-weight_xy.sum(axis=0)
			if type(weight_z) != np.ndarray:
				weight = np.r_[weight_xy, weight_z] # 行扩展
			else:
				weight = np.c_[np.transpose(weight_xy), weight_z] #列扩展
			# 找出三角形的顶点索引
			vertex_index = self.tri.simplices[index]
			vertex_z = self.pts[vertex_index, 2]
			# 插值点为内积
			pt_z = np.array(vertex_z).dot(weight)
			pts_z.append(pt_z)

		if len(triangle_index) == 1:
			return pt_z
		else:
			return pts_z
	def Plot(self):
		# 画出来
		plt.triplot(self.pts[:,0], self.pts[:,1], self.tri.simplices.copy())
		plt.plot(self.pts[:,0], self.pts[:,1], '.')
		plt.show()

