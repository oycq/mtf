import cv2
import numpy as np

img = cv2.imread("output.bmp")
cv2.imshow('output', img)
cv2.waitKey(0)