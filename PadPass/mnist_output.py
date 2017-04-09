import os
from keras.datasets import mnist
import cv2
(x_train, y_train), (x_test, y_test) = mnist.load_data()

dir="/Users/wzq/Downloads/mnist/"
for i in range(0,10):
    if not os.path.exists(dir+str(i)):
        os.mkdir(dir+str(i))
count=[0]*10

for i,j in zip(x_train,y_train):
    i=255-i
    count[j]+=1
    cv2.imwrite(dir+str(j)+"/"+str(count[j])+".jpg",i)

for i,j in zip(x_test,y_test):
    i=255-i
    count[j]+=1
    cv2.imwrite(dir+str(j)+"/"+str(count[j])+".jpg",i)

print 1
# for i in range(len(x_train)):

