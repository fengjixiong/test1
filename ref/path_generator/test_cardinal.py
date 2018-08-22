# 测试各种插值曲线
import numpy as np
from matplotlib import pyplot as plt


def Cardinal(pts, tao=0.5, step=0.1):
    'cardinal曲线'
    pts_card = []
    cardinal_matrix = np.array([[-tao, 2-tao, -2+tao, tao],
                              [2*tao, -3+tao, 3-2*tao, -tao],
                              [-tao, 0, tao, 0],
                              [0, 1, 0, 0]])
    for i in range(len(pts)-1):
        if i == 0:
            index = [0, 0, 1, 2]
        elif i == len(pts)-2:
            index = [i-1, i, i+1, i+1]
        else:
            index = [i-1, i, i+1, i+2]
        
        cardinal_coeff = np.zeros([4,2])
        for j in range(4):
            #print(cardinal_matrix[j,:])
            #print(pts[index,0])
            cardinal_coeff[j,0] = cardinal_matrix[j,:].dot(pts[index,0])
            cardinal_coeff[j,1] = cardinal_matrix[j,:].dot(pts[index,1])
        u = 0
        while u <= 1:
            u_array = np.array([u*u*u, u*u, u, 1])
            new_pt = cardinal_coeff.transpose().dot(u_array).tolist()
            pts_card.append(new_pt)
            u = u + step
    return np.array(pts_card)

def main():
    pts = np.array([[0,0], [1,1], [2,0], [3, -2], [4,0], [5,3], [6,0], [7, -4]])
    pts_card = Cardinal(pts)
    plt.plot(pts[:,0], pts[:,1], 'r*')
    plt.plot(pts_card[:,0], pts_card[:,1])
    plt.title('Cardinal Spline')
    plt.show()

if __name__ == '__main__':
    main()