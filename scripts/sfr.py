import cv2
import numpy as np
from numpy.polynomial import Polynomial

H = 56
W = 34
LEFT = 208 - 1 
TOP = 280 - 1

img = cv2.imread("board_640_480.png", 0)[TOP:TOP+H, LEFT:LEFT+W].astype(np.float32) / 255.0
y = np.zeros(H)

for i in range(H):
    a = 0.0
    b = 0.0
    for j in range(W-1):
        d = img[i, j+1] - img[i, j]
        a += d
        b += j * d
    y[i] = b / a

x = np.arange(y.shape[0]).astype(np.float32)
p = Polynomial.fit(x, y, 1)
slope = p.convert().coef[1]

print("slope = %.5f"% (np.atan(slope) / 3.1415926 * 180))