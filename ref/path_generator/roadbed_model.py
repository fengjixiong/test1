# -*- coding: utf-8 -*-
# fengjx
# 2017.12.28

import codecs

MAXBIANPO = 4

class BianPo: # 边坡
	def __init__(self):
		self.TianwaType #填挖类型（1-路基，2-路堑）
		self.Dangqiang  #挡墙
		self.Huqiang    #护墙
		self.BianpoWidth = [0 for i in range(0,MAXBIANPO)] #边坡坡率
		self.BianpoHeight = [0 for i in range(0,MAXBIANPO)] #边坡高度
		self.BianpoPingtaiWidth = [0 for i in range(0,MAXBIANPO)] #边坡平台宽

class LuJi: #路基
	def __init__(self):
		self.Guanhao = '' #冠号
		self.Licheng = 0.0 #里程
		# 路面信息
		self.Left_RoadWidth = 3.6 #左路面宽度
		self.Right_RoadWidth = 8.59 #右路面宽度
		# 边坡信息
		self.Left_BianPo = BianPo() #左边坡
		self.Right_BianPo = BianPo() #右边坡
	def Load(self, str):
		temp = str.split(',')
		self.Guanhao = eval(temp[0])
		self.Licheng = eval(temp[1])
		self.Left_BianPo.TianwaType = eval(temp[2])
		self.Left_BianPo.Dangqiang = eval(temp[3])
		self.Left_BianPo.Huqiang = eval(temp[4])
		for i in range(0, 4):
			index = 5+i*3;
			self.Left_BianPo.BianpoWidth = eval(temp[index])
			self.Left_BianPo.BianpoHeight = eval(temp[index+1])
			self.Left_BianPo.BianpoPingtaiWidth = eval(temp[index+2])
		self.Right_BianPo.TianwaType = eval(temp[17])
		self.Right_BianPo.Dangqiang = eval(temp[18])
		self.Right_BianPo.Huqiang = eval(temp[19])
		for i in range(0, 4):
			index = 20+i*3;
			self.Right_BianPo.BianpoWidth = eval(temp[index])
			self.Right_BianPo.BianpoHeight = eval(temp[index+1])
			self.Right_BianPo.BianpoPingtaiWidth = eval(temp[index+2])


class RoadbedModel:
	def __init__(self):
		self.model = []

	def Load(model_file):
		if model_file=None:
			model_file = '../bianpo.csv'
		f_in = codecs.open(model_file, encoding='utf-8')
		title_line = f_in.readline()
		for line in f_in:
			luji = Luji()
			luji.Load(line.strip('\n'))
			self.model.append(luji)

