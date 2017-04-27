import cv2

# cap=cv2.VideoCapture("/Users/wzq/Downloads/wzq_1_1491099798.avi")

count=1200
cap=cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)
def resize(rawimg):  # resize img to 28*28
    fx = 640.0 / rawimg.shape[0]
    fy = 480.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    return img
while True:
    success, frame = cap.read()
    #frame=resize(frame)
    count+=1
    cv2.imwrite("test3/"+str(count)+".jpg",frame)
    cv2.imshow("",frame)

    cv2.waitKey(0XFF)