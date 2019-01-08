//
// Created by marco on 07/01/19.
//

#include "common.h"

#include <cstdlib>
#include <cstdint>
#ifndef ACAPROJECT_SOBEL_H
#define ACAPROJECT_SOBEL_H

#endif //ACAPROJECT_SOBEL_H

/*
 * This structure reprents an image, either in RGBA or in GreyScale.
 */

struct image {
    uint32_t width; // Width of the picture in pixels
    uint32_t height; // Height of the picture in pixels
    unsigned char *data; // Actual data width*height for a GreyScale image.

};


/*
 * This struct reprensents a matrix that's not necessarily an image
 */


struct matrix {
    uint32_t width; // Width in elements
    uint32_t height; // Height in elements
    int16_t *data; // Actual data, of width*height elements
};


#define MATRIX_INITIALIZER {0, 0, NULL}
#define IMAGE_INITIALIZER {0, 0, NULL}

// define kernel as square matrix
#define KERNEL_DIM 3
typedef signed char kernel_t[KERNEL_DIM][KERNEL_DIM] ;

/*
 * Sobel G_x kernel is a matrix 3x3 with this weights:
 *
 *            | -1   0   1 |
 *    G__x =  | -2   0   2 |
 *            | -1   0   1 |
 *
 * This compute gradient along y-axis.
 */

static inline void initKernelX (kernel_t kernelX) {

    kernelX[0][0] = -1;
    kernelX[0][1] = 0;
    kernelX[0][2] = 1;

    kernelX[1][0] = -2;
    kernelX[1][1] = 0;
    kernelX[1][2] = 2;

    kernelX[2][0] = -1;
    kernelX[2][1] = 0;
    kernelX[2][2] = 1;

}

/*
 * Sobel G_y kernel is a matrix 3x3 with this weights:
 *
 *            |  1   2   1 |
 *    G__y =  |  0   0   0 |
 *            | -1  -2  -1 |
 *
 * This compute gradient along x-axis.
 */


static inline void initKernelY (kernel_t kernelY) {

    kernelY[0][0] = -1;
    kernelY[0][1] = -2;
    kernelY[0][2] = -1;

    kernelY[1][0] = 0;
    kernelY[1][1] = 0;
    kernelY[1][2] = 0;

    kernelY[2][0] = 1;
    kernelY[2][1] = 2;
    kernelY[2][2] = 1;

}

/*
 * For a 3x3 kernel the convolution is easy: ( k is kernel function and I is the image )
 * for each pixel at points (i,j):
 *
 *  convoluted(i,j) =  k(0,0) * I(i+1, j+1)
 *                    +k(0,1) * I(i+1, j)
 *                    +k(0,2) * I(i+1, j-1)
 *
 *                    +k(1,0) * I(i, j+1)
 *                    +k(1,1) * I(i, j)
 *                    +k(1,2) * I(i, j-1)
 *
 *                    +k(2,0) * I(i-1, j+1)
 *                    +k(2,1) * I(i-1, j)
 *                    +k(2,2) * I(i-1, j-1)
 *
 *  Operator fails for border conditions:
 *      (i,j = 0) AND (i = M and j = N) where image is not defined.
 *
 *  we compute the convolution for values that we can correctly compute and then we fill the unknown values at the edges
 *  repeating the inner borders.
 *
 *    ? ? ? ? ?                                           1 1 2 3 3
 *    ? 1 2 3 ?                                           1 1 2 3 3
 *    ? 4 5 6 ?   it would be artificially extended to    4 4 5 6 6
 *    ? 7 8 9 ?                                           7 7 8 9 9
 *    ? ? ? ? ?                                           7 7 8 9 9
 */


