#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Vec3b  getpixel(int x , int y ,Mat_<Vec3b> img) {
    Vec3b pix = img.at<Vec3b>(x,y);
    return pix;
}

void setpixel(int x , int y ,Mat_<Vec3b> img,int r ,int g ,int b) {
    Vec3b & pix = img.at<Vec3b>(x,y);
    pix[0]=r;
    pix[1]=g;
    pix[2]=b;
}



vector<int> histogramme(const Mat_<Vec3b> src , int intervalle){
    vector<int> Histo (256,0);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            Histo[getpixel(i,j,src)[0]]++;
        }
    }

    if(intervalle==1) return Histo;

    int t=256/intervalle;
    if(256%intervalle!=0)t++; // si on a un rest on ajout un block pour se
    vector<int> res (t,0);
    int block=intervalle;
    cout<<"  Valeur   ";
    // on calcule tout les block sauf le dernier
    for(int i=0;i<t-1;i++){
        for(int j=(i*block);j<(i*block)+block;j++)
           res[i]+=Histo[j];
        cout<<"  <"<<(i*block)<<" , "<<(i*block)+block<<" > ";
    }
    // on calcule le dernier block ( le rest des elem )
    for(int j=((t-1)*block);j<256;j++)
        res[t-1]+=Histo[j];
    cout<<"  <"<<((t-1)*block)<<" , "<<255<<"> \n";
    cout<<"  Quantité ";
    for(int i=0;i<t;i++){
        cout<<"       "<<res[i]<<"     ";
    }
    cout<<"\n";
    return  res;

}

vector<float> probaI(Mat_<Vec3b> image, int seuil){
    vector<int> H=histogramme(image, 1);
    vector<float> Pi (256,0);
    int sommeH=image.cols*image.rows;

    for(int i=0;i<256;i++) {
        Pi[i] = H[i] / (float)sommeH;
    }

    return Pi;
}

vector<float> probaC(Mat_<Vec3b> image, int seuil){
    vector<float> pc (2,0);
    vector<float> pi=probaI(image,seuil);
    for(int i=0;i<256;i++){
        if(i<seuil){
            pc[0]+=pi[i];
        }else{
            pc[1]+=pi[i];
        }
    }
    return pc;
}

vector<float> calcul_moy(Mat_<Vec3b> image, int seuil){

    vector<float > Pi =probaI(image,seuil);
    vector<float > Pc =probaC(image,seuil);

    vector<float> moy (2,0);

    for(int i=0;i<256;i++){
        if(i<seuil)
            moy[0]+=i*Pi[i];
        else
            moy[1]+=i*Pi[i];
    }

    moy[0]/=Pc[0];
    moy[1]/=Pc[1];

    return moy;
}

vector<float> var_calass(Mat_<Vec3b> image, int seuil){
    vector<float > probai =probaI(image,seuil);
    vector<float > probaj =probaC(image,seuil);
    vector<float > m =calcul_moy(image,seuil);
    vector<float > variance (2,0);

    //for(int i=0;i<256;i++) cout<< " "<< probai[i];
    //cout<<" proba i "<<seuil<< " p1 " << probaj[0] << " p2 " << probaj[1]<<"\n" ;
    //cout<<" m i "<<seuil<< " p1 " << m[0] << " p2 " << m[1]<<"\n" ;

    for(int i=0;i<256;i++){
        if(i<seuil)
         variance[0]+=((i-m[0])*(i-m[0]))*probai[i];
        else
            variance[1]+=((i-m[1])*(i-m[1]))*probai[i];
    }


    variance[0]=variance[0]/probaj[0];
    variance[1]=variance[1]/probaj[1];

    //cout<<" proba i "<<seuil<< " p1 " << variance[0] << " p2 " << variance[1]<<"\n" ;
    return variance;
}


float calculeVariance_intra_class(Mat_<Vec3b> image, int seuil){
    vector<float> p =probaC(image,seuil);
    vector<float> var_c=var_calass(image,seuil);
    return (p[0]*var_c[0])+(p[1]*var_c[1]);
}

int seuillageOtsu(Mat_<Vec3b> image){
    int bests=0;
    float bestvar=999999999;
    float v;
    for(int i=0;i<256;i++){
        v=calculeVariance_intra_class(image,i);
        if(v<bestvar) {
            bestvar=v;
            bests=i;
        }
    }
    return bests-1;
}

void seuillage(const Mat_<Vec3b> src, Mat_<Vec3b>& dest, int seuil){
    dest=Mat_<Vec3b>(src.size(), src.type());
    int s=seuil;
    if(s<0)
        s=seuillageOtsu(src);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if(getpixel(i,j,src)[0]<s)
                setpixel(i,j,dest,0,0,0);
            else setpixel(i,j,dest,255,255,255);
        }
    }

}


