

#include "../Library_image/image_ppm.h"
#include "../Library_image/test_any.h"
#include <vector>
#include <iostream>

extern int _nH, _nW , _nTaille;
int main(int argc, char* argv[])
{
    
    std::vector<OCTET> image_gauche = lire_image_pgm("../Images_RGB/Img_Left.pgm") ;
    image_gauche = image4regionsrecursive(image_gauche ,_nW , _nW, 500);
    std::vector<OCTET> image_droite = lire_image_pgm("../Images_RGB/Img_Right.pgm") ;
    image_droite =image4regionsrecursive( image_droite ,_nW, _nW ,  500);
    
    ecrire_image_pgm(image_gauche,"segmented.pgm");
    
    
    std::vector<OCTET> image_droite_for_homogene = image_droite;
    
    std::vector<OCTET> Disparity = compute_disparity_map(image_droite , image_gauche , 2 , 100);
    
    
    //std::vector<OCTET> Disparity = lire_image_pgm ("finaldisparity.pgm");
    ecrire_image_pgm(Disparity,"DisparityMap.pgm");
    
    fermetureedge(image_gauche ,image_droite, 35);
    
    std::vector<OCTET> ImgEdgeDistd = edge_distance(image_droite,image_gauche );

    ecrire_image_pgm(ImgEdgeDistd,"edgedistance.pgm");
    
    std::vector<OCTET> homogenous = homogenparam(image_droite_for_homogene, 1 , 150);
    
    ecrire_image_pgm(FinalDisparity(Disparity,ImgEdgeDistd,homogenous, 150),"finaldisparity.pgm");
    
    //image_gauche = image4regionsrecursive(image_gauche , _nH , _nW , 1500);
    //image_droite = image4regionsrecursive(image_gauche , _nH , _nW , 1000);

    //ecrire_image_pgm(image_gauche , "image.pgm");
    //ecrire_image_pgm(image_droite , "image.pgm");
    
}
       
