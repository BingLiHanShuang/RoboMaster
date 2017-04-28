import cv2
import numpy as np
from sklearn.externals import joblib
# from skimage.feature import hog
import datetime
from keras import backend as K
K.set_image_dim_ordering('th')
from keras.models import model_from_json


with open('/Users/wzq/model_handwrite.json', 'r') as f:
    model = model_from_json(f.read())
model.load_weights('/Users/wzq/model_handwrite.h5')
model.summary()

with open('model_led.json', 'r') as f:
    model_led = model_from_json(f.read())
model_led.load_weights('model_led.h5')
model_led.summary()


clf = joblib.load("digits_cls1.pkl")
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
def recognize(image):
    roi = cv2.resize(image, (28, 28), interpolation=cv2.INTER_AREA)
    #gray = roi[:, :, 0]
    #roi = cv2.resize(roi, (28, 28), interpolation=cv2.INTER_AREA)
    roi_hog_fd = hog(roi, orientations=9, pixels_per_cell=(14, 14), cells_per_block=(1, 1), visualise=False)
    nbr = clf.predict(np.array([roi_hog_fd], 'float64'))
    return nbr
def resize(rawimg):  # resize img to 28*28
    fx = 640.0 / rawimg.shape[0]
    fy = 480.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    return img
green = np.uint8([[[110,140,130 ]]])
hsv_green = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)

count=15

def process(frame):
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
    # cv2.imshow("frame", frame)
    # cv2.imshow("edge", edge)

    #cv2.waitKey(0)
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
    result = []

    def slice_first_line():
        arg_line=0
        x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
        y=(pos_rect_left[arg_line][1]+pos_rect_right[arg_line][1])/2-5
        length=0
        height=0
        for i in range(2):
            length+=(pos_rect_right[i][0]-pos_rect_left[i][0]-pos_rect_left[i][2])
        length=(length)/2
        length = (length) /3

        height+=pos_rect_left[1][1]-y+0.77*pos_rect_left[1][3]
        height+=pos_rect_right[1][1]-y+0.77*pos_rect_right[1][3]
        height=height/2
        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        for i in range(3):
            result.append(frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])
            #cv2.rectangle(frame, (x+ int(length)*i, y), (x + int(length)*(i+1), y + int(height)), (0, 255, 0), 2)

    def slice_second_line():
        arg_line=1
        x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
        y=(pos_rect_left[arg_line][1]+pos_rect_right[arg_line][1]+0.75*pos_rect_left[arg_line][3]+0.75*pos_rect_right[arg_line][3])/2
        length=0
        height=0
        for i in range(1,3):
            length+=(pos_rect_right[i][0]-pos_rect_left[i][0]-pos_rect_left[i][2])
        length=(length)/2
        length = (length) /3

        height+=pos_rect_left[3][1]-y+0.29*pos_rect_left[3][3]
        height+=pos_rect_right[3][1]-y+0.29*pos_rect_right[3][3]
        height=height/2
        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        for i in range(3):
            result.append(frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])
            #cv2.imshow(str(i)+"slice",frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])

            #cv2.rectangle(frame, (x+ int(length)*i, int(y)), (x + int(length)*(i+1), int(y) + int(height)), (0, 255, 0), 2)

    def slice_third_line():
        arg_line=3
        x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
        y=(pos_rect_left[arg_line][1]+pos_rect_right[arg_line][1]+0.25*pos_rect_left[arg_line][3]+0.25*pos_rect_right[arg_line][3])/2
        length=0
        height=0
        for i in range(3,5):
            length+=(pos_rect_right[i][0]-pos_rect_left[i][0]-pos_rect_left[i][2])
        length=(length)/2
        length = (length) /3

        height+=pos_rect_left[4][1]-y+1.5*pos_rect_left[4][3]
        height+=pos_rect_right[4][1]-y+1.5*pos_rect_right[4][3]
        height=height/2

        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        for i in range(3):
            result.append(frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])

            #cv2.rectangle(frame, (x+ length*i, y), (x + length*(i+1), y+ height), (0, 255, 0), 2)
        return result

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

        cv2.imshow("ledscreen", led_screen)

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
    slice_first_line()
    slice_second_line()
    slice_third_line()
    return




    digit_pad=[]

    for i in range(9):
        edge = cv2.Canny(result[i], 500, 2000, apertureSize=5)
        contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        copy = result[i].copy()
        #cv2.imshow("origin",copy)

        im_gray = cv2.cvtColor(copy, cv2.COLOR_BGR2GRAY)
        #cv2.adaptiveThreshold()
        #ret, im_th = cv2.threshold(im_gray, 180  , 255, cv2.THRESH_BINARY_INV)

        im_th = cv2.adaptiveThreshold(im_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 25, 25)
