import cv2
import imutils
import numpy as np
import tarfile
import shutil
import os
from tqdm import tqdm
def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)
def rmdir(path):
    if os.path.exists(path):
        shutil.rmtree(path)


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
    outimg[y:y + h, x:x + w] = img
    return outimg


def convert(img):
    # img = cv2.imread(imgpath)
    # gray = cv2.imread(imgpath, cv2.IMREAD_GRAYSCALE)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)
    img2, ctrs, hier = cv2.findContours(bw.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    rects = [cv2.boundingRect(ctr) for ctr in ctrs]
    x, y, w, h = rects[-1]
    roi = gray[y:y + h, x:x + w]
    return resize(roi)


def rotate(img, angle):
    rotated = imutils.rotate(image, angle)
    return convert(rotated)


rmdir('train')

for i in range(11):
    path = '/Users/wzq/Downloads/English/Fnt/Sample%03d/' % (i + 1)
    trainpath = '/Users/wzq/Downloads/Digit/train/%d/' % i
    mkdir(trainpath)
    j = 0
    for filename in tqdm(os.listdir(path), desc=trainpath):
        try:
            img = cv2.imread(path + filename)

            for angle in np.arange(-20, 21, 1):
                j += 1
                # print img.shape
                print("img%03d-%05d.png" % {i, j})
                cv2.imwrite(path + "img%03d-%05d.png" % {i, j}, rotate(img, angle))

        except:
            pass    #cv2.waitKey(0)