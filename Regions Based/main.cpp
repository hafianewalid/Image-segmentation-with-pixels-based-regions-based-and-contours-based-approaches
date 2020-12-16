#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <list>

using namespace cv;
using namespace std;

int seuil = 20;
bool conex_8= true;
bool bascule= true;
Mat_<Vec3b> image;
Mat_<Vec3b> imagecopy;
list<Point> germes;



Vec3b  getpixel(int x , int y , Mat_<Vec3b> img) {
    if(!(x<0||x>=img.cols||y<0||y>=img.rows)) {
        Vec3b pix = img.at<Vec3b>(y, x);
        return pix;
    }
    return Vec3b(255,255,255);
}

void setpixel(int x , int y ,Mat_<Vec3b>& img,int r ,int g ,int b) {
    if(!(x<0||x>=img.cols||y<0||y>=img.rows)){
        Vec3b &pix = img.at<Vec3b>(y,x);
        pix[0] = r;
        pix[1] = g;
        pix[2] = b;
    }
}

//// affectation d'une couleur a chaque region
void segmentation_color(Mat_<Vec3b> src,Mat_<Vec3b>& dest,int K){
    vector<int>C(K,0);
    for(int i=0;i<K;i++){
        C[i]=(255/K)*(i+1); // distrubution de couleur de seg
    }

    dest=src.clone();
    int p;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            p=C[getpixel(j,i,src)[0]];
            setpixel(j,i,dest,p,p*2,p*3);
        }
    }

}

/// le predicat de similarite
bool predicat(Point p1 ,Point p2,Mat_<Vec3b> img,int seuil){
      int i1=getpixel(p1.x,p1.y,img)[0];
      int i2=getpixel(p2.x,p2.y,img)[0];
      return abs(i1-i2)<seuil;
}

/// recuperer les voisin qui respecte la similarite et la conexite ( 4 ou 8 au choix )
/// marquer les voisin selectionner comme des voisin visiter
list<Point> get_voisin(Point p,Point g,int seuil ,Mat_<Vec3b>& visite,Mat_<Vec3b> img ,bool conx8,int c){

    int x=p.x;
    int y=p.y;
    int ver;
    list<Point> voisin;
    ver=getpixel(x+1,y,visite)[0];
    if(ver!=255&&ver!=c && predicat(g,Point(x+1,y),img,seuil)){
        voisin.push_front(Point(x+1,y));
        setpixel(x+1,y,visite,c,c,c);
    }
    ver=getpixel(x-1,y,visite)[0];
    if(ver!=255&&ver!=c&& predicat(g,Point(x-1,y),img,seuil)){
        voisin.push_front(Point(x-1,y));
        setpixel(x-1,y,visite,c,c,c);
    }
    ver=getpixel(x,y+1,visite)[0];
    if(ver!=255&&ver!=c&&predicat(g,Point(x,y+1),img,seuil)){
        voisin.push_front(Point(x,y+1));
        setpixel(x,y+1,visite,c,c,c);
    }
    ver=getpixel(x,y-1,visite)[0];
    if(ver!=255&&ver!=c&& predicat(g,Point(x,y-1),img,seuil)){
        voisin.push_front(Point(x,y-1));
        setpixel(x,y-1,visite,c,c,c);
    }
    if(conx8) {
        ver=getpixel(x + 1, y + 1, visite)[0];
        if (ver!=255&&ver!=c && predicat(g, Point(x + 1, y + 1), img, seuil)) {
            voisin.push_front(Point(x + 1, y + 1));
            setpixel(x + 1, y + 1, visite, c,c,c);
        }
        ver=getpixel(x + 1, y - 1, visite)[0];
        if (ver!=255&&ver!=c&& predicat(g, Point(x + 1, y - 1), img, seuil)) {
            voisin.push_front(Point(x + 1, y - 1));
            setpixel(x + 1, y - 1, visite, c,c,c);
        }
        ver=getpixel(x - 1, y + 1, visite)[0];
        if (ver!=255&&ver!=c && predicat(g, Point(x - 1, y + 1), img, seuil)) {
            voisin.push_front(Point(x - 1, y + 1));
            setpixel(x - 1, y + 1, visite, c,c,c);
        }
        ver=getpixel(x - 1, y - 1, visite)[0];
        if (ver!=255&&ver!=c && predicat(g, Point(x - 1, y - 1), img, seuil)) {
            voisin.push_front(Point(x - 1, y - 1));
            setpixel(x - 1, y - 1, visite, c,c,c);
        }

    }

    return voisin;

}

