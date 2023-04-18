import cv2
import numpy as np

# Charger les deux images stéréo et la carte de disparité de référence
imgL = cv2.imread('im2.pgm', cv2.IMREAD_GRAYSCALE)
imgR = cv2.imread('im6.pgm', cv2.IMREAD_GRAYSCALE)
true_disp = cv2.imread('truedisp.pgm', cv2.IMREAD_GRAYSCALE)

# Créer un objet stereoBM pour calculer la carte de disparité
stereo = cv2.StereoBM_create(numDisparities=64, blockSize=15)

# Calculer la carte de disparité
disparity = stereo.compute(imgL, imgR)
disparity_normalized = cv2.normalize(
    disparity, None, alpha=10, beta=20, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
# Calculer le PSNR
# Calculer le PSNR
psnr = cv2.PSNR(true_disp, disparity_normalized.astype('uint8'), 255)
print('PSNR: {:.2f}'.format(psnr))

# Afficher la carte de disparité
cv2.imshow('Disparity Map', disparity_normalized)
cv2.waitKey(0)
cv2.destroyAllWindows()
