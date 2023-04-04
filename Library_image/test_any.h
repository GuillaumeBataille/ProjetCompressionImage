#include "image_ppm.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <iostream>


int _nTaille, _nH, _nW;

void ecrire_image_pgm(std::vector<OCTET> ImgIn, char *cNomEcriture)
{
    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, _nTaille);

    for (int i = 0; i < ImgIn.size(); i++)
    {
        ImgOut[i] = ImgIn[i];
    }
    ecrire_image_pgm(cNomEcriture, ImgOut, _nH, _nW);
}

std::vector<OCTET> lire_image_pgm(char *NomImageLue)
{

    OCTET *ImgIn;
    lire_nb_lignes_colonnes_image_pgm(NomImageLue, &_nH, &_nW);
    _nTaille = _nH * _nW;
    allocation_tableau(ImgIn, OCTET, _nTaille);
    lire_image_pgm(NomImageLue, ImgIn, _nTaille);
    std::vector<OCTET> ImgOut;
    for (int i = 0; i < _nTaille; i++)
    {
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

// Moyenne d'une zone
float moyenne(std::vector<OCTET> datain)
{
    double total;
    float moy;
    for (int i = 0; i < datain.size(); i++)
    {
        total += datain[i];
    }
    moy = total / (float)datain.size();
    return moy;
}

// Variance d'une zone (sa dispertion sur le spectre des niveau de gris)
float variance(std::vector<OCTET> datain, float moy)
{
    double variance;
    double ecart;
    for (int i = 0; i < datain.size(); i++)
    {
        ecart += std::pow(datain[i] - moy, 2);
    }
    return ecart / (float)datain.size();
}

// Remplir une image par sa moyenne
std::vector<OCTET> ecrire_image_moy(std::vector<OCTET> imagequart, float moy, int nH, int nW)
{
    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            imagequart[i * nW + j] = (int)moy;
        }
    }
    return imagequart;
}

// Prends une image pgm et ça découpe recursivement en 4 tant que la variance de ses blocs sont variance
std::vector<OCTET> image4regionsrecursive(std::vector<OCTET> ImgIn, int nH, int nW, int seuil)
{

    std::vector<OCTET> ImgFull, ImgUpR, ImgUpL, ImgDownR, ImgDownL;
    int nTaille = nH * nW;
    ImgFull.resize(nTaille);

    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {
            ImgUpL.push_back(ImgIn[i * nW + j]);
            ImgUpR.push_back(ImgIn[i * nW + j + nW / 2]);
            ImgDownL.push_back(ImgIn[(i + nH / 2) * nW + j]);
            ImgDownR.push_back(ImgIn[(i + nH / 2) * nW + j + nW / 2]);
        }
    }

    float moy1 = moyenne(ImgUpL);
    float var1 = variance(ImgUpL, moy1);

    float moy2 = moyenne(ImgUpR);
    float var2 = variance(ImgUpR, moy2);

    float moy3 = moyenne(ImgDownL);
    float var3 = variance(ImgDownL, moy3);

    float moy4 = moyenne(ImgDownR);
    float var4 = variance(ImgDownR, moy4);

    // VAR > SEUIL ça veux dire que la zone moyenné ne convient pas / on perds trop d'info a cause d'une dispersion trop importante
    if (var1 > seuil and (nH >= 4 and nW >= 4))
    {
        ImgUpL = image4regionsrecursive(ImgUpL, nH / 2, nW / 2, seuil);
    }
    else
    {
        ImgUpL = ecrire_image_moy(ImgUpL, moy1, nH / 2, nW / 2);
    }

    if (var2 > seuil and (nH >= 4 and nW >= 4))
    {
        ImgUpR = image4regionsrecursive(ImgUpR, nH / 2, nW / 2, seuil);
    }
    else
    {
        ImgUpR = ecrire_image_moy(ImgUpR, moy2, nH / 2, nW / 2);
    }
    if (var3 > seuil and (nH >= 4 and nW >= 4))
    {
        ImgDownL = image4regionsrecursive(ImgDownL, nH / 2, nW / 2, seuil);
    }
    else
    {
        ImgDownL = ecrire_image_moy(ImgDownL, moy3, nH / 2, nW / 2);
    }

    if (var4 > seuil and (nH >= 4 and nW >= 4))
    {
        ImgDownR = image4regionsrecursive(ImgDownR, nH / 2, nW / 2, seuil);
    }
    else
    {
        ImgDownR = ecrire_image_moy(ImgDownR, moy4, nH / 2, nW / 2);
    }
    // Reconstruction de l'image
    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {

            ImgFull[i * nW + j] = ImgUpL[i * (nW / 2) + j];
            ImgFull[i * nW + j + nW / 2] = ImgUpR[i * (nW / 2) + j];
            ImgFull[(i + nH / 2) * nW + j] = ImgDownL[i * (nW / 2) + j];
            ImgFull[(i + nH / 2) * nW + j + nW / 2] = ImgDownR[i * (nW / 2) + j];
        }
    }
    return ImgFull;
}

