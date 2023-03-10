#include "image_ppm.h"
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

// Prends une image pgm et ??a d??coupe recursivement en 4 tant que la variance de ses blocs sont variance
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

    // VAR > SEUIL ??a veux dire que la zone moyenn?? ne convient pas / on perds trop d'info a cause d'une dispersion trop importante
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
    bool res = false; //Valeur resultat qui dit si on est ou pas sur le pixel recherch??
    int cpt_valid = 0; // Nombre de valeur consid??r?? comme valide
    float nbr_valid_allowed; //On d??finit notre nombre tol??rable de valeur autoris??
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
        //On recup??re la distance entre valL et valR
        int ecart = abs(valL - valR);
        //Si ses valeurs ne sont pas trop ??loign??es
        if (ecart < range)
            cpt_valid++; // On la consid??re valide
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

    bool test =false; // Valeur de test declar?? ici pour permettre des breaks plus bas
    // Pour chaque pixel de l'image de gauche
    for (int i_L = 0; i_L < _nH; i_L++)
    { 
        for (int j_L = 0; j_L < _nW; j_L++)
        {

            // TODO Recup??rer le voisinage de leftID et le stocker dans un vector de niveau de gris
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

                    // TODO Recup??rer le voisinage de rightID et le stocker dans un vector de niveau de gris
                    //Vecteur de taille voisinage * taille voisinage qui contient toute les valeurs du voisinages (-1 si outofbound)
                    std::vector<OCTET> matrice_de_passage_droite = get_voisinage(ImgIn_R,i_R, j_R, Taille_Voisinage);

                    int rightID = i_R * _nW + j_R;
                    OCTET rightValue = ImgIn_L[rightID];
                    
                    //TODO Comparer les mat de passage gauche et droite pour d??duire si oui ou non on ?? affaire au m???me pixel
                    
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
