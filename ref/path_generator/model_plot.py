# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import os, sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from path_model import PathModel

def PlotModel(my_path, different_color=True):
	x, y, sec = my_path.GetPoints()
	if different_color == False:
		plt.plot(x, y)
	else:
		for i in range(0, len(x)-2):
			if sec[i] == 'roadbed':
				line1 = plt.plot(x[i:i+2], y[i:i+2], 'r-')
			elif sec[i] == 'tunnel':
				line2 = plt.plot(x[i:i+2], y[i:i+2], 'b-')
			else:
				line3 = plt.plot(x[i:i+2], y[i:i+2], 'g-')
		#plt.legend((line1,line2,line3),('roadbed','tunnel','bridge'))

	direction = my_path.sections[0]['start_direction']
	plt.title('Automatically Generated Path: Start at ' + str(direction))
	return plt

def PlotGroundPoints(ground3D_file, sample_N=15, type='2d'):
	if ground3D_file == '':
		ground3D_file = '../ground3D.txt'
	f_ground = open(ground3D_file)
	sample_i = 0
	x_pts = []
	y_pts = []
	z_pts = []
	for line in f_ground:
		if sample_N != None and sample_i < sample_N:
			sample_i += 1
			continue
		sample_i = 0
		line.strip('\n')
		pt = list(eval('['+line+']'))
		#第一列是里程，后面三列是数值
		x_pts.append(pt[1])
		y_pts.append(pt[2])
		z_pts.append(pt[3])
	if type == '2d':
		plt.plot(x_pts, y_pts, 'y.')
	elif type == '3d':
		fig = plt.figure()
		ax = Axes3D(fig)
		x = np.array(x_pts)
		y = np.array(y_pts)
		z = np.array(z_pts)
		ax.scatter(x, y, z, cmap='rainbow')


def PlotVerticalLine(my_path):
	'绘制纵断面，目前还没有高程信息'
	pass