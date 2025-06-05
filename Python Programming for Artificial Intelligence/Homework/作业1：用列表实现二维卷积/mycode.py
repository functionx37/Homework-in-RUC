
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

#输入图像
im = np.array(Image.open('ruc.jpg').convert('L'))
plt.imshow(im)
plt.show()
# im是输入图像转换成的list
im = im.tolist()

# ker是卷积核对应的list
ker = np.array([[1,1,1,-1,-1],
                [1,0,0,0,-1],
                [1,0,0,0,-1],
                [1,0,0,0,-1],
                [1,1,1,-1,-1]])

ker = ker.tolist()

def conv2d(im , kernel, stride =1):    
    # 请完成本函数代码
    rev = kernel.copy()
    rev.reverse()
    for it in rev:
        it.reverse()
    H = len(im)
    W = len(im[0])
    h = len(rev)
    w = len(rev[0])
    im_out = []
    for i in range(0,H-h+1,stride):
        im_out.append([])
        for j in range(0,W-w+1,stride):
            im_out[-1].append(0)
            for u in range(0,min(h,H-i)):
                for v in range(0,min(w,W-j)):
                    im_out[-1][-1] += im[i+u][j+v]*rev[u][v]
    return im_out

# 用ker铺im的处理结果，更改这行参数
im_out = conv2d(im,ker,1)

#显示处理后的图像
print(len(im_out))
print(len(im_out[0]))
im_out = np.array(im_out)
im1 = Image.fromarray(im_out.astype('uint8'))
plt.imshow(im1)
plt.show()

ker = np.array([[1,1,1,-1,-1],
                [1,0,0,0,-1],
                [1,0,0,0,-1],
                [1,0,0,0,-1],
                [1,1,1,-1,-1]])
ker2 = ker/25
ker2 = ker2.tolist()
print(ker2)

# 用ker2铺im的处理结果
im_out = conv2d(im,ker2,2)

# 显示处理后的图像
im_out = np.array(im_out)
im2 = Image.fromarray(im_out.astype('uint8'))
plt.imshow(im2)
plt.show()

k3 =[[1, 0, -1], 
     [2, 0, -2], 
    [1, 0, -1]]
# 用ker2铺im的处理结果
im_out = conv2d(im,k3,1)

# 显示处理后的图像
im_out = np.array(im_out)
im2 = Image.fromarray(im_out.astype('uint8'))
plt.imshow(im2)
plt.show()

k3 =[[1, 2, 1], 
     [0, 0, 0], 
    [-1, -2, -1]]
# 用ker2铺im的处理结果
im_out = conv2d(im,k3,1)

# 显示处理后的图像
im_out = np.array(im_out)
im2 = Image.fromarray(im_out.astype('uint8'))
plt.imshow(im2)
plt.show()

im = [[ 8, -1, -8,  2,  3, -9, -2,  5,  5,  5, -8],
       [ 8,  2, -8, -4,  1,  7,  6, -7,  8, -4, -6],
       [ 5,  7, -1,  2,  1, -7, -4,  1, -1,  1,  0],
       [-3,  0, -2, -2, -5, -4, -7,  5,  0, -3, -1],
       [-5,  0,  3,  1, -1,  4,  8, -2, -3, -8,  5],
       [-2, -7, -6, -3, -3, -3,  2,  5, -7,  7,  3],
       [ 8, -9, -3,  3,  0, -4, -6, -8,  6, -8, -7],
       [-6,  0,  0,  8,  3, -6,  1,  8, -2,  2,  7],
       [-4, -8,  6, -3, -9,  2, -5, -4, -9,  0, -5],
       [-6,  4, -1,  0, -4,  7,  4,  5,  0, -4, -6],
       [ 6,  4,  2, -4,  7,  4,  8, -5, -1, -7, -5]]

K = [[ 5, -3,  3,  5,  7],
       [ 0, -9,  8, -8, -8],
       [ 6,  6,  1, -5,  3],
       [-3, -9,  5,  0,  1],
       [ 1, -9, -6,  0,  8]]

print(conv2d(im, K))
print(conv2d(im, K, 2))

im = [[1, 1, 0, 0],
             [1, 1, 0, 0],
             [0, 0, 1, 1],
             [0, 0, 1, 1]]
K = [[1, 1],
      [1, 1]]
print(conv2d(im, K, 2))