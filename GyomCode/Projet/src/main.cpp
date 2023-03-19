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
        printf("Usage: ImageLeft.pgm ImageRight.ppm\n");
        exit(1);
    }

    std::string inputNameLeft = argv[1];
    std::string inputNameRight = argv[2];
    inputNameLeft = "../in/" + inputNameLeft;
    inputNameRight = "../in/" + inputNameRight;

    int nH, nW, nTaille;

    OCTET *ImgLeft, *ImgRight, *ImgOut, *ImgGradient, *ImgLaplacien;

    bool color = !(inputNameLeft.substr(inputNameLeft.size() - 3, 3)).compare("ppm");
    (color ? lire_nb_lignes_colonnes_image_ppm(inputNameLeft, &nH, &nW) : lire_nb_lignes_colonnes_image_pgm(inputNameLeft, &nH, &nW));
    std::cout << "Image " << (color ? "couleur" : "niveau de gris") << " en input de taille : " << nH << "x" << nW << std::endl;

    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    // Allocation et lecture
    allocation_tableau(ImgLeft, OCTET, nTaille);
    allocation_tableau(ImgRight, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    lire_image_pgm(inputNameLeft, ImgLeft, nH * nW);
    lire_image_pgm(inputNameRight, ImgRight, nH * nW);
    ImageAlgorithms::normalize_image_intensity(ImgLeft, nW, nH);
    ImageAlgorithms::normalize_image_intensity(ImgRight, nW, nH);
    /*ImageAlgorithms::smooth_image(ImgLeft, nW, nH, 16);
    ImageAlgorithms::smooth_image(ImgRight, nW, nH, 16);

    ImageAlgorithms::smooth_image(ImgLeft, nW, nH, 16);
    ImageAlgorithms::smooth_image(ImgRight, nW, nH, 16);*/
    // ImageAlgorithms::normalize_image_intensity(ImgLeft, nW, nH);
    int max_disp = 64; // plage de disparité maximale
    ImageAlgorithms::compute_disparity_map(ImgLeft, ImgRight, nW, nH, ImgOut, max_disp);

    //  Écriture de l'image de disparité
    ecrire_image_pgm("../out/DisparityMap.pgm", ImgOut, nH, nW);

    return 1;
}
