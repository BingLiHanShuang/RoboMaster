import cv2

cap=cv2.VideoCapture(1)
# cap.set(3, 640)
# cap.set(4, 480)
count=0
while True:
    success, frame = cap.read()
    count+=1
    cv2.imwrite(str(count)+".jpg",frame)
    cv2.imshow("",frame)

    cv2.waitKey(0XFF)