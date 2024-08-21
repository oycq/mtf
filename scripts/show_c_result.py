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

# 显示图像
input_img = cv2.imread("board_640_480.png")
input_img = input_img.astype(np.float32)
input_img[:,:,1] += image // 4
input_img = input_img / 256
cv2.imshow('input_img', input_img)
cv2.imshow('Raw Image', image)
cv2.waitKey(0)
cv2.destroyAllWindows()