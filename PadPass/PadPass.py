import sys
sys.path.append('../Protocol/MainFold/Python')
import protocol
import PadRecognization
import PassRecognization
import cv2

datainterface=protocol.Protocol('192.168.1.200',6000)
cap=cv2.VideoCapture(1)
success,frame=cap.read()

while success:
    success, frame = cap.read()

    #raw_image = cv2.imread("6_18444_239156784.jpg")
    res_pad= PadRecognization.recognize(frame)
    res_pass= PassRecognization.recognize(frame)

    print res_pad,res_pass
    cv2.imshow("test",frame)
    key = cv2.waitKey(10)
    #cv2.waitKey(0)
    if res_pad ==None or res_pass==None:
        continue
    datainterface.SendPadPass(res_pad,res_pass)
