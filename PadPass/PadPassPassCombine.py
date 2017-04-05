import cv2
import numpy as np
from sklearn.externals import joblib
from skimage.feature import hog
import datetime
clf = joblib.load("digits_cls.pkl")
def recognize(image):
    roi = cv2.resize(image, (28, 28), interpolation=cv2.INTER_AREA)
    #gray = roi[:, :, 0]

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
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_white = np.array([0, 0, 150])
    upper_white = np.array([255, 255, 255])
    mask = cv2.inRange(hsv, lower_white, upper_white)

    contours, hierarchy = cv2.findContours(mask.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    pos_rect=[]
    for cnt in contours:
        approx = cv2.approxPolyDP(cnt, 0.04 * cv2.arcLength(cnt, True), True)
        if len(approx) == 4 or len(approx)==5 or len(approx)==6:
            x, y, w, h = cv2.boundingRect(cnt)
            #w,h,x,y=int(rect[0][1]),int(rect[0][1]),int(rect[1][0]),int(rect[1][1])
            if  w*h<380 or w*h>750  or h>w:
                continue
            mask_rect = np.zeros((mask.shape[0], mask.shape[1]), np.uint8)
            cv2.rectangle(mask_rect,(x, y), (x + w, y + h),255,-1)
            x1, y1, w1, h1 = cv2.boundingRect(cnt)
            mask_rect_color= mask[y1:y1+h1, x1:x1+w1].mean()
            print w, h, w * h, mask_rect_color
            if(mask_rect_color<190):
                continue
            #cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            pos_rect.append([x,y,w,h])
    # if(len(pos_rect)!=10):
    #     return
    #cv2.imshow("frame", frame)
    #cv2.waitKey(0)
    pos_rect.sort(key=lambda x:(x[0],x[1]))
    pos_rect_left=pos_rect[0:5]
    pos_rect_left.sort(key=lambda x:(x[1]))
    pos_rect_right=pos_rect[5:10]
    pos_rect_right.sort(key=lambda x:(x[1]))
    result = []

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

        height+=pos_rect_left[3][1]-y+0.25*pos_rect_left[3][3]
        height+=pos_rect_right[3][1]-y+0.25*pos_rect_right[3][3]
        height=height/2
        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        for i in range(3):
            result.append(frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])
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

        height+=pos_rect_left[4][1]-y+1*pos_rect_left[4][3]
        height+=pos_rect_right[4][1]-y+1*pos_rect_right[4][3]
        height=height/2

        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        for i in range(3):
            result.append(frame.copy()[y:y+height,x+ length*i:x + length*(i+1)])
            #cv2.rectangle(frame, (x+ length*i, y), (x + length*(i+1), y+ height), (0, 255, 0), 2)
        return result
    def slice_pass():
        arg_line = 0
        x=pos_rect_left[arg_line][0]+pos_rect_left[arg_line][2]
        length= (pos_rect_right[arg_line][0] - pos_rect_left[arg_line][0] - pos_rect_left[arg_line][2])
        length=length/3
        length=length*2
        height=(pos_rect_left[0][3]+pos_rect_right[0][3])/2
        height=height*1.8

        y=(pos_rect_left[0][1]+pos_rect_right[0][1])/2
        y=y-((pos_rect_left[0][3]+pos_rect_right[0][3])/2)*2.1

        y=int(y)
        x=int(x)
        height=int(height)
        length=int(length)
        pass_area=frame.copy()[y:y+height,x:x + length]
        result_pass=[]

        edge = cv2.Canny(pass_area, 500, 2000, apertureSize=5)
        contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        copy = pass_area.copy()
        im_gray = cv2.cvtColor(copy, cv2.COLOR_BGR2GRAY)
        ret, im_th = cv2.threshold(im_gray, 200 , 255, cv2.THRESH_BINARY_INV)

        pad_rect=[]
        max_index=-1
        for i in contours:
            global count
            x2, y2, w2, h2 = cv2.boundingRect(i)
            if w2<3 or h2<10:

                continue
            # print h2
            pad_rect.append([ x2, y2, w2, h2,w2*h2])
            # img_temp = im_th[y2 - 1:y2 + h2 + 1, x2 - 1:x2 + w2 + 1].copy()
            # name=recognize(img_temp)
            # #cv2.rectangle(pass_area, (x2, y2), (x2 + w2, y2 + h2), (0, 0, 255), 2)
            # cv2.imshow(str(count)+"-"+str(name[0]),img_temp)
            # count+=1
            #print name
        pad_rect.sort(key=lambda x: (x[0]))
        pad_rect_filtered=[]
        for i in range(len(pad_rect)-1):
            if abs(pad_rect[i][0]-pad_rect[i+1][0]) <10:#assume the maximum interval between two digit is 10
                present_max=(pad_rect[i] if pad_rect[i][4]-pad_rect[i+1][4]>0 else pad_rect[i+1])
                if len(pad_rect_filtered)==0:
                    pad_rect_filtered.append(present_max)
                    continue

                filter_max=pad_rect_filtered[len(pad_rect_filtered)-1]

                real_max=(present_max if present_max[4]>filter_max[4] else filter_max)
                pad_rect_filtered.remove(filter_max)
                pad_rect_filtered.append(real_max)
            else:
                if abs(pad_rect[i][0]-pad_rect_filtered[len(pad_rect_filtered)-1][0])<10:#assume the maximum interval between two digit is 10
                    continue
                pad_rect_filtered.append(pad_rect[i])
                pad_rect_filtered.append(pad_rect[i+1])

                #if pad_rect[i][5]-pad_rect[i+1][0]
        pad_rect_image=[]
        for i in range(len(pad_rect_filtered)):
            x2,y2,w2,h2,area=pad_rect_filtered[i]
            img_temp = im_th[y2 - 1:y2 + h2 + 1, x2 - 1:x2 + w2 + 1].copy()
            result=recognize(img_temp)
            cv2.imshow("pass-"+str(i)+"-"+str(result[0]),img_temp)
            pad_rect_image.append(img_temp)




        cv2.rectangle(frame, (x, y), (x + length * (1), y + height), (0, 255, 0), 2)


    slice_first_line()
    slice_second_line()
    slice_third_line()
    slice_pass()


    end = datetime.datetime.now()
    print end-begin


    for i in range(9):
        edge = cv2.Canny(result[i], 500, 2000, apertureSize=5)
        contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        copy = result[i].copy()
        im_gray = cv2.cvtColor(copy, cv2.COLOR_BGR2GRAY)
        ret, im_th = cv2.threshold(im_gray, 190 , 255, cv2.THRESH_BINARY_INV)
        temp_dict={}
        max_index=-1
        for i in contours:
            x2, y2, w2, h2 = cv2.boundingRect(i)
            area=w2*h2

            if w2<2 or w2>30 or h2<13:
                continue
            #print w2, h2, area
            # if x2<0.15*w2 or x2>0.85*w2:
            #     continue
            # if(w>0.8*w):
            #     continue
            max_index=max([max_index,area])
            img_temp=im_th[y2-1:y2 + h2+1, x2-1:x2 + w2+1].copy()
            temp_dict[area]=img_temp
            #cv2.rectangle(im_gray, (x2, y2), (x2 + w2, y2 + h2), (0, 0, 255), 2)
            #cv2.imshow(str(count),im_th )
        if max_index==-1:
            cv2.imshow("wzq",im_th)
            print -1
            continue
        name= recognize(temp_dict[max_index])
        print name
        #cv2.imwrite("test/"+str(count)+".jpg",temp_dict[max_index])
            # res=recognize(copy[y2-1:y2 + h2+1, x2-1:x2 + w2+1].copy())
            # #cv2.rectangle(copy, (x2, y2), (x2 + w2, y2 + h2), (0, 0, 255), 2)
        count += 1
        #cv2.imshow(str(count)+"-"+str(name), temp_dict[max_index].copy())





    #cv2.imshow("RotImg",RotImg)

    cv2.imshow("frame",frame)
    #cv2.imshow("mask",mask)
    #cv2.waitKey(0)
count=0

cap=cv2.VideoCapture(1)
cap.set(3, 640)
cap.set(4, 480)
while True:

    success, frame = cap.read()
# error=[123,125,166,188,195,196,176,58]
    frame = cv2.imread("/Users/wzq/RoboMaster/PadPass/test/85.jpg")
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