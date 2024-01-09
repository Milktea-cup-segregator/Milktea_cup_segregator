import os
import time

import tensorflow as tf
import pathlib
from tensorflow import keras
from keras.preprocessing import image
from keras.utils import image_utils
import cv2
import numpy as np


def judge():
    path = "D:/milkTea/realPhoto/1.jpg"

    class_names = ['yes', 'no']


    img = image_utils.load_img(path, target_size=(180, 180))
    x = image_utils.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    model = keras.models.load_model('./model/model.h5')
    softmax = model.predict(x)
    y_label = tf.argmax(softmax, axis=1).numpy()[0]

    print(class_names[y_label])




cap = cv2.VideoCapture(0)
cap.set(3,180) # set Width
cap.set(4,180) # set Height
model = keras.models.load_model('./model/model.h5')
path = "D:/milkTea/realPhoto/1.jpg"
class_names = ['yes', 'no']

while True:
    ret, img = cap.read()
    # img = cv2.flip(img, -1)
    cv2.imshow('video', img)
    cv2.imwrite("D:/milkTea/realPhoto/1.jpg", img)
    img = image_utils.load_img(path, target_size=(180, 180))
    x = image_utils.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    k = cv2.waitKey(30) & 0xff
    softmax = model.predict(x)
    y_label = tf.argmax(softmax, axis=1).numpy()[0]
    print(class_names[y_label])
    time.sleep(1)
    if k == 27:  # press 'ESC' to quit
        break




