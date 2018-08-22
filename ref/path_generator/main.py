# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import os, sys
import matplotlib.pyplot as plt
from path_model import PathModel
from model_plot import *
from model_generator import *

def Test_Generate_Path():
	'测试生成路线'
	lichengFile = '../licheng.txt'
	sectionFile = '../section.txt'
	modelFile = '../pathmodel.xml'
	interpFile = '../interp_points.txt'
	direction = [1,1]

	licheng = GetSectionLicheng(lichengFile)
	secType = GetSectionType(sectionFile)
	licheng = ModifyLicheng(licheng, secType, '../newlicheng.txt')
	my_path = PathModel()
	my_path.Generate(licheng, secType, R=1000.0, startPt=[0.0,0.0], startDirection=direction, x_min=-1000.0, x_max=1000.0)
	my_path.Save(modelFile)
	my_path.MakeInterpPoints()
	my_path.SaveInterpPoints(interpFile)
	PlotModel(my_path)

	new_path = my_model.PathModel()
	new_path.LoadModel()
	PlotModel(new_path)

def Test_Generate_Ground():
	'测试生成地面点'
	ground2D_file = '../ground-origin-2D.txt'
	ground3D_file = '../ground-origin-3D.txt'
	licheng_file = '../licheng.txt'
	licheng_new_file = '../newlicheng.txt'
	modelFile = '../pathmodel.xml'
	interpFile = '../interp_points.txt'

	my_path = LoadModel()
	#GenerateGroundPoints3D(my_path, ground2D_file, ground3D_file, licheng_file, licheng_new_file)
	PlotGroundPoints(ground3D_file)
	PlotModel(my_path)
	plt.show()

def Test_Ground_3D():
	pass
	#LoadGroundPoints()
	#ground3D_file = '../ground3D.txt'
	#PlotGroundPoints(ground3D_file, sample_N=None, type='3d')
	#plt.show()

def Test_DEM():
	ground3D_file = '../ground3D.txt'
	GenerateDEMfromTin(ground3D_file)

def Test_ModifyDEM():
	GenerateDEM()

def Test_Clean_Ground3D():
	Ground_Clean()

def Test_Main():
	#Test_Generate_Path()
	Test_Generate_Ground()
	#Test_Ground_3D()
	#Test_DEM()
	#Test_Clean_Ground3D()
	#Test_ModifyDEM()

if __name__ == '__main__':
	p = 'E:/people/hf/Railbase3D/Jx3DEngine/Python/autogen/path_generator'
	os.chdir(os.path.abspath('.')) # 切换路径
	Test_Main()
	