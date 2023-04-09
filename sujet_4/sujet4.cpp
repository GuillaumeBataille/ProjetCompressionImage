

#include "../Library_image/image_ppm.h"
#include "../Library_image/test_any.h"
#include <vector>
#include <iostream>

extern int _nH, _nW , _nTaille;
int main(int argc, char* argv[])
{
    std::vector<OCTET> image_gauche = lire_image_pgm("../Images_RGB/im2.pgm");
    std::vector<OCTET> image_droite = lire_image_pgm("../Images_RGB/im6.pgm");
    
    std::vector<OCTET> Disparity = compute_disparity_map(image_droite , image_gauche , 4 , 150);
    
    ecrire_image_pgm(Disparity,"DisparityMap.pgm");
    
    fermetureedge(image_droite , image_gauche , 30);
   
    std::vector<OCTET> ImgEdgeDistd = edge_distance(image_droite , image_gauche );
    std::vector<OCTET> ImgEdgeDistg = edge_distance(image_gauche, image_droite);
    
    
    ecrire_image_pgm(image_droite,"edgedfermeture.pgm");
    ecrire_image_pgm(image_gauche,"edgegfermeture.pgm");
    
    ecrire_image_pgm(ImgEdgeDistd,"test2.pgm");
    ecrire_image_pgm(ImgEdgeDistg,"test1.pgm");
    
    std::vector<OCTET> homogenous = homogenparam(image_gauche, 1, 100);
    
    ecrire_image_pgm(FinalDisparity(Disparity,ImgEdgeDistg,homogenous, 150),"finaldisparity.pgm");
    
    //image_gauche = image4regionsrecursive(image_gauche , _nH , _nW , 1500);
    //image_droite = image4regionsrecursive(image_gauche , _nH , _nW , 1000);

    //ecrire_image_pgm(image_gauche , "image.pgm");
    //ecrire_image_pgm(image_droite , "image.pgm");
    
}
       
