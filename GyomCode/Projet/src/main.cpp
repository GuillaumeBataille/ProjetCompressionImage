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
        printf("Usage: ImageIn.ppm ImageLeft.pgm ImageRight.ppm\n");
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

    lire_image_pgm(inputName, ImgIn, nH * nW);

    ImgOut = ImgIn;

    //Carte de disparité
    ecrire_image_pgm("../out/"+inputName,ImgOut,nH,nW);



    return 1;
}
