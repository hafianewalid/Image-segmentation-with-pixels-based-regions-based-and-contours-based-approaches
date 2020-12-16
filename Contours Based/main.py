import cv2
import copy
import numpy as np
import sys


def get_pix(x,y,im):
    w, h = im.shape
    if(x<0 or y<0 or x>=w or y>=h):
        return -1000
    else:
        return im[x,y]

def dilatation(src,element):
    w, h = src.shape
    ew,eh=len(element[0])/2,len(element)/2
    dst=copy.copy(src)
    for i in range(w):
        for j in range(h):
            V=[]
            for ei in range(-ew,ew+1):
                for ej in range(-eh,eh+1):
                  if (element[ew + ei][eh + ej] == 1):
                    V.append(get_pix(i+ei,j+ej,src))
            dst[i,j]=max(V)
    return dst

def erosion(src,element):
    w, h = src.shape
    ew,eh=len(element[0])/2,len(element)/2
    dst=copy.copy(src)
    for i in range(w):
        for j in range(h):
            V=[]
            for ei in range(-ew,ew+1):
                for ej in range(-eh,eh+1):
                   if(element[ew+ei][eh+ej]==1):
                    V.append(abs(get_pix(i+ei,j+ej,src)))
            dst[i,j]=min(V)
    return dst

def ouverture( src,element):
    return dilatation(erosion(src,element),element)

def fermeture ( src, element):
    return erosion(dilatation(src, element), element)

def debuitage( src,element):
    return fermeture(ouverture(src,element),element)



def Gradient_interne( src , element):
    return src-erosion(src,element)


def Gradient_externe( src , element):
    return dilatation(src,element)-src


def Gradient_morphologique( src , element):
    return  dilatation(src,element)-erosion(src,element)

def minima(gradient):
    dst=np.zeros(gradient.shape,np.uint8)
    dstbin=np.zeros(gradient.shape,np.uint8)
    s,im= cv2.threshold(gradient, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    contours, hierarchy = cv2.findContours(im, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for idx in range(len(contours)):
        cv2.drawContours(dst, contours,idx,idx%255,cv2.FILLED,8, hierarchy)
        cv2.drawContours(dstbin, contours, idx,255, cv2.FILLED, 8, hierarchy)
    return dst,dstbin

def colorer(markeur_img):
    l_h = np.uint8(179 * markeur_img / np.max(markeur_img))
    b_ch = 255*np.ones_like(l_h)
    seg_img = cv2.merge([l_h, b_ch, b_ch])
    seg_img = cv2.cvtColor(seg_img, cv2.COLOR_HSV2BGR)
    seg_img[l_h==0] = 0
    return seg_img

def segmentation(im,elem):
    gadiant=Gradient_morphologique(im,elem)
    reg,regbin=minima(gadiant)
    cv2.waitKey()
    G = cv2.cvtColor(im, cv2.COLOR_GRAY2RGB)
    ret, markers = cv2.connectedComponents(reg)
    for i in range(markers.shape[0]):
        for j in range(markers.shape[1]):
            markers[i,j]=reg[i,j]
    markers = cv2.watershed(G,markers)
    c=colorer(np.array(markers))
    return c


lien=str(sys.argv[1])
image = cv2.imread(lien,0)


elem=[[0,1,0],[1,1,1],[0,1,0]]


if('delatation'==str(sys.argv[2])):
   res=dilatation(image,elem)
   cv2.imshow('delatation',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('delatation.png',res)

if('erosion'==str(sys.argv[2])):
   res=erosion(image,elem)
   cv2.imshow('erosion',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('erosion.png',res)

if('ouverture'==str(sys.argv[2])):
   res=ouverture(image,elem)
   cv2.imshow('ouverture',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('ouverture.png',res)

if('fermeture'==str(sys.argv[2])):
   res=fermeture(image,elem)
   cv2.imshow('fermeture',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('fermeture.png',res)

if('Gradient_interne'==str(sys.argv[2])):
   res = Gradient_interne(image, elem)
   cv2.imshow('Gradient_interne',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('Gradient_interne.png',res)

if('Gradient_externe'==str(sys.argv[2])):
   res = Gradient_externe(image, elem)
   cv2.imshow('Gradient_externe',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('Gradient_externe.png',res)

if('G_morphologique'==str(sys.argv[2])):
   res = Gradient_morphologique(image, elem)
   cv2.imshow('G_morphologique',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('G_morphologique.png',res)

if('debruitage'==str(sys.argv[2])):
   res = debuitage(image, elem)
   cv2.imshow('debruitage',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('debruitage.png',res)

if('minima'==str(sys.argv[2])):
   r,res = minima(Gradient_morphologique(image, elem))
   cv2.imshow('minima', res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('minima.png', res)

if('segmentation'==str(sys.argv[2])):
   res = segmentation(image,elem)
   cv2.imshow('segmentation',res)
   cv2.waitKey(0)
   cv2.destroyAllWindows()
   cv2.imwrite('segmentation.png', res)