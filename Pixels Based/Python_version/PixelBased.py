import Image
import affichage
import cv2
import sys

def binarisation(s,img):
    pixels = list(img.getdata())
    for i in range(len(pixels)):
        if pixels[i]<=s :
         pixels[i]=0
        else:
         pixels[i]=255

    img1 = Image.new('L', img.size)
    img1.putdata(pixels)
    return img1


def hisogramme (img):
    global n,h,pixels
    pixels = list(img.getdata())
    n=len(pixels)
    for i in range(len(pixels)):
        h[pixels[i]] += 1

    return h

def calculedeP():
  global p,h,n
  for i in range(len(h)) :
      p[i]=(float(h[i])/float(n))

def W(deb,fin):
    somme=0.0
    for i in range(deb,fin):
      somme+=p[i]

    return somme

def U(deb, fin):
    u=0.0
    wa=W(deb,fin)

    if wa==0.0 :return 0

    for i in range(deb,fin):
      u+=(i*p[i])

    return u/wa

def var(deb,fin):
   return W(deb,fin)*((U(deb,fin)-UT)**2)


n=0
h = [0.0] * 256
p=[0.0] * 256
UT=0.0
def otsu(img):
 global UT

 hisogramme (img)

 calculedeP()
 UT = 0.0
 for i in range(len(h)):
  UT+=i*p[i]

 s=1
 maxvar=0

 for i in range(1,256):
   sver=(var(0,i)+var(i,256))
   if(sver>=maxvar ):
       maxvar=sver
       s=i

 return s-1
def otsu1(img):
 global UT
 hisogramme (img)
 calculedeP()

 UT=0.0
 for i in range(len(h)):
  UT+=i*p[i]
 print UT
 W0=p[0]
 W1=float(1-W0)

 U0=0.0
 U1=(UT-p[0])/W1

 s=0
 maxdes=( W0*(U0-UT)**2 ) +( W1*(U1-UT)**2 )

 for i in range(1,256):
   U0=(U0*W0)+(i*p[i])
   W0=W0+p[i] 
   if W0!=0 :
    U0=U0/W0 

    U1=(U1*W1)-(i*p[i]) 
    W1=W1-p[i]         
    if W1 != 0:
     U1=U1/W1 

     des=( W0*(U0-UT)**2 ) +( W1*(U1-UT)**2 ) 
     if(des>maxdes):
       maxdes=des
       s=i
 return s


def threshold ( lien , s ):
    img = Image.open(lien).convert('L')
    img1 =binarisation(s, img)
    affichage.afficheImage(img, img1)
    h = hisogramme (img)
    affichage.affh(h,s)
    img1.save('img_res.png','PNG')

def histo_ranges ( lien ):
    img = Image.open(lien).convert('L')
    H=hisogramme (img)
    for i in H :
        print i

def otsu_method ( lien ):
    img = Image.open(lien).convert('L')
    img1 = cv2.imread(lien, 0)
    seuil_de_cv, imgf = cv2.threshold(img1, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    print ('cv_res=', seuil_de_cv)

    T = otsu(img)
    print ('mon_res=', T)
    img1 = binarisation(T, img)

    affichage.afficheImage(img, img1)
    affichage.affh(h, T)
    img1.save('img_res.png','PNG')

lien=str(sys.argv[1])
ex=str(sys.argv[2])
if(ex=="threshold"or ex=="1" or ex=="Threshold"):
 seuil=int(str(sys.argv[3]))
 threshold(lien,seuil)
if(ex=="histo_ranges"or ex=="2" or ex=="Histo_ranges" or ex=="hr"):
 histo_ranges(lien)
if(ex=="otsu"or ex=="3" or ex=="Otsu" or ex=="ot"):
 otsu_method(lien)
