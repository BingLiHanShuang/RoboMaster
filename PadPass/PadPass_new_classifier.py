from sklearn.externals import joblib
from sklearn import datasets
from skimage.feature import hog
from sklearn.svm import LinearSVC
import os
import numpy as np
import urllib2
import socks
import socket
import cv2
# socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, "127.0.0.1", 1080)
# socket.socket = socks.socksocket
import tensorflow.examples.tutorials.mnist.input_data as input_data
m=input_data.read_data_sets("MNIST")
list_hog_fd = []
for i in m.train.images:
    cv2.imshow("1",i.reshape((28, 28)))
    cv2.waitKey(0)
    continue
    fd = hog(i.reshape((28, 28)), orientations=9, pixels_per_cell=(14, 14), cells_per_block=(1, 1),
             visualise=False)
    list_hog_fd.append(fd)
hog_features = np.array(list_hog_fd, 'float64')
clf = LinearSVC()
clf.fit(hog_features, m.train.labels)
joblib.dump(clf, "digits_cls1.pkl", compress=3)
# print 1

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

# m=input_data.read_data_sets("MNIST")
# list_hog_fd = []
# label=[]
# for i in range(1,10):
#     path1="/Users/wzq/Downloads/test 2/"+str(i)
#     for j in os.listdir(path1):
#         if j in [".DS_Store"]:
#             continue
#         image=cv2.imread(path1+"/"+j)
#         gray = cv2.resize(image, (28, 28), interpolation=cv2.INTER_AREA)
#         gray=gray[:,:,0]
#         fd = hog(gray.reshape((28, 28)), orientations=9, pixels_per_cell=(14, 14), cells_per_block=(1, 1),
#                  visualise=False)
#         list_hog_fd.append(fd)
#         label.append(i)
# hog_features = np.array(list_hog_fd, 'float64')
# clf = LinearSVC()
# clf.fit(hog_features, label)
# joblib.dump(clf, "digits_cls.pkl", compress=3)
# print 1