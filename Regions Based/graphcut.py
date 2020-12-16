import numpy as np
import cv2 as cv
import sys
from matplotlib import pyplot as plt

lien=str(sys.argv[1])
img = cv.imread(lien)
mask = np.zeros(img.shape[:2],np.uint8)
bgdModel = np.zeros((1,65),np.float64)
fgdModel = np.zeros((1,65),np.float64)
n,m= img.shape[:2]
cv.grabCut(img,mask,(0,0,m-1,n-1),bgdModel,fgdModel,5,cv.GC_INIT_WITH_RECT)
mask2 = np.where((mask==2)|(mask==0),0,1).astype('uint8')
img = img*mask2[:,:,np.newaxis]
plt.imshow(img),plt.show()
cv.imwrite("res.png",img)