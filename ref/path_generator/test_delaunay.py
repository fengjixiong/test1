# -*- coding: utf-8 -*-
# fengjx
# 2017.11.8
# Examples from help(scipy.spatial)
# Triangulation of a set of points:
#
import numpy as np
import scipy.spatial as sp
import matplotlib.pyplot as plt

# 生成点
points = np.array([[0, 0], [0, 1.1], [1, 0], [1, 1]])

# Delaunay三角剖分
tri = sp.Delaunay(points)

# 画出来
plt.triplot(points[:,0], points[:,1], tri.simplices.copy())
plt.plot(points[:,0], points[:,1], 'o')
plt.show()

# 另一种快速画法
fig = sp.delaunay_plot_2d(tri)
fig.show()

# 显示三角形组成
print(tri.simplices)
# 结果表明，一共生成了2个三角形.这个结果的顺序也许会随机
# 第一个三角形由第2/3/0个点组成
# 第二个三角形由第3/1/0个点组成
#array([[2, 3, 0],                 # may vary
#       [3, 1, 0]], dtype=int32)

# 显示每个三角形的顶点
print(points[tri.simplices])
# 结果
#array([[[ 1. ,  0. ],            # may vary
#        [ 1. ,  1. ],
#        [ 0. ,  0. ]],
#       [[ 1. ,  1. ],
#        [ 0. ,  1.1],
#        [ 0. ,  0. ]]])
# 显示第1个三角形的第一个点
print(points[tri.simplices[1,1]])
# 结果
#array([ 0. ,  1.1])

# 显示第一个三角形的相邻三角形
#Triangle 0 is the only neighbor of triangle 1, and it's opposite to
#vertex 1 of triangle 1:
print(tri.neighbors[1])
# 结果
#array([-1,  0, -1], dtype=int32)

# 判断任意一个点在哪个三角形里
#We can find out which triangle points are in:
p = np.array([(0.1, 0.2), (1.5, 0.5), (0.5, 1.05)])
print(tri.find_simplex(p))
# 结果：（-1表示没有在任何一个三角形中
#array([ 1, -1, 1], dtype=int32)

#The returned integers in the array are the indices of the simplex the 
#corresponding point is in. If -1 is returned, the point is in no simplex.
#Be aware that the shortcut in the following example only works corretcly 
#for valid points as invalid points result in -1 which is itself a valid
#index for the last simplex in the list.
# 用p中有效的点来索引三角形
p_valids = np.array([(0.1, 0.2), (0.5, 1.05)])
tri.simplices[tri.find_simplex(p_valids)]
#array([[3, 1, 0],                 # may vary
#       [3, 1, 0]], dtype=int32)


# 计算重心坐标，可以用来根据权重插值
# tri.transform维数是[三角形数，点维数+1，点维数]
# tri.transform[:,ndim,:]是参考坐标
#We can also compute barycentric coordinates in triangle 1 for
#these points:
b = tri.transform[1,:2].dot(np.transpose(p - tri.transform[1,2]))
np.c_[np.transpose(b), 1 - b.sum(axis=0)]
#array([[ 0.1       ,  0.09090909,  0.80909091],
#       [ 1.5       , -0.90909091,  0.40909091],
#       [ 0.5       ,  0.5       ,  0.        ]])
#
#The coordinates for the first point are all positive, meaning it
#is indeed inside the triangle. The third point is on a vertex,
#hence its null third coordinate.