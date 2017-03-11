import cv2
import os

count=0

def split_img(raw_image):

    gray = cv2.cvtColor(raw_image, cv2.COLOR_BGR2GRAY)
    bw = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)

    edge = cv2.Canny(bw.copy(), 500, 2000, apertureSize=5)
    contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    temp_x=[]
    global count
    for i in contours:
        x, y, w, h = cv2.boundingRect(i)
        if w > 30 or h > 40 or h < 25 or w <15:  # give up useless part
            continue
        if x in temp_x:
            continue
        for j in range(x-5,x+5,1):
            temp_x.append(j)
        roi = raw_image[y:y + h, x:x + w]
        cv2.imwrite("/Users/wzq/Desktop/digit_split/"+str(count)+".png",roi)
        count+=1
path="/Users/wzq/Downloads/testcase1/"
for filename in os.listdir(path):
    try:
        raw_image = cv2.imread(path+filename)
        split_img(raw_image)
    except Exception,e:
        continue


