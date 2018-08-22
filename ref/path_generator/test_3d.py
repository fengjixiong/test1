from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

def Test_3D():
	fig = plt.figure()
	ax = Axes3D(fig)
	x = np.arange(-4,4, 0.25)
	y = np.arange(-4,4, 0.25)
	x, y = np.meshgrid(x,y)
	r = np.sqrt(x**2 + y**2)
	z = np.sin(r)
	ax.plot_surface(x, y, z, rstride=1, cstride=1, cmap='rainbow')
	plt.show()