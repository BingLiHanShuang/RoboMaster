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
#print hsv_green


frame=cv2.imread("/Users/wzq/Downloads/pic_padpass/200.jpg")
begin = datetime.datetime.now()

frame=resize(frame)
frame = cv2.GaussianBlur(frame, (5, 5), 1)
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
lower_white = np.array([0, 0, 150])
upper_white = np.array([255, 255, 255])
mask = cv2.inRange(hsv, lower_white, upper_white)




contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

min_y=0
avg_width=0
avg_height=0
pos_rect=[]
for cnt in contours:
    approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
    if len(approx) == 4 or len(approx)==5 or len(approx)==6:
        x, y, w, h = cv2.boundingRect(cnt)
        #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
        if  w*h<160 or w*h>380 or h>w:
            continue
        mask_rect = np.zeros((mask.shape[0], mask.shape[1]), np.uint8)
        cv2.rectangle(mask_rect,(x, y), (x + w, y + h),255,-1)
        x1, y1, w1, h1 = cv2.boundingRect(cnt)
        mask_rect_color= mask[y1:y1+h1, x1:x1+w1].mean()
        print w, h, w * h, mask_rect_color
        if(mask_rect_color<210):
            continue

        avg_width+=w
        avg_height+=h
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
        pos_rect.append([x,y,w,h])

        #image_dict.append([x, y, w, h])
        #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
pos_rect.sort(key=lambda x:(x[0],x[1]))
pos_rect_left=pos_rect[0:5]
pos_rect_left.sort(key=lambda x:(x[1]))
pos_rect_right=pos_rect[5:10]
pos_rect_right.sort(key=lambda x:(x[1]))

def slice_first_line():
    arg_line=0
    x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
    y=(pos_rect_left[arg_line][1]+pos_rect_right[arg_line][1])/2
    length=0
    height=0
    for i in range(2):
        length+=(pos_rect_right[i][0]-pos_rect_left[i][0]-pos_rect_left[i][2])
    length=(length)/2
    length = (length) /3

    height+=pos_rect_left[1][1]-y+0.75*pos_rect_left[1][3]
    height+=pos_rect_right[1][1]-y+0.75*pos_rect_right[1][3]
    height=height/2

    for i in range(3):
        cv2.rectangle(frame, (x+ int(length)*i, y), (x + int(length)*(i+1), y + int(height)), (0, 255, 0), 2)

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

    height+=pos_rect_left[3][1]-y+0.25*pos_rect_left[3][3]
    height+=pos_rect_right[3][1]-y+0.25*pos_rect_right[3][3]
    height=height/2

    for i in range(3):
        cv2.rectangle(frame, (x+ int(length)*i, int(y)), (x + int(length)*(i+1), int(y) + int(height)), (0, 255, 0), 2)

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

    height+=pos_rect_left[4][1]-y+1*pos_rect_left[4][3]
    height+=pos_rect_right[4][1]-y+1*pos_rect_right[4][3]
    height=height/2

    for i in range(3):
        cv2.rectangle(frame, (x+ int(length)*i, int(y)), (x + int(length)*(i+1), int(y) + int(height)), (0, 255, 0), 2)

slice_first_line()
slice_second_line()
slice_third_line()
# for i in range(5):
#     print (pos_rect_left[i][1]-pos_rect_right[i][1])/float(pos_rect_left[i][0]-pos_rect_right[i][0])

avg_width/=10
avg_height/=10
avg_x_left=sum([float(i[0]) for i in pos_rect_left])/5
avg_x_right=sum([float(i[0]) for i in pos_rect_right])/5

pos_rect.sort(key=lambda x:(x[1]))
avg_y_top=sum([float(i[1]) for i in pos_rect[0:2]])/2

length=avg_x_right-avg_x_left


#avg_x_left+=avg_width
print avg_x_left,avg_x_right,avg_width,avg_height
print pos_rect


# def slice_num(num):
#     #paramater
#     original_resolution_width=float(4160)
#     original_resolution_height=float(3120)
#     original_gap_horizontal=float(135)
#     original_gap_vertical=float(60)
#     original_box_width=float(365)
#     oritinal_box_height=float(220)
#     original_signrect_width=float(210)
#     original_signrect_height = float(90)
#
#     resize_resolution_width=float(640)
#     resize_resolution_height=float(214)
#     resize_signrect_height=float(avg_height)
#     resize_signrect_width=float(avg_width)
#
#     factor_vertical=resize_signrect_height/original_signrect_height
#     factor_horizontal=resize_signrect_width/original_signrect_width
#
#
#
#
#
#
#     resize_gap_horizontal=factor_horizontal*original_gap_horizontal
#     resize_gap_gap_vertical=factor_vertical*original_gap_vertical
#     resize_box_width=factor_horizontal*original_box_width
#     resize_box_height=factor_vertical*oritinal_box_height
#
#
#
#     x = (avg_width + resize_gap_horizontal)*(num%3+1) + avg_x_left+resize_gap_horizontal*(num%3)
#     y = avg_y_top+(num/3)*(resize_gap_gap_vertical+resize_box_height)
#     h = resize_box_height
#     w = resize_box_width
#
#     return int(x),int(y),int(w),int(h)
#
#
#
#
# for i in range(0,9):
#     x,y,w,h=slice_num(i)
#
#
#
#     cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

end = datetime.datetime.now()
print end-begin
#cv2.imshow("RotImg",RotImg)

cv2.imshow("frame",frame)
cv2.imshow("mask",mask)
cv2.waitKey(0)