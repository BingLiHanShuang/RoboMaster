from sklearn.externals import joblib
from skimage.feature import hog
import numpy as np
import cv2
clf = joblib.load("led_cls.pkl")
im = cv2.imread("/Users/wzq/Desktop/led/train/6/0.jpg")
roi = cv2.resize(im, (28, 28), interpolation=cv2.INTER_AREA)
gray = roi[:,:,0]

roi_hog_fd = hog(gray, orientations=9, pixels_per_cell=(1, 1), cells_per_block=(1, 1), visualise=False)
nbr = clf.predict(np.array([roi_hog_fd], 'float64'))
print nbr
cv2.imshow("",roi)
cv2.waitKey(0)