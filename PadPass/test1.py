import cv2
gray=cv2.imread("/Users/wzq/pic01/2047_96156_586397412.jpg",0)
blurred = cv2.GaussianBlur(gray, (5, 5), 1)
edged = cv2.Canny(blurred, 50, 0, 500)


contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for cnt in contours:
    approx = cv2.approxPolyDP(cnt, 0.01 * cv2.arcLength(cnt, True), True)
    if len(approx) == 4:
        x, y, w, h = cv2.boundingRect(cnt)
        if(w<20 or h < 20 or w>150 or h>150):
            continue
        cv2.rectangle(gray, (x, y), (x + w, y + h), (0, 0, 255), 2)

        #cv2.drawContours(gray, [cnt], 0, (0, 0, 255), -1)
cv2.imshow("",gray)
cv2.waitKey(0)