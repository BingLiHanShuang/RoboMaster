#coding=utf-8
import cv2
import numpy as np
from keras.models import model_from_json

with open('model.json', 'r') as f:
    model = model_from_json(f.read())
model.load_weights('model.h5')
model.summary()

def resize(rawimg):  # resize img to 28*28
    fx = 28.0 / rawimg.shape[0]
    fy = 28.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    outimg = np.ones((28, 28), dtype=np.uint8) * 255
    w = img.shape[1]
    h = img.shape[0]
    x = (28 - w) / 2
    y = (28 - h) / 2
    outimg[y:y+h, x:x+w] = img
    return outimg

boundraries=[([86, 31, 4], [220, 88, 50])]
#121，49，51


raw_image = cv2.imread("/Users/wzq/Desktop/testcase/h201703032044/8_78263219874365.jpg")
hsv = cv2.cvtColor(raw_image, cv2.COLOR_BGR2HSV)
lower_red = np.array([0,90,50])
upper_red = np.array([220,200,120])

mask = cv2.inRange(hsv, lower_red, upper_red)

kernel = np.ones((3,3),np.uint8)


enrode = cv2.erode(mask,kernel,iterations = 1)

dilate = cv2.dilate(enrode,kernel,iterations = 1)
res = cv2.bitwise_and(raw_image, raw_image, mask=dilate)


gray = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)

edge = cv2.Canny(bw.copy(), 0, 2000, apertureSize=7)
contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)



for i in contours:
    x, y, w, h = cv2.boundingRect(i)
    if w > 50 or h > 85 or h < 40:  # give up useless part
        pass
    roi = gray[y:y + h, x:x + w]
    res1 = resize(roi)
    res1 = np.resize(res1, (1, 28, 28, 1))


    cv2.rectangle(res, (x, y), (x + w, y + h), (0, 255, 0), 2)

#cv2.imshow('frame', raw_image)
#cv2.imshow('mask', mask)
cv2.imshow('res', res)
cv2.imshow('gray', gray)
cv2.imshow('edge', edge)
cv2.imshow("bw",bw)
#cv2.imshow('enrode', enrode)


cv2.waitKey(0)