import cv2
import numpy as np
import sys
import glob

def draw_roi(img_name):
    # 读取图像
    color_img = cv2.imread(img_name)
    shape = color_img.shape[:2]

    #建立极大值抑制表
    nms_map = np.zeros(shape, dtype=np.uint8)

    # 将图像转换为灰度图像
    gray_img = cv2.cvtColor(color_img, cv2.COLOR_BGR2GRAY)

    cv2.imshow("gray_img", gray_img)

    # 应用阈值处理，小于60的像素变为0，大于等于60的像素变为255
    _, thresholded_img = cv2.threshold(gray_img, 140, 255, cv2.THRESH_BINARY)
    # 显示结果图像
    cv2.imshow("Thresholded Image", thresholded_img)


    roi_w = 30
    roi_h = 60

    margin = 3

    for row in range(shape[0]):
        for col in range(shape[1]):
            col_left = col - roi_w // 2
            col_right = col + roi_w // 2
            row_up = row - roi_h // 2
            row_down = row + roi_h // 2
            #检查是否被极大值抑制
            if nms_map[row, col] == 1:
                continue
            #检查roi区域是否出界
            if (col_left < 0) or (row_up < 0):
                continue
            if (col_right >= shape[1]) or (row_down >= shape[0]):
                continue
            #检查边缘四个角的
            if (thresholded_img[row_up, col_left] != thresholded_img[row_down, col_left]) or\
                (thresholded_img[row_up, col_right] != thresholded_img[row_down, col_right]) or\
                (thresholded_img[row_up, col_left] == thresholded_img[row_up, col_right]):
                    continue
            #检查中心区域四个角
            r = 2 #centor_size
            if (thresholded_img[row - r, col - r] != thresholded_img[row + r, col - r]) or\
                (thresholded_img[row - r, col + r] != thresholded_img[row + r, col + r]) or\
                (thresholded_img[row - r, col - r] == thresholded_img[row - r, col + r]):
                    continue
            #检查区域内是否有噪点
            roi = thresholded_img[row_up:row_down, col_left:col_right]
            diff = np.abs(roi[:, :-1] - roi[:, 1:])
            if ((diff > 0).sum(-1).max()) > 1:#检查每一行是否只有一个边沿
                continue
            #建立极大值抑制
            nms_map[max(row - roi_h, 0):row + roi_h, max(col - roi_w, 0):col + roi_w] = 1
            #可视化
            top_left = (col_left + margin, row_up + margin)
            bottom_right =  (col_right - margin, row_down - margin)
            color = (0, 255, 0)  # 绿色 (B, G, R)
            thickness = 2  # 线条粗细
            cv2.rectangle(color_img, top_left, bottom_right, color, thickness)
    cv2.imshow("color", color_img)
    cv2.waitKey(0)


imgs = glob.glob("board_640_480.png")
for img in imgs:
     draw_roi(img)
cv2.destroyAllWindows()