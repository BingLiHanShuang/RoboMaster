import cv2
import numpy as np
import datetime
from keras import backend as K
K.set_image_dim_ordering('th')
from keras.models import model_from_json


with open('model_handwrite.json', 'r') as f:
    model = model_from_json(f.read())
model.load_weights('model_handwrite.h5')
model.summary()

with open('model_led.json', 'r') as f:
    model_led = model_from_json(f.read())
model_led.load_weights('model_led.h5')
model_led.summary()


def resize1(rawimg):  # resize img to 28*28
    fx = 25.0 / rawimg.shape[0]
    fy = 25.0 / rawimg.shape[1]
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


def recognize1(img):
    gray=img
    #gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    res = resize1(gray)
    img1=res
    res = np.resize(res, (1,1,28,28))
    predictions = model.predict(res)
    result = np.argmax(predictions)
    #cv2.imshow(str(count) + "-" + str(result), img1)
    # print result,predictions
    return result
def recognize_led(img):
    gray=img
    #gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    res = resize1(gray)
    res = np.resize(res, (1,1,28,28))
    predictions = model_led.predict(res)
    result = np.argmax(predictions)
    # print result,predictions
    return result

def resize(rawimg):  # resize img to 28*28
    fx = 640.0 / rawimg.shape[0]
    fy = 480.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    return img
count=0

def process(frame,index):
    global count
    #frame=cv2.imread("/Users/wzq/Downloads/pic_padpass/200.jpg")
    begin = datetime.datetime.now()

    frame=resize(frame)
    frame = cv2.GaussianBlur(frame, (5, 5), 1)

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # bw = cv2.adaptiveThreshold(gray, 200, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 25, 25)
    edge = cv2.Canny(gray, 500, 1500, apertureSize=5)
    im_th = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)
    #cv2.imshow("im_th",im_th)
    # hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # lower_white = np.array([0, 0, 100])
    # upper_white = np.array([255, 255, 255])
    # mask = cv2.inRange(hsv, lower_white, upper_white)

    contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # cv2.imshow("edge",edge)
    # cv2.waitKey(0)
    pos_rect=[]
    for cnt in contours:
        approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
        if len(approx) >=4 or len(approx) <=10:
            x, y, w, h = cv2.boundingRect(cnt)
            #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
            if  w*h<350 or w*h>560  :
                continue
            if w/float(h)<=1.5 or w/float(h)>=2.5:
                continue
           # mask_rect = np.zeros((mask.shape[0], mask.shape[1]), np.uint8)
#            cv2.rectangle(mask_rect,(x, y), (x + w, y + h),255,-1)
            x1, y1, w1, h1 = cv2.boundingRect(cnt)
            mask_rect_color= gray[y1:y1+h1, x1:x1+w1].mean()
            if(mask_rect_color>100 or mask_rect_color<80):
                continue
            print w, h, w * h,w/float(h), mask_rect_color

            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            pos_rect.append((x,y,w,h))
    # if(len(pos_rect)!=10):
    #     return
    #pos_rect=list(set(pos_rect))
    cv2.imshow("frame", frame)
    # cv2.imshow("edge", edge)

    cv2.waitKey(0)
    pos_rect.sort(key=lambda x:(x[0],x[1]))
    pos_rect_new=[]
    pos_rect_new.append(pos_rect[0])
    for i in range(len(pos_rect)-1):
        distance=abs(pos_rect[i][0]-pos_rect[i+1][0])+abs(pos_rect[i][1]-pos_rect[i+1][1])
        if distance <10:
            continue
        pos_rect_new.append(pos_rect[i+1])
    pos_rect=pos_rect_new


    pos_rect_left=pos_rect[0:5]
    pos_rect_left.sort(key=lambda x:(x[1]))
    pos_rect_right=pos_rect[5:10]
    pos_rect_right.sort(key=lambda x:(x[1]))


    def slice_led():
        arg_line=0
        x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
        y=(pos_rect_left[arg_line][1]+pos_rect_right[arg_line][1])/2
        length=0
        height=(pos_rect_left[arg_line][3]+pos_rect_right[arg_line][3])/2
        for i in range(2):
            length+=(pos_rect_right[i][0]-pos_rect_left[i][0]-pos_rect_left[i][2])
        length=(length)/2

        led_x=(270/950.0)*length+x
        led_width=(415/950.0)*length

        led_height = (155 / 57.0) * height
        led_y = y - led_height*1.2

        led_y=int(led_y)
        led_x=int(led_x)
        led_width=int(led_width)
        led_height=int(led_height)
        led_screen=frame.copy()[led_y:led_y + led_height, led_x:led_x + led_width]
        global count

        #cv2.imshow("ledscreen", led_screen)
        cv2.imwrite("/Users/wzq/Downloads/led_screen/"+str(index)+".jpg",led_screen)
        count+=1
        return

        hsv = cv2.cvtColor(led_screen, cv2.COLOR_BGR2HSV)
        lower_white = np.array([0, 0, 210])
        upper_white = np.array([255, 255, 255])
        kernel = np.ones((3, 3), np.uint8)

        mask = cv2.inRange(hsv, lower_white, upper_white)

        mask = cv2.dilate(mask, kernel, iterations=1)

        digitCnts = []
        contours1, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        for i,cnt in enumerate(contours1):
            x1, y1, w1, h1 = cv2.boundingRect(cnt)
            # print x1, y1, w1, h1

            if (h1 < 10):
                continue
            digitCnts.append(cnt)
            res=recognize_led(mask[y1:y1+h1, x1:x1+w1])
            cv2.imshow("led"+str(i)+"-"+str(res),mask[y1:y1+h1, x1:x1+w1])


            #cv2.rectangle(frame, (x1, y1), (x1 + w1, y1 + h1), (0, 0, 255), 2)
        #digitCnts = contours.sort_contours(digitCnts,method="left-to-right")[0]
        #for i,j in enumerate(digitCnts):




        #cv2.imshow("led_screen",mask)
        #cv2.waitKey(0)

    slice_led()
    return

    #cv2.waitKey(0)

    #cv2.imshow("RotImg",RotImg)

    cv2.imshow("frame",frame)
    #cv2.imshow("mask",mask)
    cv2.waitKey(0)
count=0

# cap=cv2.VideoCapture(1)
# cap.set(3, 640)
# cap.set(4, 480)
frame = cv2.imread("/Users/wzq/RoboMaster/PadPass/test3/1221.jpg")
process(frame,1221)
#success, frame = cap.read()
# error=[123,125,166,188,195,196,176,58]
# for i in range(1220,1538):
#     frame = cv2.imread("/Users/wzq/RoboMaster/PadPass/test3/"+str(i)+".jpg")
#     process(frame,i)
    # begin = datetime.datetime.now()

# end = datetime.datetime.now()
# print end-begin

    # cv2.waitKey(0)
# for i in range(2,220):
#     # if i in error:
#     #     continue
#     frame=cv2.imread("/Users/wzq/Downloads/pic_padpass/"+str(i)+".jpg")
#     process(frame)