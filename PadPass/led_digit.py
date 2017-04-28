import os
import cv2
import numpy as np
path="/Users/wzq/Downloads/led_screen"
dirs=os.listdir(path)
count=0
for i,j in enumerate(dirs):
    if j in [".DS_Store"]:
        continue
    frame=cv2.imread(path+"/"+j)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_white = np.array([0, 0, 210])
    upper_white = np.array([255, 255, 255])
    kernel = np.ones((3, 3), np.uint8)

    mask = cv2.inRange(hsv, lower_white, upper_white)

    mask = cv2.dilate(mask, kernel, iterations=1)
    #cv2.imshow("test",mask)

    # digitCnts = []
    contours1, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for i, cnt in enumerate(contours1):
        x1, y1, w1, h1 = cv2.boundingRect(cnt)
        # print x1, y1, w1, h1

        if (h1 < 10):
            continue
        cv2.imwrite("/Users/wzq/Downloads/led_digit/"+str(count)+".jpg",frame[y1:y1+h1, x1:x1+w1])
        count+=1
        # digitCnts.append(cnt)

    #cv2.waitKey(0)
