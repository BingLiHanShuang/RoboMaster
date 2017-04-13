from __future__ import print_function

import keras
import numpy as np
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.optimizers import RMSprop
from keras.layers import Dense, Dropout, Flatten,Activation
from keras.layers import Conv2D, MaxPooling2D
import tensorflow as tf
from tensorflow.python.framework import graph_util
from keras import backend as K
K.set_image_dim_ordering('th')
# sess = tf.Session()

from keras import backend as K
# K.set_session(sess)
batch_size = 128
num_classes = 10
epochs = 2
img_rows=28
img_cols=28

# the data, shuffled and split between train and test sets
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# if K.image_data_format() == 'channels_first':
x_train = x_train.reshape(x_train.shape[0],1, img_rows, img_cols)
x_test = x_test.reshape(x_test.shape[0], 1, img_rows, img_cols)
input_shape = (1, img_rows, img_cols)
# else:
#     x_train = x_train.reshape(x_train.shape[0], img_rows, img_cols, 1)
#     x_test = x_test.reshape(x_test.shape[0], img_rows, img_cols, 1)
#     input_shape = (img_rows, img_cols, 1)
x_train = x_train.astype('float32')
x_test = x_test.astype('float32')
x_train /= 255
x_test /= 255
print(x_train.shape[0], 'train samples')
print(x_test.shape[0], 'test samples')

# convert class vectors to binary class matrices
temp=np.zeros([len(y_train),num_classes])
for i in range(len(y_train)):
    temp[i][y_train[i]]=1
y_train=temp

temp=np.zeros([len(y_test),num_classes])
for i in range(len(y_test)):
    temp[i][y_test[i]]=1
y_test=temp
# y_train = keras.utils.to_categorical(y_train, num_classes)
# y_test = keras.utils.to_categorical(y_test, num_classes)

model = Sequential()


# model.add(Flatten(input_shape=(28, 28, 1)))
# model.add(Dense(512))
# model.add(Activation('relu'))
# model.add(Dropout(0.2))
# model.add(Dense(512))
# model.add(Activation('relu'))
# model.add(Dropout(0.2))
# model.add(Dense(10))
# model.add(Activation('softmax'))




model.add(Conv2D(32, 5,5,
                 input_shape=(1,28,28),border_mode='valid'))
model.add(Activation('relu'))

model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Dropout(0.5))

model.add(Conv2D(64, 5, 5, border_mode='valid'))
model.add(Activation('relu'))

model.add(MaxPooling2D(pool_size=(2, 2),border_mode='valid'))
model.add(Dropout(0.5))
model.add(Flatten())
model.add(Dense(1024))
model.add(Activation('relu'))

model.add(Dense(num_classes))
model.add(Activation('softmax'))


model.summary()

model.compile(loss='categorical_crossentropy',
              optimizer=keras.optimizers.Adadelta(),
              metrics=['accuracy'])
# model.load_weights('model_handwrite.h5')
#
# graph_def = sess.graph.as_graph_def()
# tf.train.write_graph(graph_def, logdir='.', name='model.pb', as_text=False)
# saver = tf.train.Saver()
# saver.save(sess, 'model.ckpt')

# from kerasify import export_model
# export_model(model, 'model_handwrite.model')


history = model.fit(x_train, y_train,
                    batch_size=batch_size,
                    nb_epoch=epochs,
                    verbose=1,
                    validation_data=(x_test, y_test))
model_json = model.to_json()
with open("model_handwrite.json", "w") as json_file:
    json_file.write(model_json)
# serialize weights to HDF5
model.save_weights("model_handwrite.h5")
score = model.evaluate(x_test, y_test, verbose=0)
print('Test loss:', score[0])
print('Test accuracy:', score[1])