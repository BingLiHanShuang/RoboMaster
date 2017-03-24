
from keras.models import model_from_json
import string

import random
import datetime
import cv2
import matplotlib.pyplot as plt
import numpy as np

with open('model_cnn.json', 'r') as f:
    model = model_from_json(f.read())
model.load_weights('model_cnn.h5')
model.summary()

def resize(rawimg):  # resize img to 28*28
    fx = 640.0 / rawimg.shape[0]
    fy = 480.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    w = img.shape[1]
    h = img.shape[0]
    x = (640 - w) / 2
    y = (480 - h) / 2
    #outimg[y:y+h, x:x+w] = img
    return img


img=cv2.imread("/Users/wzq/pic01/2158_17944_326574189.jpg",0)
img=resize(img)

width, height = img.shape[1], img.shape[0]
img=np.reshape(img, img.shape + (1,))
X = np.zeros((1, width, height, 3), dtype=np.uint8)
X[0]=img.transpose(1, 0, 2)

# test_x,test_y=train_generator.next()
begin = datetime.datetime.now()

res=model.predict(X)
print "predict",[list(i[0]).index(max(i[0])) for i in list(res)]
end = datetime.datetime.now()
print "time",(str(end - begin))
#print "real   ",[list(i[0]).index(max(i[0])) for i in list(test_y)]
cv2.imshow("",img)
cv2.waitKey(0)