std::vector<OCTET> get_voisinage(std::vector<OCTET> &ImgIn, int i_L, int j_L, int T_V){
   
    std::vector<OCTET> voisins;
    int i_min = i_L - T_V;
    int j_min = j_L - T_V;

    int i_max = i_L + T_V;
    int j_max = j_L + T_V;

    /*std::cout<<"i_min : "<< i_min <<std::endl;
    std::cout<<"j_min : "<< j_min <<std::endl;
    std::cout<<"i_max : "<< i_max <<std::endl;
    std::cout<<"j_max : "<< j_max <<std::endl;*/

  for (int new_I = i_min; new_I < i_max; new_I++)
    {
        for (int new_J= j_min; new_J < j_max ; new_J++)
        {
            if (new_I < 0 or new_J < 0)voisins.push_back(-1);
            else {if (new_I > _nH or new_J > _nW )voisins.push_back(-1);
             else {
                int newID = new_I*_nW+new_J;
                voisins.push_back(ImgIn[newID]);
            }}
           
        }
    }
    

    return voisins;
}
// corner = current id  + ou - (taille * nH) + ou - (taille)

bool compareVoisinageLetR(std::vector<OCTET> matrice_de_passage_gauche, std::vector<OCTET> matrice_de_passage_droite, int range, float percentage_allowed)
{
    int taille = matrice_de_passage_gauche.size();
    //std::cout<<"taille de la matrice de passage gauche : "<< taille<< std::endl;
    bool res = false; //Valeur resultat qui dit si on est ou pas sur le pixel recherché
    int cpt_valid = 0; // Nombre de valeur considéré comme valide
    float nbr_valid_allowed; //On définit notre nombre tolérable de valeur autorisé
    //Pour chaque pixel du voisinage
    for (int i = 0; i < taille ;i++)
    {   
        int valL = matrice_de_passage_gauche[i] ;
        int valR = matrice_de_passage_droite[i] ;
        //std::cout<<"A l'indice "<< i <<"valL : "<< valL <<" et valR : "<< valR << std::endl;
        // Si on regarde out of bound
        if(valL == -1 || valR == -1)
        {
            continue; // On skip
        }else{
        //On recupère la distance entre valL et valR
        int ecart = abs(valL - valR);
        //Si ses valeurs ne sont pas trop éloignées
        if (ecart < range)
            cpt_valid++; // On la considère valide
    }
    float nbr_valid_allowed = percentage_allowed * (float)taille;
    }
    //std::cout<<"cpt valide = "<< cpt_valid<<" et nbr allowed de valide = "<< nbr_valid_allowed<<std::endl;
    //On retourne si on a plus de pixel valide que de pixel requis
    return (cpt_valid >= nbr_valid_allowed);
}

std::vector<OCTET> disparatemap(std::vector<OCTET> ImgIn_L, std::vector<OCTET> ImgIn_R, int Taille_Voisinage , int range , float pourcentage)
{
    std::vector<OCTET> res;
    res.resize(ImgIn_L.size(),0);

    bool test =false; // Valeur de test declaré ici pour permettre des breaks plus bas
    // Pour chaque pixel de l'image de gauche
    for (int i_L = 0; i_L < _nH; i_L++)
    { 
        for (int j_L = 0; j_L < _nW; j_L++)
        {

            // TODO Recupérer le voisinage de leftID et le stocker dans un vector de niveau de gris
            //Vecteur de taille voisinage * taille voisinage qui contient toute les valeurs du voisinages (-1 si outofbound)
            std::vector<OCTET> matrice_de_passage_gauche = get_voisinage(ImgIn_L , i_L, j_L, Taille_Voisinage);

            int leftID = i_L * _nW + j_L;
            OCTET leftValue = ImgIn_L[leftID];

            test = false;
            // Pour chaque pixel de l'image de droite
            for (int i_R = i_L; i_R < _nH; i_R++)
            {
                for (int j_R = j_L; j_R < _nW; j_R++)
                {

                    // TODO Recupérer le voisinage de rightID et le stocker dans un vector de niveau de gris
                    //Vecteur de taille voisinage * taille voisinage qui contient toute les valeurs du voisinages (-1 si outofbound)
                    std::vector<OCTET> matrice_de_passage_droite = get_voisinage(ImgIn_R,i_R, j_R, Taille_Voisinage);

                    int rightID = i_R * _nW + j_R;
                    OCTET rightValue = ImgIn_L[rightID];
                    
                    //TODO Comparer les mat de passage gauche et droite pour déduire si oui ou non on à affaire au mếme pixel
                    test = compareVoisinageLetR(matrice_de_passage_gauche ,matrice_de_passage_droite, range , pourcentage);
                    
                    if (test){
                     //std::cout << i_L<< " X " << j_L << " | " << i_R<< " X " << j_R << std::endl;
                     break;
                    }
                }
            if (test) break;
            }

        }
    }
    return res;
}