int ind_min(int Hi , vector<float> moys ){
    int min_dis=(Hi-moys[0])*(Hi-moys[0]);
    int min_ind=0;
    int dis;
    for(int i=1;i<moys.size();i++){
        dis=(Hi-moys[i])*(Hi-moys[i]);
        if(dis<min_dis){
            min_dis=dis;
            min_ind=i;
        }
    }
    return min_ind;
}

bool mise_a_jour_moy(vector<int> histo , vector<int> C , vector<float>& moys ){
    vector<int> new_somme(moys.size(),0);
    vector<int> nb(moys.size(),0);
    for(int i=0;i<256;i++){
        new_somme[C[i]]+=histo[i]*i;
        nb[C[i]]+=histo[i];
    }
    float new_moy;
    bool changement=false;
    for(int i=0;i<moys.size();i++) {
        new_moy = new_somme[i]/ nb[i]+0.000000001; // pour eviter la div sur 0
        if(moys[i]!= new_moy){
            //cout<<"new m "<<new_moy<<"\n";
            moys[i]=new_moy;
            changement=true;
        }
    }
    return changement;
}

vector<float> kmean(vector<int> histo, int nbClasse){
    vector<float> moys (nbClasse,0);
    vector<int> C (256,0);
    for(int i=0;i<nbClasse;i++){
        bool exist=true;
        while (exist) {
            moys[i] = rand() % 256;
            exist=false;
            for(int j=0;j<i;j++)
                if(moys[i]==moys[j])
                    exist=true;

        }
    }

    bool changement =true;
    while ( changement ){
        for(int i=0;i<256;i++){
            C[i]=ind_min(i,moys);
        }
        changement=mise_a_jour_moy(histo,C,moys);
    }
    return moys;
}

void segmentation_Kmeans(Mat_<Vec3b> src,Mat_<Vec3b>& dest,int K){
    vector<int> Hist=histogramme(src,1);
    vector<float>moy=kmean(Hist,K);
    vector<float>C(256,0);
    for(int i=0;i<256;i++){
        C[i]=(255/K)*(ind_min(i,moy)+1); // distrubution de couleur de seg
    }

    dest=Mat_<Vec3b>(src.size(), src.type());
    int p;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            p=C[getpixel(i,j,src)[0]];
            setpixel(i,j,dest,p,p*2,p*3);
        }
    }
    cout<<"\n";
    for(int i=0;i<K;i++){
        cout<<(int)moy[i]<<" ,";
    }
    cout<<"\n";

}

void affichage(Mat_<Vec3b> image,Mat_<Vec3b> imgres){
    namedWindow( "img" , WINDOW_AUTOSIZE );
    imshow( "img" , image );

    namedWindow( "img res", WINDOW_AUTOSIZE );
    imshow( "img res", imgres );

    waitKey(0);

    bool isSuccess = imwrite("imgres.png", imgres);
    if (isSuccess == false)
    {
        cout << "Failed to save the image" << endl;
        cin.get(); //wait for a key press
        return ;
    }
}

void threshold(String lien , int s){
    Mat_<Vec3b> image=imread(lien);
    if( image.empty() )
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return;
    }
    Mat_<Vec3b> imgres;
    seuillage(image,imgres,s);
    affichage(image,imgres);
}

void Histo_ranges(String lien , int intrvale){
    Mat_<Vec3b> image=imread(lien);
    cout<<"Histogramme de "<<lien<<" avec un intervalle de "<<intrvale<<" est \n";
    vector<int> table = histogramme(image,intrvale);
}

void exo3(String lien){
    Mat_<Vec3b> image=imread(lien);
    Mat_<Vec3b> imgres;
    int s=seuillageOtsu(image);
    seuillage(image,imgres,s);
    cout<<"\n otsu s = "<<s<<"\n";
    affichage(image,imgres);
}
void exo4(String lien , int k){
    Mat_<Vec3b> image=imread(lien);
    Mat_<Vec3b> imgres;
    segmentation_Kmeans(image,imgres,k);
    affichage(image,imgres);
}

int main( int argc, char** argv )
{
    string lien= argv[1];
    if(string(argv[2])=="td"||string(argv[2])=="1"||string(argv[2])=="threshold"||string(argv[2])=="Threshold"){
        int seuil=atoi(argv[3]);
        threshold(lien,seuil);
    }
    if(string(argv[2])=="hr"||string(argv[2])=="2"||string(argv[2])=="histo_ranges"||string(argv[2])=="Histo_ranges"){
        int intervale=atoi(argv[3]);
        Histo_ranges(lien,intervale);
    }
    if(string(argv[2])=="ot"||string(argv[2])=="3"||string(argv[2])=="otsu"||string(argv[2])=="Otsu"){
        exo3(lien);
    }
    if(string(argv[2])=="km"||string(argv[2])=="4"||string(argv[2])=="kmeans"||string(argv[2])=="Kmeans"){
        int k=atoi(argv[3]);
        exo4(lien,k);
    }

    return 0;
}
