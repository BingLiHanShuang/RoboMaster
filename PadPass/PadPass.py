import sys
sys.path.append('../Protocol/MainFold/Python')
import protocol
import PadRecognization
import PassRecognization
import cv2


raw_image = cv2.imread("7_62159_981436527.jpg")
res_pad= PadRecognization.recognize(raw_image)
res_pass= PassRecognization.recognize(raw_image)
cv2.imshow("test",raw_image)
key = cv2.waitKey(0)


# datainterface=protocol.Protocol('192.168.1.200',6000)
# cap=cv2.VideoCapture(0)
# success,frame=cap.read()
#
# while success:
#     success, frame = cap.read()
#
#     raw_image = cv2.imread("/Users/wzq/pic01/0_85561_345728169.jpg")
#     res_pad= PadRecognization.recognize(raw_image)
#     res_pass= PassRecognization.recognize(raw_image)
#
#     print res_pad,res_pass
#     cv2.imshow("test",frame)
#     key = cv2.waitKey(10)
#     #cv2.waitKey(0)
#     if res_pad ==None or res_pass==None:
#         continue
#     datainterface.SendPadPass(res_pad,res_pass)
