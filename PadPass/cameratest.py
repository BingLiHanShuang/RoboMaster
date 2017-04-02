import cv2

cap=cv2.VideoCapture("/Users/wzq/Downloads/wzq_1_1491099798.avi")
# cap.set(3, 640)
# cap.set(4, 480)
count=0
while True:
    success, frame = cap.read()
    count+=1
    #cv2.imwrite(str(count)+".jpg",frame)
    cv2.imshow("",frame)

    cv2.waitKey(0XFF)