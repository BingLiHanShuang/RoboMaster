import cv2

cap=cv2.VideoCapture(1)
cap.set(3, 640)
cap.set(4, 480)

while True:
    success, frame = cap.read()
    cv2.imwrite("test.jpg",frame)
    cv2.imshow("",frame)

    cv2.waitKey(0)