// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include "image_ppm.h"
#include "color.h"
#include "ImageAlgorithms.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ImageIn.ppm Seuil\n");
        exit(1);
    }

    std::string inputName = argv[1];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S;
    S = atoi(argv[2]);

    OCTET *ImgIn, *ImgOut, *ImgGradient, *ImgLaplacien;

    bool color = !(inputName.substr(inputName.size() - 3, 3)).compare("ppm");
    (color ? lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW) : lire_nb_lignes_colonnes_image_pgm(inputName, &nH, &nW));
        std::cout << "Image " << (color ? "couleur" : "niveau de gris") << " en input de taille : " <<nH <<"x"<<nW<< std::endl;

    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    //Allocation et lecture
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgGradient, OCTET, nTaille);
    allocation_tableau(ImgLaplacien, OCTET, nTaille);

    lire_image_pgm(inputName, ImgIn, nH * nW);

    //1-Gradient
    ImageAlgorithms::gradient(ImgIn,ImgGradient,nH,nW);
    ImageAlgorithms::writeProfilDatFile(ImgIn,nH,nW,nW/2,"../dat/profil.dat",0);
    ImageAlgorithms::writeProfilDatFile(ImgGradient,nH,nW,nW/2,"../dat/profil_g.dat",0);
    ecrire_image_pgm("../out/gradiant.pgm",ImgGradient,nH,nW);

    //2- Extraction des maximum locaux et inversion
    ImageAlgorithms::seuillage(ImgGradient,ImgOut,nH,nW,S);
    ecrire_image_pgm("../out/seuilled.pgm",ImgOut,nH,nW);
    ImageAlgorithms::reverse(ImgOut,ImgOut,nTaille);
    ecrire_image_pgm("../out/reversed.pgm",ImgOut,nH,nW);

    //3- Seuillage par hystérésis
    ImageAlgorithms::hysteresis(ImgGradient,ImgOut,nH,nW,15,40);
    ecrire_image_pgm("../out/hysteresis.pgm",ImgOut,nH,nW);

    //4.1- Filtrage moyenneur (flou)
    ImageAlgorithms::flou(ImgIn,ImgOut,nH,nW,0);
    ecrire_image_pgm("../out/moyenneur.pgm",ImgOut,nH,nW);
    ImageAlgorithms::writeProfilDatFile(ImgOut,nH,nW,nW/2,"../dat/profil_moy.dat",0);
    //4.2- Gradiant moyenneur
    ImageAlgorithms::gradient(ImgOut,ImgGradient,nH,nW);
    ecrire_image_pgm("../out/gradient_M.pgm",ImgGradient,nH,nW);
    ImageAlgorithms::writeProfilDatFile(ImgGradient,nH,nW,nW/2,"../dat/profil_g_moy.dat",0);

    //4.3- Seuillage classique sur gradiant moyenneur
    ImageAlgorithms::seuillage(ImgGradient,ImgOut,nH,nW,30);
    ecrire_image_pgm("../out/seuilled_M.pgm",ImgOut,nH,nW);
    ImageAlgorithms::reverse(ImgOut,ImgOut,nTaille);
    ecrire_image_pgm("../out/reversed_M.pgm",ImgOut,nH,nW);
    //4.4- Seuillage hysteresis sur gradiant moyenneeur
    ImageAlgorithms::hysteresis(ImgGradient,ImgOut,nH,nW,15,35);
    ecrire_image_pgm("../out/hysteresis_M.pgm",ImgOut,nH,nW);

    //5- Filtrage Laplacien + 128
    ImageAlgorithms::Laplacien(ImgIn,ImgOut,nH,nW,0);
    ecrire_image_pgm("../out/laplacien+128.pgm",ImgOut,nH,nW);

    //6-Recherche passage par 0 pour detecter les contours
    ImageAlgorithms::passage_zero(ImgOut,ImgLaplacien,ImgOut,nH,nW);
    ecrire_image_pgm("../out/laplacien.pgm",ImgLaplacien,nH,nW); // Le laplacien de l'exo précédent sans +128
    ecrire_image_pgm("../out/contour_laplacien.pgm",ImgOut,nH,nW); // Le contour par laplacien

    //7 - Appliquer un seuillage par hysteresis sur le contour laplacien
    ImageAlgorithms::hysteresis(ImgOut,ImgOut,nH,nW,50,80);
    ecrire_image_pgm("../out/hysteresis_Laplace.pgm",ImgOut,nH,nW);



    return 1;
}
