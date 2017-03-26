import cv2
import numpy as np
import datetime
def resize(rawimg):  # resize img to 28*28
    fx = 640.0 / rawimg.shape[0]
    fy = 480.0 / rawimg.shape[1]
    fx = fy = min(fx, fy)
    img = cv2.resize(rawimg, None, fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)
    w = img.shape[1]
    h = img.shape[0]
    x = (640 - w) / 2
    y = (480 - h) / 2
    #outimg[y:y+h, x:x+w] = img
    return img
def cmp_w(x, y):
    # print x.result,y.result,x.y - y.y
    delta = (x[2] - y[2])
    #print delta
    if delta > 40:
        return -1
    return 1
def cmp_h(x, y):
    # print x.result,y.result,x.y - y.y
    delta = (x[3] - y[3])
    #print delta
    if delta>0:
        return 1
    return 0

green = np.uint8([[[110,140,130 ]]])

hsv_green = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)
print hsv_green


frame=cv2.imread("/Users/wzq/Downloads/MobileFile/IMG_20170317_205659.jpg")
begin = datetime.datetime.now()

frame=resize(frame)
frame = cv2.GaussianBlur(frame, (5, 5), 1)
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
lower_white = np.array([0, 0, 140])
upper_white = np.array([255, 255, 255])
mask = cv2.inRange(hsv, lower_white, upper_white)

contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
# peri = cv2.arcLength(c, True)
image_dict=[]
for cnt in contours:
    approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
    if len(approx) == 4 or len(approx)==5:
        rect= cv2.boundingRect(cnt)
        x, y, w, h = cv2.boundingRect(cnt)
        #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
        if w<100 or h<100:
            continue
        # if(rect[1])

        # box = cv2.cv.BoxPoints(rect)
        # box = np.int0(box)
        #cv2.drawContours(frame, [box], 0, (0, 0, 255), 2)
        image_dict.append([x, y, w, h])
        #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

image_dict=sorted(image_dict,cmp=cmp_w)
image_dict=sorted(image_dict,cmp=cmp_h)
image_dict=image_dict[0]
frame=frame[image_dict[1]:image_dict[1]+image_dict[3],image_dict[0]:image_dict[0]+image_dict[2]]
mask=mask[image_dict[1]:image_dict[1]+image_dict[3],image_dict[0]:image_dict[0]+image_dict[2]]



contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
# peri = cv2.arcLength(c, True)
count=0
sum_angle=0
for cnt in contours:
    approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
    if len(approx) == 4 or len(approx)==5:
        rect= cv2.minAreaRect(cnt)
        #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
        h,w=rect[1][0],rect[1][1]
        if  w*h<160 or w*h>340:
            continue
        # if(rect[1])
        sum_angle+=rect[2]
        count+=1
        #print w,h,w*h,rect[2]
        box = cv2.cv.BoxPoints(rect)
        box = np.int0(box)
        #cv2.drawContours(frame, [box], 0, (0, 0, 255), 2)
        #image_dict.append([x, y, w, h])
        #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

RotateMatrix = cv2.getRotationMatrix2D(center=(frame.shape[1]/2, frame.shape[0]/2), angle=sum_angle/count+90, scale=1)
frame = cv2.warpAffine(frame, RotateMatrix, (frame.shape[0]*2, frame.shape[1]*1))
mask = cv2.warpAffine(mask, RotateMatrix, (frame.shape[0]*2, frame.shape[1]*1))


contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

min_y=0
avg_width=0
avg_height=0
pos_rect=[]
for cnt in contours:
    approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
    if len(approx) == 4 or len(approx)==5:
        x, y, w, h = cv2.boundingRect(cnt)
        #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
        if  w*h<160 or w*h>380:
            continue
        # if(rect[1])
        avg_width+=w
        avg_height+=h
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
        pos_rect.append([x,y,w,h])

        #image_dict.append([x, y, w, h])
        #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
pos_rect.sort(key=lambda x:(x[0],x[1]))
avg_width/=10
avg_height/=10
avg_x_left=sum([float(i[0]) for i in pos_rect[0:5]])/5
avg_x_right=sum([float(i[0]) for i in pos_rect[5:10]])/5

pos_rect.sort(key=lambda x:(x[1]))
avg_y_top=sum([float(i[1]) for i in pos_rect[0:2]])/2

avg_x_left+=avg_width
print avg_x_left,avg_x_right,avg_width,avg_height
print pos_rect

length=avg_x_right-avg_x_left

x=0.147703180212014*length+avg_x_left
y=avg_y_top
h=2.164948453608247*avg_height
w=0.253003533568905*length

x=int(x)
y=int(y)
h=int(h)
w=int(w)


cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

end = datetime.datetime.now()
print end-begin
#cv2.imshow("RotImg",RotImg)

cv2.imshow("frame",frame)
cv2.imshow("mask",mask)
cv2.waitKey(0)