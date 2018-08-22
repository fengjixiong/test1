# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import codecs
import math
import xml.dom.minidom as xmlDom
from geometry_helper import GeometryHelper

class PathModel:
	def __init__(self):
		'初始化一个路线模型'
		# 定义两个成员
		self.string = ''
		self.interp_points = []
		self.interp_points_items = ['point_index', 'section_index', 'point_type', 'licheng', 
									'point2D', 'direction', 'normal']
		self.interp_len = 10
		self.gh = GeometryHelper()
        self.sections = []
		self.itemnames = ['section_index', 'section_type', 
                          'data_valid', 
                          'start_licheng','end_licheng', 
						  'line_or_circle', 
                          'circle_side', 'circle_R', 'circle_center', 
						  'start_pt', 'end_pt', 
                          'start_direction', 'end_direction', 
                          'start_normal','end_normal']
		self.section_types = {1:'roadbed', 2:'tunnel', 3:'bridge', 4:'station'}

	def MakeInterpPoints(self):
		'计算插值点'
		self.interp_points.clear()
		for sec_index, sec in enumerate(self.sections):
			total_len = sec['start_licheng']
			pt_index = 0
			while True:
				i_pt = {}.fromkeys(self.interp_points_items)
				i_pt['point_index'] = pt_index
				i_pt['section_index'] = sec_index
				i_pt['point_type'] = sec['section_type']
				i_pt['licheng'] = total_len

				if total_len >= sec['end_licheng']:
					break
				if (total_len + self.interp_len) > sec['end_licheng']:
					total_len = sec['end_licheng']
				normal = self.gh.GetNormal(sec['start_direction'], sec['circle_side'])
				
				if sec['section_type'] == 'roadbed': # 路基
					i_pt['point2D'], i_pt['direction'] = self.gh.GetPointByCircleLicheng(
						sec['start_pt'], sec['start_direction'], i_pt['licheng']-sec['start_licheng'], sec['circle_center'],sec['circle_R'], sec['circle_side'])
				else:# 桥梁或者隧道
					i_pt['point2D'], i_pt['direction'] = self.gh.GetPointByLineLicheng(
						sec['start_pt'], sec['start_direction'], i_pt['licheng']-sec['start_licheng'])
					#print(total_len, pt, direction, normal)
				total_len += self.interp_len
				pt_index += 1
				i_pt['normal'] = self.gh.GetNormal(i_pt['direction'], sec['circle_side'])

				self.interp_points.append(i_pt)
				

	def Generate(self, licheng, secType, R=1000.0, startPt=[0.0,0.0], startDirection=[0,1], x_min=-1000.0, x_max=1000.0):
		if len(licheng) == 0 or len(licheng) != len(secType):
			print('cannot init pathsection')
			return
		for i in range(0, len(licheng)):
			sec = {}.fromkeys(self.itemnames)
			sec['section_index'] = i
			sec['data_valid'] = False
			sec['section_type'] = self.section_types[secType[i]]
			sec['circle_side'] = 'right'
			sec['circle_R'] = R
			sec['start_licheng'] = licheng[i][0]
			sec['end_licheng'] = licheng[i][1]
			self.sections.append(sec)

		self.sections[0]['start_pt'] = startPt
		self.sections[0]['start_direction'] = startDirection
		count = len(self.sections)
		for i, sec in enumerate(self.sections):
			if i > 0:
				sec['start_pt'] = self.sections[i-1]['end_pt']
				sec['start_direction'] = self.sections[i-1]['end_direction']
			sec['start_normal'] = self.gh.GetNormal( sec['start_direction'], sec['circle_side'])
			if sec['start_pt'][0] < x_min:
				sec['circle_side'] = 'right'
			if sec['start_pt'][0] > x_max:
				sec['circle_side'] = 'left'
			lc = sec['end_licheng'] - sec['start_licheng']
			if sec['section_type'] == 'roadbed': # 路基
				sec['line_or_circle'] = 'circle'
				sec['circle_center'] = self.gh.GetCenterPoint(
					sec['start_pt'], sec['start_direction'], sec['circle_R'], sec['circle_side'])
				sec['end_pt'], sec['end_direction'] = self.gh.GetPointByCircleLicheng(
					sec['start_pt'], sec['start_direction'], lc, sec['circle_center'],sec['circle_R'], sec['circle_side'])
			else:
				sec['line_or_circle'] = 'line'
				# if lc > 1000:
				# 	lc = 1000
				sec['end_pt'], sec['end_direction'] = self.gh.GetPointByLineLicheng(
					sec['start_pt'], sec['start_direction'], lc)
			sec['end_normal'] = self.gh.GetNormal(sec['end_direction'], sec['circle_side'])
			sec['data_valid'] = True
	
	def GetLichengInfo(self, licheng):
		'获取licheng处的信息'
		# 在路线范围之外
		if licheng < self.sections[0]['start_licheng'] or licheng > self.sections[-1]['end_licheng']:
			return None
		sec = {}
		for sec_index in range(0, len(self.sections)):
			sec = self.sections[sec_index]
			if sec['start_licheng'] <= licheng < sec['end_licheng']:
				break
		#print(sec['section_type'])
		len_licheng = licheng - sec['start_licheng']
		pt = None
		direct = None
		if sec['section_type'] == 'roadbed': # 路基
			pt, direct = self.gh.GetPointByCircleLicheng(
				sec['start_pt'], sec['start_direction'], len_licheng, 
				sec['circle_center'],sec['circle_R'], sec['circle_side'])
		else: # 桥梁和隧道
			pt, direct = self.gh.GetPointByLineLicheng(
				sec['start_pt'], sec['start_direction'], len_licheng)
		norm = self.gh.GetNormal(direct, sec['circle_side'])
		return sec, pt, direct, norm
	
	def GetLichengPoint(self, licheng, x):
		'在licheng处，横向值为x的2维点'
		sec, pt, direct, norm = self.GetLichengInfo(licheng)
		if math.fabs(x) < 0.001:
			return pt
		ptNew = [0,0]
		if x > 0: # 右侧
			ptNew, direct = self.gh.GetPointByLineLicheng(pt, norm, x)
		else: 
			ptNew, direct = self.gh.GetPointByLineLicheng(pt, norm, -x)
		return ptNew
	
	def Save(self, modelFile):
		'保存模型文件'
		if self.sections == []:
			return
		# 替换open，可以使用encoding
		file = codecs.open(modelFile, 'w', encoding='utf-8')
		file.write('<?xml version="1.0" encoding="utf8"?>\n')
		file.write('<path>\n')
		for sec in self.sections:
			file.write('  <section>\n')
			for key in self.itemnames:
				valueStr = str(sec[key])
				valueType = type(sec[key]).__name__
				file.write('    <%s datatype="%s">%s</%s>\n'%(key, valueType, valueStr, key))
			file.write('  </section>\n')
		file.write('</path>\n')
		file.close()
	
	def SaveInterpPoints(self, filename):
		'保存插值点'
		file = codecs.open(filename, 'w', encoding='utf-8')
		for itemname in self.interp_points_items:
			file.write(itemname + ' | ')
		file.write('\n')
		for i_pt in self.interp_points:
			for itemname in self.interp_points_items:
				file.write(str(i_pt[itemname]) + ' | ')
			file.write('\n')

	def LoadInterpPoints(self, filename):
		'加载插值点'
		file = codecs.open(filename, 'r', encoding='utf-8')
		titile = file.readline() # 过滤掉标题行
		self.interp_points = []
		for line in file:
			line = line.strip('| \n')
			values = line.split('|')
			#print(values)
			i_pt = {}.fromkeys(self.interp_points_items)
			for i, value in enumerate(values):
				itemname = self.interp_points_items[i]
				if itemname != 'point_type':
					value = eval(value.strip(' '))
				else:
					value = value.strip(' ')
				i_pt[itemname] = value
				self.interp_points.append(i_pt)


	def Load(self, modelFile):
		'加载模型文件'
		doc = xmlDom.parse(modelFile)
		root = doc.documentElement
		for secNode in root.childNodes:
			if secNode.nodeType != 1 or secNode.nodeName != 'section':
				continue
			sec = {}.fromkeys(self.itemnames)
			for itemNode in secNode.childNodes:
				if itemNode.nodeType != 1:
					continue
				itemName = itemNode.nodeName
				itemValueStr = itemNode.firstChild.data
				if itemName not in self.itemnames:
					continue
				itemType = itemNode.getAttribute('datatype')
				itemValue = None
				if itemType == 'str':
					itemValue = itemValueStr
				elif itemType == 'int':
					itemValue = int(itemValueStr)
				elif itemType == 'float':
					itemValue = float(itemValueStr)
				elif itemType == 'NoneType':
					pass
				elif itemType == 'list':
					#lstStr = itemValueStr.replace('[','').replace(']','').split(',')
					itemValue = eval(itemValueStr)
				sec[itemName] = itemValue
			#print(sec)
			self.sections.append(sec)

	def GetPoints(self):
		x = []
		y = []
		types = []
		for item in self.interp_points:
			x.append(item['point2D'][0])
			y.append(item['point2D'][1])
			types.append(item['point_type'])
		return x, y, types