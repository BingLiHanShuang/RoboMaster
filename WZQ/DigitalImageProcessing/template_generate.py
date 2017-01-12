import cv2
class image:
    def __init__(self, img, (x, y), (w, h)):

        self.img = img
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.result = 0
        self.sum_x = [0] * 100
        self.sum_y = [0] * 100
        self.sum()
       #self.recognize()
    def settemplate(self,template):
        self.template=template
    def sum(self):
        self.sum_x = [0] * 100
        self.sum_y = [0] * 100
        for i in range(self.h):
            for j in range(self.w):
                if self.img[i][j][0]==0:
                    self.sum_x[j] += 1
                    self.sum_y[i] += 1
        return (self.sum_x,self.sum_y)
    def recognize(self):
        temp_dict={}
        for i in range(1,10):
            error=0
            template_x,template_y=self.template[i]
            for j in range(100):
                error +=abs(template_x[j]-self.sum_x[j])+abs(template_y[j]-self.sum_y[j])
            temp_dict[i]=error
        value_min=min(temp_dict.itervalues())
        self.result=[k for k,v in temp_dict.iteritems() if v==value_min][0]
        #print self.result
    def __cmp__(self, other):
        if self.x<other.x:
            return True
        else:
            if self.y<other.y:
                return True
            return False

image_pass = []

for i in range(1,10):
    raw_image = cv2.imread("/Users/wzq/Downloads/1/"+str(i)+"_.bmp")
    edge = cv2.Canny(raw_image, 500, 2000, apertureSize=5)
    contours, hierarchy = cv2.findContours(edge.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    x, y, w, h = cv2.boundingRect(contours[0])
    temp = image(raw_image[y:y + h, x:x + w].copy(), (x, y), (w, h))
    image_pass.append(temp)

    cv2.rectangle(raw_image, (x, y), (x + w, y + h), (0, 255, 0), 2)
    cv2.imshow(str(i),raw_image)
template={}
for i in range(len(image_pass)):
    template[i+1] = image_pass[i].sum()
print template
cv2.waitKey(0)