import cv2
import os
import numpy as np
import keras
from sklearn.svm import LinearSVC
from sklearn.externals import joblib

from skimage.feature import hog

from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.preprocessing import image
from keras import backend as K
K.set_image_dim_ordering('th')

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
def loadled():
    path="/Users/wzq/Desktop/led/train"
    #dirs=os.listdir(path)
    label=[]
    image=[]
    for i in range(1,10):

        dir_image=os.listdir(path+"/"+str(i))
        for j in dir_image:
            if j in [".DS_Store"]:
                continue
            gray=cv2.imread(path+"/"+str(i)+"/"+j,cv2.IMREAD_GRAYSCALE)
            bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 25, 25)
            bw=resize(bw)
            # cv2.imshow("",bw)
            # cv2.waitKey(0)
            # gray = cv2.resize(img, (28, 28), interpolation=cv2.INTER_AREA)
            # gray = gray[:, :, 0]
            # fd = hog(bw.reshape((28, 28)), orientations=9, pixels_per_cell=(1, 1), cells_per_block=(1, 1),
            #          visualise=False)
            label.append(i)
            image.append(bw)
    return image,label

list_hog_fd,label=loadled()

temp=np.zeros([len(label),10])

for i,j in enumerate(label):
    temp[i][j]=1

label=temp


img_rows=28
img_cols=28
# hog_features = np.array(list_hog_fd, 'float64')
# clf = LinearSVC()
# clf.fit(list_hog_fd, label)
# # clf.fit(list_hog_fd, label)
#
# joblib.dump(clf, "led_cls.pkl", compress=3)
model = Sequential()
model.add(Flatten(input_shape=(1,28, 28)))
model.add(Dense(512))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(Dense(512))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(Dense(10))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='adadelta',
              metrics=['accuracy'])

model.summary()

x_merge=list_hog_fd
x_merge=np.array(x_merge)
x_merge = x_merge.reshape(x_merge.shape[0],1, img_rows, img_cols)

#x_merge=np.resize(x_merge,(1,28,28))
y_merge=label
epochs=2000
batch_size=20
history = model.fit(x_merge, y_merge,
                    batch_size=batch_size,
                    nb_epoch=epochs,
                    verbose=1)
model_json = model.to_json()

with open("model_led.json", "w") as json_file:
    json_file.write(model_json)
# serialize weights to HDF5
model.save_weights("model_led.h5")