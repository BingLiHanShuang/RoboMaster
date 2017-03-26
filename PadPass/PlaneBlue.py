import cv2
import numpy as np
import sys
sys.path.append('../Protocol/MainFold/Python')
import protocol

datainterface=protocol.Protocol('192.168.1.200',6000)
cap=cv2.VideoCapture(1)
cap.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 480)
i=1000
# while True:
#     success, frame = cap.read()
#     if success:
#         cv2.imwrite("plane/"+str(i)+".jpg",frame)
#         i+=1
#         cv2.imshow("a",frame)
#         if cv2.waitKey(1) & 0xFF == ord('q'):
#             break


while True:

    success, frame = cap.read()
    frame = cv2.imread("/Users/wzq/RoboMaster/PadPass/plane/1.jpg")
    green = np.uint8([[[0,0,255 ]]])

    lower_green = np.array([50, 90, 0])
    upper_green = np.array([70, 255, 255])

    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_green, upper_green)
    kernel = np.ones((5, 5), np.uint8)

    mask = cv2.dilate(mask, kernel, iterations=2)
    contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    pict_dict={}
    pict_array=[]
    for i in contours:
        rect= cv2.minAreaRect(i)
        h,w=rect[1][0],rect[1][1]
        print w,h
        if w<25 or h<25:
            continue
        box = cv2.cv.BoxPoints(rect)
        box = np.int0(box)

        area=w * h
        pict_array.append(area)
        pict_dict[area]=box
    if(len(pict_array)>0):
        box=pict_dict[max(pict_array)]

        #datainterface.SendQRCode([frame.shape[0],frame.shape[1]],box)

        cv2.drawContours(frame, [box], 0, (0, 0, 255), 2)

    cv2.imshow("",frame)
    cv2.waitKey(1)

# print hsv_green