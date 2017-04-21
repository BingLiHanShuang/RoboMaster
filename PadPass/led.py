import cv2
import numpy as np
from imutils.perspective import four_point_transform
from imutils import contours
import imutils
from keras import backend as K
#K.set_image_dim_ordering('th')
from keras.models import model_from_json


with open('model_digit.json', 'r') as f:
    model = model_from_json(f.read())
model.load_weights('model_digit.h5')
model.summary()
# each digit on the thermostat
DIGITS_LOOKUP = {
	(1, 1, 1, 0, 1, 1, 1): 0,
	(0, 0, 1, 0, 0, 1, 0): 1,
	(1, 0, 1, 1, 1, 1, 0): 2,
	(1, 0, 1, 1, 0, 1, 1): 3,
	(0, 1, 1, 1, 0, 1, 0): 4,
	(1, 1, 0, 1, 0, 1, 1): 5,
	(1, 1, 0, 1, 1, 1, 1): 6,
	(1, 0, 1, 0, 0, 1, 0): 7,
	(1, 1, 1, 1, 1, 1, 1): 8,
	(1, 1, 1, 1, 0, 1, 1): 9
}
def resize1(rawimg):  # resize img to 28*28
    fx = 28.0 / rawimg.shape[0]
    fy = 28.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    outimg = np.zeros((28, 28), dtype=np.uint8) * 255
    w = img.shape[1]
    h = img.shape[0]
    x = (28 - w) / 2
    y = (28 - h) / 2
    outimg[y:y+h, x:x+w] = img
    # cv2.imshow("out",outimg)
    # cv2.waitKey(0)
    return outimg


def recognize1(img):
    gray=img

    #gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    res = resize1(gray)
    # res = np.resize(res, (1,1,28,28))
    res = np.resize(res, (1,28,28,1))

    predictions = model.predict(res)
    result = np.argmax(predictions)
    print result
    return result
frame=cv2.imread("/Users/wzq/Desktop/Untitled 2.png")
frame = cv2.GaussianBlur(frame, (5, 5), 1)
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
lower_white = np.array([0, 0, 240])
upper_white = np.array([255, 255, 255])
kernel = np.ones((3,3), np.uint8)

mask = cv2.inRange(hsv, lower_white, upper_white)
mask = cv2.dilate(mask, kernel, iterations=1)
digitCnts = []
contours1, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for cnt in contours1:
    x1, y1, w1, h1 = cv2.boundingRect(cnt)
    #print x1, y1, w1, h1

    if(h1<20):
        continue
    digitCnts.append(cnt)
    cv2.rectangle(frame, (x1, y1), (x1 + w1, y1 + h1), (0, 0, 255), 2)
digitCnts = contours.sort_contours(digitCnts,
	method="left-to-right")[0]
digits = []
count=0
# loop over each of the digits
for c in digitCnts:
    # extract the digit ROI
    (x, y, w, h) = cv2.boundingRect(c)
    roi = mask[y:y + h, x:x + w]

    # compute the width and height of each of the 7 segments
    # we are going to examine
    (roiH, roiW) = roi.shape
    (dW, dH) = (int(roiW * 0.25), int(roiH * 0.15))
    dHC = int(roiH * 0.05)

    # define the set of 7 segments
    segments = [
        ((0, 0), (w, dH)),  # top
        ((0, 0), (dW, h // 2)),  # top-left
        ((w - dW, 0), (w, h // 2)),  # top-right
        ((0, (h // 2) - dHC), (w, (h // 2) + dHC)),  # center
        ((0, h // 2), (dW, h)),  # bottom-left
        ((w - dW, h // 2), (w, h)),  # bottom-right
        ((0, h - dH), (w, h))  # bottom
    ]
    on = [0] * len(segments)

    roi1 = frame[y:y + h, x:x + w]
    recognize1(roi)
    # loop over the segments
    # for (i, ((xA, yA), (xB, yB))) in enumerate(segments):
    #     # extract the segment ROI, count the total number of
    #     # thresholded pixels in the segment, and then compute
    #     # the area of the segment
    #     segROI = roi[yA:yB, xA:xB]
    #     total = cv2.countNonZero(segROI)
    #     area = (xB - xA) * (yB - yA)
    #
    #
    #     # if the total number of non-zero pixels is greater than
    #     # 50% of the area, mark the segment as "on"
    #     if total / float(area) > 0.5:
    #         #cv2.rectangle(roi1, segments[i][0], segments[i][1], (0, 255, 0), 2)
    #
    #         on[i] = 1
    # cv2.imshow("rot"+str(count),roi)
    # count+=1
    # lookup the digit and draw it on the image
    # digit = DIGITS_LOOKUP[tuple(on)]
    # digits.append(digit)
    # cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 1)
    # cv2.putText(frame, str(digit), (x - 10, y - 10),
    #             cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
cv2.imwrite("test.jpg",mask)
cv2.imshow("mask",mask)
cv2.imshow("frame",frame)

cv2.waitKey(0)