/// une coissance de Region pour tout les germes
void croissanceRegion(Mat_<Vec3b> src,Mat_<Vec3b>& dst, list<Point> germes, int seuil, bool type_voisinage){
    Mat_<Vec3b> visite=src.clone();

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            setpixel(j,i,visite,0,0,0);
        }
    }

    int c=1;
    list<Point> pile,valide_voisin;
    Point p;

    for(auto g :germes ){
        setpixel(g.x,g.y,visite,c,c,c);
        pile.push_back(g);
        while (!pile.empty()){
            p=pile.back();
            pile.pop_back();
            valide_voisin=get_voisin(p,g,seuil,visite,image,type_voisinage,c);
            for( auto v : valide_voisin)pile.push_back(v);
        }
        c++;
    }

    segmentation_color(visite,dst,c);

}


//// selection des germe
//// switch entre l image et l image segmenter
static void  germeSelect( int event, int x, int y, int, void* )
{
    if(event==EVENT_LBUTTONDOWN) {
        //// mise a jour d'affichage
        Point p;
        p=Point(x, y);
        germes.push_back(p);
        circle(imagecopy,p, 2.0, Scalar(0, 0, 255), 2, 8);
        imshow("croissance de region", imagecopy);
    }
    if(event==EVENT_RBUTTONDOWN) {
        //// mise a jour d'affichage
        if (!germes.empty()) {
            if (bascule) {
                bascule=!bascule;
                croissanceRegion(image, imagecopy, germes, seuil, conex_8);
                imshow("croissance de region", imagecopy);
            } else {
                bascule=!bascule;
                imagecopy = image.clone();
                for (auto p : germes)
                    circle(imagecopy, p, 2.0, Scalar(0, 0, 255), 2, 8);
                imshow("croissance de region", imagecopy);
            }
        }

    }



}
// reglage de seuil
void on_trackbar( int, void* )
{
    cout <<"seuil = "<< seuil << "\n";
    //// mise a jour d'affichage
    if (!germes.empty()) {
        if (!bascule) {
            croissanceRegion(image, imagecopy, germes, seuil, conex_8);
            imshow("croissance de region", imagecopy);
        }
    }
}


int main(int argc, char** argv ) {
    String lien="coins.png";
    if(argv[1]!=NULL)lien=argv[1];

    image= imread(lien);

    if ( !image.data )
    {
        cout <<"pas image data \n";
        return 1 ;
    } else {

        imagecopy= image.clone();

        namedWindow("croissance de region", WINDOW_AUTOSIZE);
        imshow("croissance de region", imagecopy);

        createTrackbar("seuil", "croissance de region", &seuil, 255, on_trackbar);
        setMouseCallback("croissance de region", germeSelect, 0);

        int a;
        bool non_quit = true;

        while (non_quit) {

            a = waitKey(0);
            non_quit = false;

            //// R
            if (a == 114) {
                cout << " ajouter germes aliatoire  \n ";
                germes.push_back(Point(rand() % image.cols, rand() % image.rows));
                non_quit = true;
            }

            //// C
            if (a == 99) {
                cout << " changement de conexité  \n ";
                non_quit = true;
                conex_8 = (!conex_8);
            }

            //// S
            if (a == 115) {
                imagecopy = image.clone();
                for (auto p : germes)
                    circle(imagecopy, p, 2.0, Scalar(0, 0, 255), 2, 8);
                bool s1 = imwrite("img_seg.png",imagecopy);
                croissanceRegion(image, imagecopy, germes, seuil, conex_8);
                bool s2 = imwrite("img_germes"+std::to_string(seuil)+".png",imagecopy);
                if (s1 == false||s2 == false)cout << "echec de save l image \n" << endl;
                else cout << " save d img segmentation " << endl;
                non_quit = true;
            }

            //// mise a jour d'affichage
            if (non_quit && !germes.empty()) {
                if (!bascule) {
                    croissanceRegion(image, imagecopy, germes, seuil, conex_8);
                    imshow("croissance de region", imagecopy);
                } else {
                    imagecopy = image.clone();
                    for (auto p : germes)
                        circle(imagecopy, p, 2.0, Scalar(0, 0, 255), 2, 8);
                    imshow("croissance de region", imagecopy);
                }
            }
        }
    }

    return 0;
}
