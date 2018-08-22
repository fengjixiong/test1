# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8

import codecs
import xml.dom.minidom as xmlDom

class SchemeParser:
	def __init__(self):
		'初始化一个路线模型'
		self.tree = {}
		self.root_name = 'Scheme'
	
	def Load(self, xml_filepath):
		'加载模型文件'
		doc = xmlDom.parse(xml_filepath)
		root = doc.documentElement
		for secNode in root.childNodes:
			if secNode.nodeType != 1: # 滤掉纯字符串
				continue
			secName = secNode.nodeName
			print(secName)
			for itemNode in secNode.childNodes:
				if itemNode.nodeType != 1:
					continue
				itemName = itemNode.nodeName
				print(itemName)
				if itemNode.hasChildNodes():
					self.tree['%s.%s'%(secName,itemName)] = itemNode.firstChild.data
				else:
					self.tree['%s.%s'%(secName,itemName)] = None

	def GetValue(self, key_string):
		if key_string not in self.tree.keys:
			return None
		valueStr = self.tree[key_string]
		return valueStr


def Main():
	a = SchemeParser()
	a.Load('../railscheme-template.rsch')
	print(a.tree)

if __name__ == '__main__':
	Main()