std::vector<OCTET> compute_disparity_map(std::vector<OCTET>left_img, std::vector<OCTET> right_img, int window_size ,int max_disparity)
{
    
    // Create a temporary array to store the intermediate results
    int num_pixels = _nH * _nW;
    int *temp = new int[num_pixels];
    
    
    std::vector<OCTET> disparity_map;
    disparity_map.resize(num_pixels);
    // Compute the disparity map using block matching with a window size of 9x9
    int half_window = window_size / 2;
    for (int y = 0; y < _nH; y++)
    {
        for (int x = 0; x < _nW; x++)
        {
            int min_sad = INT32_MAX;
            int best_disp = 0;
            for (int d = 0; d < max_disparity; d++)
            {
                int sad = 0;
                for (int j = -half_window; j <= half_window; j++)
                {
                    for (int i = -half_window; i <= half_window; i++)
                    {
                        int px_left = left_img[(y + j) * _nW + (x + i)];
                        int px_right = right_img[(y + j ) * _nW + (x + i + d)];
                        int diff = abs(px_left - px_right); // calculate SAD instead of SSD
                        sad += diff;
                    }
                }
                if (sad < min_sad)
                {
                    min_sad = sad;
                    best_disp = d;
                }
            }
            temp[y * _nW + x] = best_disp;
        }
    }
    
    // Normalize the disparity map to the range [0, 255]
    int disp_range = max_disparity - 1;
    float scale = 255.0f / disp_range;
    for (int i = 0; i < num_pixels; i++)
    {
        disparity_map[i] = (OCTET)(temp[i] * scale);
    }
    
    // Free the temporary array
    delete[] temp;
    
    return disparity_map;
    
}

double k (int P_1 , int P_2){
    return std::abs(P_1 - P_2) ; 
}


std::vector<OCTET> edge_distance(std::vector<OCTET>& img_1 , std::vector<OCTET>& img_2 , double threshold)
{
    
    std::vector<OCTET> Edge_map;
    
    for (int y = 0 ; y < _nH ; y++){        
        for (int x = 0 ; x < _nW ; x++){
            
            if (threshold < k(img_1[y * _nW + x] ,img_2[y* _nW + x + 1]))
            {
                Edge_map.push_back(std::clamp(img_1[y * _nW + x] + 1 , 0 , 255));
                
            }
            else{
                Edge_map.push_back(0);

            }
            
        }
        
    }
    return Edge_map;
}
/*
void getRGBDtoOff (std::vector<OCTET> ImgIn, std::vector<OCTET> DisparityMap){
    
    std::vector<float> Depthvalues;
    std::vector<Vec3> Color;
    std::vector<Vec3> Positions;
    std::vector<unsigned int> Indices;
    
    for (int i = 0 ; i<DisparityMap.size() ; i++) {
               Depthvalues.push_back(DisparityMap[i] / 255);
    }
    
    float x, y, z, u, v;
    float Cr , Cv, Cb;
    float pasX = 1 / ((float) _nW - 1);
    float pasY = 1 / ((float) _nH - 1);
    for (int i = 0 ; i< _nH ; i++){
            u = (i * pasY) - (1. / 2.) ;  
            for (int j = 0 ; j < _nW ; j++){
                
                positions des pixels dans le plan 3D.
                v = (j * pasX) - (1. / 2.);  
                x = u;
                y = v;
                z = Depthvalues[i*_nW+j];
                Positions.push_back(Vec3(x,y,z));
                
                couleurs des pixels couleurs du plan
                Cr = ImgIn[i*_nW+j] / 255;
                Cv = ImgIn[i*_nW+j+1] / 255;
                Cb = ImgIn[i*_nW+j+2] / 255;
                Color.push_back(Vec3(Cr,Cv,Cb));                
            }
    }
    for (int i = 0 ; i < _nH-1; i++ ){
        for (int j = 0 ; j < _nW-1; j++ ){
            _indices.push_back(i*_nW+j);
            _indices.push_back(i*_nW+j+1);
            _indices.push_back((i+1)*_nW+j+1);
            
            _indices.push_back(i*_nW+j);
            _indices.push_back((i+1)*_nW+j+1);
            _indices.push_back((i+1)*_nW+j);
        }       
    }
    
    
    
    
}*/


