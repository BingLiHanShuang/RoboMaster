from keras.models import Sequential
from keras.layers import Input,merge,MaxPooling2D
from keras.layers.core import Dense, Dropout, Activation, Flatten,Merge
from keras.preprocessing import image
from keras.layers.convolutional import Convolution2D
# from keras.utils.visualize_util import plot
from keras.layers.pooling import MaxPooling2D
from keras.models import Model

import random

import cv2
import matplotlib.pyplot as plt
import numpy as np
def generate_block():
    blank_image = np.ones((480,640,3), np.uint8)
    blank_image[:,:]=(255,255,255)
    x=160
    y=20
    h=70
    w=50
    for i in range(5):
        cv2.rectangle(blank_image,(x+w*i,y),(x+w*(i+1),y+h),(0,0,0),2)
    x=0
    y=100
    h=115
    w=640
    for i in range(4):
        cv2.line(blank_image,(x,y+h*i),(x+w,y+h*i),(0,0,0),3)
    x=0
    y=100
    h=345
    w=220
    for i in range(1,3):
        cv2.line(blank_image,(x+w*i,y),(x+w*i,y+h),(0,0,0),3)
    return blank_image

def put_text(img,pad,password):
    font = cv2.FONT_HERSHEY_PLAIN
    text_password=reduce(lambda x,y:x+y,password)
    cv2.putText(img,text_password,(160,80), font, 5,(0,0,0),thickness=5)

    text_pad=[]
    offset=120
    for i in range(3):
        temp=reduce(lambda x,y:x+"  "+y,pad[i*3:i*3+3])
        cv2.putText(img,temp,(50,200+i*offset), font, 9,(0,0,0),thickness=7)
    return img
sample_image=generate_block()
sample_image=put_text(sample_image,"123456789","12345")
import string
characters = string.digits
print(characters)
width, height, n_len, n_class = sample_image.shape[1], sample_image.shape[0], 14, len(characters)+1

from keras import backend as K

def ctc_lambda_func(args):
    y_pred, labels, input_length, label_length = args
    y_pred = y_pred[:, 2:, :]
    return K.ctc_batch_cost(labels, y_pred, input_length, label_length)
from keras.models import *
from keras.layers import *
rnn_size = 128

input_tensor = Input((width, height, 1))
x = input_tensor
for i in range(3):
    x = Convolution2D(32, 3, 3, activation='relu')(x)
    x = Convolution2D(32, 3, 3, activation='relu')(x)
    x = MaxPooling2D(pool_size=(2, 2))(x)

conv_shape = x.get_shape()
x = Reshape(target_shape=(int(conv_shape[1]), int(conv_shape[2]*conv_shape[3])))(x)

x = Dense(32, activation='relu')(x)

gru_1 = GRU(rnn_size, return_sequences=True, init='he_normal', name='gru1')(x)
gru_1b = GRU(rnn_size, return_sequences=True, go_backwards=True, init='he_normal', name='gru1_b')(x)
gru1_merged = merge([gru_1, gru_1b], mode='sum')

gru_2 = GRU(rnn_size, return_sequences=True, init='he_normal', name='gru2')(gru1_merged)
gru_2b = GRU(rnn_size, return_sequences=True, go_backwards=True, init='he_normal', name='gru2_b')(gru1_merged)
x = merge([gru_2, gru_2b], mode='concat')
x = Dropout(0.25)(x)
x = Dense(n_class, init='he_normal', activation='softmax')(x)
base_model = Model(input=input_tensor, output=x)

labels = Input(name='the_labels', shape=[n_len], dtype='float32')
input_length = Input(name='input_length', shape=[1], dtype='int64')
label_length = Input(name='label_length', shape=[1], dtype='int64')
loss_out = Lambda(ctc_lambda_func, output_shape=(1,), name='ctc')([x, labels, input_length, label_length])

model = Model(input=[input_tensor, labels, input_length, label_length], output=[loss_out])
model.load_weights('model1.h5')
model.compile(loss={'ctc': lambda y_true, y_pred: y_pred}, optimizer='adadelta')