int convolution(struct image *pInImage, kernel_t kernel, struct matrix *pOutMatrix) {


    pOutMatrix->width = pInImage->width;
    pOutMatrix->height = pInImage->height;
    pOutMatrix->data = static_cast<int16_t *>(calloc(pInImage->width * pInImage->height, sizeof(int16_t)));


    // we make convolution where possible:
    // ROWS: we starts from row = 1 (instead of 0) and we end at row = N-1 (where N is the height)
    // COLUMNS: we starts from col = 1 (instead of 0) and we end at col = M-1 (where M is the width)

    uint32_t w = pInImage->width;


    for (uint32_t row = 1; row < pInImage->height -1; row ++) {


        for (uint32_t col = 1; col < pInImage->width -1; col++) {

            int16_t acc = 0;

            acc += kernel[0][0] * pInImage->data[(row + 1)*w + col + 1];
            acc += kernel[0][1] * pInImage->data[(row + 1)*w + col];
            acc += kernel[0][2] * pInImage->data[(row + 1)*w + col - 1];

            acc += kernel[1][0] * pInImage->data[row*w + col + 1];
            acc += kernel[1][1] * pInImage->data[row*w + col];
            acc += kernel[1][2] * pInImage->data[row*w + col - 1];

            acc += kernel[2][0] * pInImage->data[(row - 1)*w + col + 1];
            acc += kernel[2][1] * pInImage->data[(row - 1)*w + col];
            acc += kernel[2][2] * pInImage->data[(row - 1)*w + col - 1];

            pOutMatrix->data[row*pOutMatrix->width + col] = acc; // save computed pixel to output matrix

        }
    }

    for (uint32_t col = 1; col < pOutMatrix->width - 1; col++) {
        pOutMatrix->data[col] = pOutMatrix->data[pOutMatrix->width + 1];

        uint32_t startBeforeLastRow = pOutMatrix->width * (pOutMatrix->height - 2);
        uint32_t startLastRow = pOutMatrix->width * (pOutMatrix->height - 1);
        pOutMatrix->data[startLastRow + col] = pOutMatrix->data[startBeforeLastRow + col];
    }


    for (uint32_t startRow = 0; startRow < pOutMatrix->height; startRow += pOutMatrix->width) {
        pOutMatrix->data[startRow] = pOutMatrix->data[startRow + 1];

        pOutMatrix->data[startRow + pOutMatrix->width - 1] =
                pOutMatrix->data[startRow + pOutMatrix->width - 2];
    }
    return 0;

}

/* Works on GreyScale images */


static inline void normalize_matrix_to_image(struct matrix *pMat, struct image *pImg)
{
    int16_t max = ~0; // min value
    for (uint32_t px = 0; px < pMat->width * pMat->height; px++) {
        if (pMat->data[px] > max) {
            max = pMat->data[px];
        }
    }

    for (uint32_t px = 0; px < pMat->width * pMat->height; px++) {
        pImg->data[px] = (unsigned char) ((pMat->data[px] * 255) / max);
    }
}

int gradient_norm(struct matrix *pInMatrix_X, struct matrix *pInMatrix_Y, struct image *pOutImage) {

    // prepare output image container
    pOutImage->width = pInMatrix_X->width;
    pOutImage->height = pInMatrix_X->height;
    pOutImage->data = static_cast<unsigned char *>(calloc(pOutImage->width * pOutImage->height, sizeof(unsigned char)));

    struct matrix unNormalizedGradient = MATRIX_INITIALIZER;
    unNormalizedGradient.width = pOutImage->width;
    unNormalizedGradient.height = pOutImage->height;
    unNormalizedGradient.data = static_cast<int16_t *>(calloc(pOutImage->width * pOutImage->height, sizeof(int16_t)));


    for (uint32_t px = 0; px < pOutImage->width * pOutImage->height; px++) {
        uint16_t val = norm2(pInMatrix_X->data[px], pInMatrix_Y->data[px]);
        unNormalizedGradient.data[px] = val;

      //  pOutImage->data[px] = norm2(pInMatrix_X->data[px], pInMatrix_Y->data[px]);

    }

    normalize_matrix_to_image(&unNormalizedGradient,pOutImage);

}




int sobel(struct image *pInImage, struct image *pOutImage) {

    struct matrix gradX = MATRIX_INITIALIZER;
    struct matrix gradY = MATRIX_INITIALIZER;
    kernel_t kernelX;
    kernel_t kernelY;

    // init kernel matrices
    initKernelX(kernelX);
    initKernelY(kernelY);

    convolution(pInImage, kernelX, &gradX);
    convolution(pInImage, kernelY, &gradY);

    gradient_norm(&gradX, &gradY, pOutImage);
}

