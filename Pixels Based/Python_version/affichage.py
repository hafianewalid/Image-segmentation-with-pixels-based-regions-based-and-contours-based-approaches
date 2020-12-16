from random import random

import matplotlib.pyplot as plt
from matplotlib import gridspec

gs = gridspec.GridSpec(3,2)

plt.figure(figsize=(10,10))
plt.subplots_adjust(
    bottom =0.05,top = 0.95,
    left  = 0.05 ,right = 0.95,
    wspace = 0.2,hspace = 0.3)

ax1 = plt.subplot(gs[2,:])
ax2 = plt.subplot(gs[:2,0])
ax3 = plt.subplot(gs[:2,1])

x = range(255)

def affh(h1,s):
 x = range(len(h1))
 for i in range(0,s):
  ax1.axvline(i, linewidth=6, color='black')

 ax1.set_xlim(0,255)
 ax1.set_ylim(0,max(h1)+max(h1)/100)


 ax1.plot(x,h1,linewidth=4,color='#ff4444')

 ax1.axvline(s,linestyle='dashed',linewidth=6,color='#00ff7f')

 ax1.set_title('histogramme : '+' ( seuille='+str(s)+')', color='#00ff00',fontsize=18,fontweight='bold')

 plt.show()

def affh_multis(h1, s):
     s=sorted(s)
     x = range(len(h1))
     colors = [(random(), random(), random()) for i in range(len(s)+1)]
     for j in range(0,len(s)+1):
         if(j==0):
             d=0
         else:
             d=s[j-1]
         if(j==len(s)):
             f=256
         else:
             f=s[j]
         for i in range(d,f):
             ax1.axvline(i, linewidth=6, color=colors[j])

     ax1.set_xlim(0, 255)
     ax1.set_ylim(0, max(h1) + max(h1) / 100)

     ax1.plot(x, h1, linewidth=4, color='#ff4444')

     colors = [(random(), random(), random()) for i in range(len(s))]
     for j in range(len(s)):
      ax1.axvline(s[j], linestyle='dashed', linewidth=6,color=colors[j])#color='#00ff7f')

     ax1.set_title('histogramme : ' + ' ( seuille=' + str(s) + ')', color='#00ff00', fontsize=18, fontweight='bold')

     plt.show()
 #plt.legend()


def afficheImage(img1,img2):
    ax2.imshow(img1,cmap='gray', vmin = 0, vmax = 255,aspect='auto')
    ax2.set_title('image avant',color='#00ff00',fontsize=18,fontweight='bold')

    ax3.imshow(img2,cmap='gray', vmin = 0, vmax = 255,aspect='auto')
    ax3.set_title('image apres',color='#00ff00',fontsize=18,fontweight='bold')