#        cv2.adaptiveThreshold()

        # cv2.imshow("im_gray", im_gray)
        # cv2.imshow("im_th", im_th)
        #
        # cv2.waitKey(0)

        temp_dict={}
        max_index=-1
        for i in contours:
            x2, y2, w2, h2 = cv2.boundingRect(i)
            area=w2*h2

            if w2<4 or h2<5 or copy.shape[1]-w2<3:
                continue
            #print w2, h2, area
            # if x2<0.15*w2 or x2>0.85*w2:
            #     continue
            # if(w>0.8*w):
            #     continue
            max_index=max([max_index,area])
            img_temp=im_th[y2+3:y2 + h2-2, x2+2:x2 + w2-3].copy()
            temp_dict[area]=img_temp
            #cv2.rectangle(im_gray, (x2, y2), (x2 + w2, y2 + h2), (0, 0, 255), 2)
            #cv2.imshow(str(count),im_th )
        if max_index==-1:
            cv2.imshow("wzq",im_th)
            print -1
            continue
        digit_pad.append(temp_dict[max_index].copy())

        #name= recognize1(temp_dict[max_index])
        # print name
        # #cv2.imwrite("test/"+str(count)+".jpg",temp_dict[max_index])
        #     # res=recognize(copy[y2-1:y2 + h2+1, x2-1:x2 + w2+1].copy())
        #     # #cv2.rectangle(copy, (x2, y2), (x2 + w2, y2 + h2), (0, 0, 255), 2)
        count += 1
        cv2.imshow(str(count), temp_dict[max_index].copy())

        #cv2.imshow("im_gray",im_gray)
    #     #cv2.imshow("max_rect"+str(count), temp_dict[max_index].copy())
    for i in range(len(digit_pad)):
#        cv2.dilate(mask, kernel, iterations=1)
        contours, hierarchy = cv2.findContours(digit_pad[i].copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        temp_dict={}
        max_index=-1
        for j in contours:
            x2, y2, w2, h2 = cv2.boundingRect(j)
            area=w2*h2

            if w2<2 or h2<2:
                continue
            #print w2, h2, area
            # if x2<0.15*w2 or x2>0.85*w2:
            #     continue
            # if(w>0.8*w):
            #     continue
            max_index=max([max_index,area])
            if y2-2>0:
                y2-=1
            img_temp=digit_pad[i][y2-1:y2 + h2+1, x2-1:x2 + w2+1].copy()
            temp_dict[area]=img_temp
        name = recognize1(temp_dict[max_index])
        temp_resize=resize1(temp_dict[max_index])
        cv2.imshow(str(i) + "-" + str(name), temp_resize)





    end = datetime.datetime.now()
    print end-begin
    #cv2.waitKey(0)

    #cv2.imshow("RotImg",RotImg)

    cv2.imshow("frame",frame)
    #cv2.imshow("mask",mask)
    cv2.waitKey(0)
count=0

cap=cv2.VideoCapture(1)
cap.set(3, 640)
cap.set(4, 480)
while True:

    #success, frame = cap.read()
# error=[123,125,166,188,195,196,176,58]
    frame = cv2.imread("/Users/wzq/RoboMaster/PadPass/test3/1290.jpg")
    begin = datetime.datetime.now()

# end = datetime.datetime.now()
# print end-begin
    process(frame)
    cv2.waitKey(0)
# for i in range(2,220):
#     # if i in error:
#     #     continue
#     frame=cv2.imread("/Users/wzq/Downloads/pic_padpass/"+str(i)+".jpg")
#     process(frame)