#include "image_ppm.h"
#include <vector>
#include <iostream>

int _nTaille , _nH , _nW;

void ecrire_image_pgm(std::vector<OCTET> ImgIn , char* cNomEcriture){
    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, _nTaille);

    for (int i = 0; i<ImgIn.size(); i++ ){
        ImgOut[i] = ImgIn[i];
    }
    ecrire_image_pgm(cNomEcriture , ImgOut , _nH , _nW); 
}

std::vector<OCTET> lire_image_pgm(char* NomImageLue){
    
    OCTET* ImgIn;
    lire_nb_lignes_colonnes_image_pgm(NomImageLue , &_nH , &_nW);
    _nTaille = _nH * _nW;
    allocation_tableau(ImgIn , OCTET , _nTaille);
    lire_image_pgm(NomImageLue , ImgIn , _nTaille);
    std::vector<OCTET> ImgOut;
    for (int i = 0 ; i< _nTaille  ; i++) {
        ImgOut.push_back(ImgIn[i]);
    }
    return ImgOut;
}



// std::vector<OCTET> charge_image_ppm(char* NomImageLue){
//     
//     OCTET* ImgIn;
//     lire_nb_lignes_colonnes_image_ppm(NomImageLue , &nH , &nW);
//     nTaille = nH * nW * 3;
//     allocation_tableau(ImgIn , OCTET , nTaille);
//     lire_image_ppm(NomImageLue , ImgIn , nTaille);
//     
//     std::vector<OCTET> ImgOut;
//     for (int i = 0 ; i< nTaille  ; i++) {
//         ImgOut.push_back(ImgIn[i]);
//     }
//     return ImgOut;
//     
// }

float moyenne (std::vector<OCTET> datain){
    double total;
    float moy;
    for (int i = 0 ; i<datain.size() ; i++){
        total += datain[i];
    }
    moy = total / (float) datain.size() ;
    return moy;   
}

float variance(std::vector<OCTET> datain ,float moy){
    double variance;
    double ecart;
    for(int i = 0 ; i< datain.size() ; i++){
        ecart += std::pow(datain[i] - moy , 2) ;
    }
    return ecart/(float)datain.size();
}

std::vector<OCTET> ecrire_image_moy ( std::vector<OCTET> imagequart , float moy , int nH , int nW){
    for (int i = 0 ; i < nH ; i++){
        for (int j = 0 ; j < nW ; j++){
           imagequart[i*nW+j] = (int)moy;
        }
    }
    return imagequart;
}
std::vector<OCTET>  image4regionsrecursive(std::vector<OCTET> ImgIn ,int nH , int nW,  int seuil ){
    
    
    std::vector<OCTET> ImgFull, ImgUpR, ImgUpL, ImgDownR, ImgDownL ; 
   
    int nTaille = nH * nW;
    ImgFull.resize(nTaille);
    
    for (int i = 0 ; i< nH/2 ; i++){
        for (int j = 0 ; j < nW/2 ; j++){
            ImgUpL.push_back(ImgIn[i*nW  +j]);
            ImgUpR.push_back(ImgIn[i*nW  +j + nW/2]);
            ImgDownL.push_back(ImgIn[(i+nH/2)*nW  +j]);
            ImgDownR.push_back(ImgIn[(i+nH/2)*nW  +j + nW/2]);
        }
    }
    
    
    
    float moy1 = moyenne(ImgUpL);
    float var1 = variance(ImgUpL , moy1);
  
    float moy2 = moyenne(ImgUpR);
    float var2 = variance(ImgUpR ,  moy2);
    
    float moy3 = moyenne(ImgDownL);
    float var3 = variance(ImgDownL , moy3);
    
    float moy4 = moyenne(ImgDownR);
    float var4 = variance(ImgDownR , moy4);
    
    
    if (var1 > seuil and ( nH >= 4 and  nW >= 4) ) {ImgUpL = image4regionsrecursive(ImgUpL , nH/2 , nW/2 , seuil);}
    else {
        ImgUpL = ecrire_image_moy(ImgUpL , moy1 , nH/2 , nW/2);
    }
    
    if (var2 > seuil and ( nH >= 4 and  nW >= 4)) {ImgUpR = image4regionsrecursive(ImgUpR , nH/2 , nW/2, seuil);}
    else {
        ImgUpR = ecrire_image_moy(ImgUpR , moy2, nH/2 , nW/2);
        
    }
    if (var3 > seuil  and ( nH >= 4 and  nW >= 4) ) {ImgDownL = image4regionsrecursive(ImgDownL , nH/2 , nW/2, seuil);}
    else {
        ImgDownL = ecrire_image_moy(ImgDownL , moy3, nH/2 , nW/2);
    }
   
    if (var4 > seuil  and ( nH >= 4 and  nW >= 4)) {ImgDownR = image4regionsrecursive(ImgDownR , nH/2 , nW/2, seuil);}
    else {
        ImgDownR = ecrire_image_moy(ImgDownR , moy4, nH/2 , nW/2);
    }
    
    for (int i = 0 ; i < nH/2; i++){
        for (int j = 0 ; j < nW/2 ; j++){
            
            ImgFull[i*nW+ j] = ImgUpL[i*(nW/2) + j];
            ImgFull[i*nW + j + nW/2] = ImgUpR[i*(nW/2)  + j ];
            ImgFull[(i+nH/2)*nW  + j] = ImgDownL[i*(nW/2) + j];
            ImgFull[(i+nH/2)*nW  + j + nW/2 ] = ImgDownR[i*(nW/2) + j];
            
        }
    }    
    return ImgFull;
    
}




