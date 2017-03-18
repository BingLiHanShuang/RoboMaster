import cv2
import numpy as np
cap=cv2.VideoCapture("/Users/wzq/Downloads/20170313_143106.mp4")

while True:
    success, frame = cap.read()
    edged = cv2.Canny(frame, 500, 2000, apertureSize=5)
    contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    count = 0
    hasfind=0
    for i in contours:
        x, y, w, h = cv2.boundingRect(i)
        if w<200 or h<200:
            continue
        #print w,h
        #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

        frame1 = cv2.GaussianBlur(frame[y:y+h,x:x+w], (5, 5), 1)
        hsv = cv2.cvtColor(frame1, cv2.COLOR_BGR2HSV)
        lower_red = np.array([90, 90, 90])
        upper_red = np.array([255, 255, 255])
        mask = cv2.inRange(hsv, lower_red, upper_red)
        mask = cv2.GaussianBlur(mask, (5, 5), 1)
        gray = cv2.cvtColor(mask, cv2.COLOR_BGR2GRAY)
        circles = cv2.HoughCircles(gray, cv2.cv.CV_HOUGH_GRADIENT,2,10)
        if circles is not None:
            circles = np.round(circles[0, :]).astype("int")
            for (x1, y1, r1) in circles:
                pass
                # draw the circle in the output image, then draw a rectangle
                # corresponding to the center of the circle
                cv2.circle(frame, (x1+x, y1+y), r1, (0, 255, 0), 4)
                #cv2.rectangle(frame, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)

        # contours1, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        # if(contours1)==0:
        #     continue
        # # peri = cv2.arcLength(c, True)
        # for cnt in contours1:
        #     approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
        #     if count>10:
        #         hasfind=1
        #         break
        #     if len(approx) == 3 or len(approx):
        #         x1, y1, w1, h1 = cv2.boundingRect(cnt)
        #         if w1<10 or h1<10 :
        #             continue
        #         print w1,h1,float(w1)/float(h1)
        #         count+=1
        #         # if (w < 20 or h < 20 or w > 150 or h > 150):
        #         #     continue
        #         cv2.rectangle(frame, (x1+x, y1+y), (x1 + w1+x, y1 + h1+y), (0, 0, 255), 2)
        if hasfind==1:
            break
        print "end"
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