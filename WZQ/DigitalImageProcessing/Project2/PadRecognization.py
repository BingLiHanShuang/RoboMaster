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

raw_image = cv2.imread("/Users/wzq/Desktop/testcase/h201703032044/73_38256928415637.jpg")

gray = cv2.imread("/Users/wzq/Desktop/testcase/h201703032044/73_38256928415637.jpg", cv2.IMREAD_GRAYSCALE)
bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)

edge = cv2.Canny(bw.copy(), 500, 2000, apertureSize=5)
contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for i in contours:
    x, y, w, h = cv2.boundingRect(i)
    if w > 50 or h > 85 or h < 40:  # give up useless part
        continue
    roi = gray[y:y + h, x:x + w]
    res = resize(roi)
    res = np.resize(res, (1, 28, 28, 1))

    predictions = model.predict(res)
    predictions = np.argmax(predictions)
    if predictions != 10:
        cv2.putText(raw_image, '{:.0f}'.format(predictions), (x, y), cv2.FONT_HERSHEY_DUPLEX, h / 25.0, (255, 0, 0))
    cv2.rectangle(raw_image, (x, y), (x + w, y + h), (0, 255, 0), 2)

cv2.imshow("test",raw_image)
cv2.imshow("test1",edge)

cv2.waitKey(0)