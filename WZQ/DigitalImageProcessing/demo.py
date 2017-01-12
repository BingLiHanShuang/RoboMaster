import math
import cv2
#reverse color
from matplotlib import pyplot as plt

a=cv2.imread("/Users/wzq/Downloads/lena.jpeg")
b=cv2.bitwise_not(a)
# for i in range(len(a)):
#     for j in range(len(a[i])):
#         a[i][j][0]=255-a[i][j][0]
#         a[i][j][1] = 255 - a[i][j][1]
#         a[i][j][2] = 255 - a[i][j][2]
cv2.imshow("raw",a)
cv2.imshow("reverse color",b)
gray_image = cv2.cvtColor(a, cv2.COLOR_BGR2GRAY)
cv2.imshow("grayscale",gray_image)
edge = cv2.Canny(gray_image, 500, 2000, apertureSize=5)
cv2.imshow("edgedetect",edge)


hist = cv2.calcHist([gray_image],[0],None,[256],[0,256])
plt.hist(gray_image.ravel(),256,[0,256])
plt.title('Histogram for gray scale picture')
plt.show()

cv2.waitKey(0)




#linear tansformation


#threashold transformation means binarize


class image:
    def __init__(self):
        self.image=[[]]
    def judgeturn(self,x,y):
        #jude the turning of the image using the gradient method
        delta_x=abs(self.image[x][y]-self.image[x+1][y])
        delta_y=abs(self.image[x][y]-self.image[x][y-1])
        delta_final=delta_x+delta_y
        return delta_final>300
    def importimage(self,path):
        cv2.imread()

