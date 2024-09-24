import cv2
import numpy as np

# 读取图像，以灰度模式读取（第二个参数为0）
img = cv2.imread("img.pgm", 0)

# 将图像拉平为1D数组
flattened_img = img.flatten()

# 对图像像素值进行排序
sorted_pixels = np.sort(flattened_img)

total_pixels = len(sorted_pixels)
threshold_index = int(total_pixels * 0.4)


# 输出结果
print(f"前30%暗的像素的平均亮度: {sorted_pixels[threshold_index]}")

# 展示原图像
cv2.imshow('img', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
