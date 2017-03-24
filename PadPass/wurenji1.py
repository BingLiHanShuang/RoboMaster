import cv2
import numpy as np
cap=cv2.VideoCapture("/Users/wzq/Downloads/20170313_143106.mp4")

while True:
    success, frame = cap.read()
    edged = cv2.Canny(frame, 500, 2000, apertureSize=5)
    contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for i in contours:
        x, y, w, h = cv2.boundingRect(i)
        if w<200 or h<200:
            continue
        print w,h
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

    #frame = cv2.GaussianBlur(frame, (5, 5), 1)
    # hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # sensitivity = 120
    # lower_white = np.array([0, 0, 180])
    # upper_white = np.array([255, 255, 250])
    # mask = cv2.inRange(hsv, lower_white, upper_white)
    # kernel = np.ones((5, 5), np.uint8)
    # img_dilation = cv2.dilate(mask, kernel, iterations=1)
    cv2.imshow("white",frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    continue








    # lower_red = np.array([90, 90, 90])
    # upper_red = np.array([255, 255, 255])
    # mask = cv2.inRange(hsv, lower_red, upper_red)
    # mask = cv2.GaussianBlur(mask, (5, 5), 1)
    # # kernel = np.ones((5, 5), np.uint8)
    # #
    # # img_dilation = cv2.dilate(mask, kernel, iterations=1)
    #
    # #cv2.waitKey(0)
    # contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # #peri = cv2.arcLength(c, True)
    # for cnt in contours:
    #     approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
    #     if len(approx) == 3 or len(approx):
    #         x, y, w, h = cv2.boundingRect(cnt)
    #         # if (w < 20 or h < 20 or w > 150 or h > 150):
    #         #     continue
    #         cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
    # cv2.imshow("", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break