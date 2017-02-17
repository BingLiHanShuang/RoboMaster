# coding: utf-8
import datetime
import time
import cv2
import numpy as np
from keras.models import model_from_json

# load model
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


def convert(imgpath):   # read digits
    img = cv2.imread(imgpath)
    gray = cv2.imread(imgpath, cv2.IMREAD_GRAYSCALE)
    bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)
    ctrs, hier = cv2.findContours(bw.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    rects = [cv2.boundingRect(ctr) for ctr in ctrs]
    begin = datetime.datetime.now()
    for rect in rects:
        x, y, w, h = rect
        roi = gray[y:y+h, x:x+w]
        hw = float(h) / w
        if (w < 200) & (h < 200) & (h > 10) & (w > 10) & (1.1 < hw) & (hw < 5):
            res = resize(roi)
            res = np.resize(res, (1, 28, 28, 1))

            predictions = model.predict(res)
            predictions = np.argmax(predictions)
            if predictions != 10:
                cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 1)
                cv2.putText(img, '{:.0f}'.format(predictions), (x, y), cv2.FONT_HERSHEY_DUPLEX, h/25.0, (255, 0, 0))
    thumb = cv2.resize(img,None,fx=0.5, fy=0.5, interpolation = cv2.INTER_CUBIC)
    #image2=cv2.resize(img,thumb)
    end = datetime.datetime.now()
    print(str(end-begin))
    cv2.imshow("res",thumb)
    cv2.waitKey(0)
    return img

convert("/home/ubuntu/Desktop/wechat_digit_recognition/QQ20170218-003357@2x.png")

