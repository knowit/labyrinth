import cv2
import numpy as np

img = cv2.imread('brio_scaled-cropped.png',0)
print('loaded image')
img = cv2.medianBlur(img, 5)
print('blured image')
cimg = cv2.cvtColor(img,cv2.COLOR_GRAY2BGR)

# We make all pixels with value lower than 100 into black pixels.
# th, dst = cv2.threshold(img, 100, 000, cv2.THRESH_TOZERO )
th, dst = cv2.threshold(img, 200, 000, cv2.THRESH_TOZERO_INV )



myimg2 = cv2.GaussianBlur(dst,(5,5),0)
# myimg = cv2.cvtColor(myimg2, cv2.COLOR_BGR2GRAY)

#cv2.imshow('detected circles',myimg2)
#cv2.waitKey(0)


circles = cv2.HoughCircles(myimg2,cv2.HOUGH_GRADIENT,1,50,
                            param1=100,param2=20,minRadius=18,maxRadius=23)
print(circles)
circles = np.uint16(np.around(circles))
print(circles)
a = 0
for i in circles[0,:]:
    print(i)
    a += 1
    # if a > 17: break
    # draw the outer circle
    cv2.circle(myimg2,(i[0],i[1]),i[2],(255,255,255),2)
    # draw the center of the circle
    cv2.circle(myimg2,(i[0],i[1]),2,(255,255,255),3)

cv2.imshow('detected circles',myimg2)
cv2.waitKey(0)
cv2.destroyAllWindows()
