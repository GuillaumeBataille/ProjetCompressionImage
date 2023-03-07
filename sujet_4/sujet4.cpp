

#include "../Library_image/image_ppm.h"
#include "../Library_image/test_any.h"
#include <vector>
#include <iostream>

extern int _nH, _nW , _nTaille;
int main(int argc, char* argv[])
{
    std::vector<OCTET> image_gauche = lire_image_pgm("../Images_RGB/left_bycle.pgm");
   // std::vector<OCTET> image_droite = lire_image_pgm("../Images_RGB/right_bycle.pgm");
    
    std::cout<< _nH << " X " << _nW <<std::endl;
    
    image_gauche = image4regionsrecursive(image_gauche , _nH , _nW , 1500);
    //image_droite = image4regionsrecursive(image_gauche , _nH , _nW , 1000);

    ecrire_image_pgm(image_gauche , "image.pgm");
    //ecrire_image_pgm(image_droite , "image.pgm");
    
}
       
