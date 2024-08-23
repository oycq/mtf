import cv2
import numpy as np

# 定义图像的宽度和高度
width = 640
height = 480

# 读取 .raw 文件
with open('output.raw', 'rb') as f:
    raw_data = f.read()

# 将数据转换为 numpy 数组并重塑为图像
image = np.frombuffer(raw_data, dtype=np.uint8).reshape((height, width))

cv2.imshow('output', image)
cv2.waitKey(0)