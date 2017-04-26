from sklearn.externals import joblib
from skimage.feature import hog
import numpy as np
import cv2

def resize(rawimg):  # resize img to 28*28
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

clf = joblib.load("led_cls.pkl")
gray = cv2.imread("/Users/wzq/Desktop/led/train/1/0.jpg",cv2.IMREAD_GRAYSCALE)
bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 25, 25)
bw = resize(bw)

roi_hog_fd = hog(bw, orientations=9, pixels_per_cell=(1, 1), cells_per_block=(1, 1), visualise=False)
nbr = clf.predict(np.array([roi_hog_fd], 'float64'))
print nbr
cv2.imshow("",bw)
cv2.waitKey(0)