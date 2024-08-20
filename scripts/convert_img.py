import cv2
import numpy as np

def convert_to_raw(image_path, output_path, width=640, height=480):
    # 读取图像
    image = cv2.imread(image_path)

    # 获取原图像的尺寸
    orig_height, orig_width = image.shape[:2]

    # 计算原图像的宽高比
    orig_ratio = orig_width / orig_height
    target_ratio = width / height

    # 根据宽高比确定裁剪区域
    if orig_ratio > target_ratio:
        # 如果原图像更宽，则裁剪宽度
        new_width = int(target_ratio * orig_height)
        x_offset = (orig_width - new_width) // 2
        cropped_image = image[:, x_offset:x_offset + new_width]
    else:
        # 如果原图像更高，则裁剪高度
        new_height = int(orig_width / target_ratio)
        y_offset = (orig_height - new_height) // 2
        cropped_image = image[y_offset:y_offset + new_height, :]

    # 调整裁剪后的图像尺寸为目标尺寸
    resized_image = cv2.resize(cropped_image, (width, height))

    # 将彩色图像转换为灰度图像
    gray_image = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)

    # 转换为raw格式
    raw_image = gray_image.flatten()

    # 保存为raw文件
    raw_image.tofile(output_path)

# 使用例子
convert_to_raw('board.png', 'board.raw')
