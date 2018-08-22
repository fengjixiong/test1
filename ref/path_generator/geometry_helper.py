# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import math

class GeometryHelper:

	def __init__(self):
		self.accuracy = 3
	
	def TruncVec(self, value):
		vecLen = len(value)
		for i in range(0, vecLen):
			value[i] = round(value[i], self.accuracy)
		return value
		
	def GetLength(self, vec2d):
		'获取向量的模长'
		vecLen = math.sqrt(vec2d[0]*vec2d[0] + vec2d[1]*vec2d[1])
		return round(vecLen, self.accuracy)
		
	def GetAngle(self, direction):
		''' 
		方向角为[0, 360]，参考为x轴方向
		根据起点、正切方向、半径和方向确定圆心
		direction是一个二维数组[dx, dy]，且其模为1
		'''
		dx = direction[0]
		dy = direction[1]
		theta = 0.0
		if math.fabs(dx) < 0.001:
			if dy > 0:
				theta = math.pi / 2
			else:
				theta = 3 * math.pi / 2
		else:
			theta0 = math.atan(dy / dx)
			if dx > 0 and dy >= 0: # 第一象限
				theta = theta0
			elif dx < 0 and dy >= 0: # 第二象限
				theta = math.pi + theta0
			elif dx < 0 and dy <= 0: # 第三象限
				theta = math.pi + theta0
			else: #第四象限
				theta = 2*math.pi + theta0
		return round(theta, self.accuracy)

	def GetNormal(self, direction, side):
		'获取法向量，side决定左右边'
		normal = [0.0, 0.0]
		dx = direction[0]
		dy = direction[1]
		if side.lower() == 'left':
			normal = [-dy, dx]
		else:
			normal = [dy, -dx]
			
		return self.TruncVec(normal)
		
	def GetPointByLineLicheng(self, startPt, direction, licheng):
		pt_xy = [0, 0]
		dx = direction[0]
		dy = direction[1]
		directionLen = math.sqrt(dx*dx + dy*dy)
		pt_xy[0] = startPt[0] + licheng*dx/directionLen
		pt_xy[1] = startPt[1] + licheng*dy/directionLen
		
		return self.TruncVec(pt_xy), self.TruncVec(direction)


	def GetCenterPoint(self, startPt, direction, R, side):
		'获取圆心'
		centerPt = [0, 0]
		normal = self.GetNormal(direction, side)
		centerPt, dir = self.GetPointByLineLicheng(startPt, normal, R)
		return self.TruncVec(centerPt)

	
	def GetPointByCircleLicheng(self, startPt, direction, licheng, center, R, side):
		'获取下一个圆弧点'
		dx = startPt[0]-center[0]
		dy = startPt[1]-center[1]
		angle_A = self.GetAngle([dx, dy]) # 开始的圆心角
		angle_delta = licheng / R # 里程对应的圆心角
		angle_B = 0.0 # 终止的圆心角
		if side.lower() == 'right':
			angle_B = angle_A - angle_delta
			if angle_B < 0:
				angle_B = 2*math.pi + angle_B
		else:
			angle_B = angle_A + angle_delta
			if angle_B > 2*math.pi:
				angle_B = angle_B - 2*math.pi
		angle_B = round(angle_B, self.accuracy)
		endPt = [0, 0]
		endPt[0] = center[0] + R*math.cos(angle_B)
		endPt[1] = center[1] + R*math.sin(angle_B)
		tempNormal = [math.cos(angle_B), math.sin(angle_B)]
		new_direction = self.GetNormal(tempNormal, side)
		#print([side, angle_A*57.3, angle_delta*57.3, angle_B*57.3])
		return self.TruncVec(endPt), self.TruncVec(new_direction)
	