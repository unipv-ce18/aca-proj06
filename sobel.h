//
// Created by marco on 07/01/19.
//

#include "common.h"
#include <iostream>

#ifndef ACAPROJECT_SOBEL_H
#define ACAPROJECT_SOBEL_H

#endif //ACAPROJECT_SOBEL_H


/*
 * For a 3x3 kernel the convolution is easy: ( k is kernel function and I is the image )
 * for each pixel at points (i,j):
 *
 *  convoluted(i,j) =  k(0,0) * I(i-1, j-1)
 *                    +k(0,1) * I(i-1, j)
 *                    +k(0,2) * I(i-1, j+1)
 *
 *                    +k(1,0) * I(i, j-1)
 *                    +k(1,1) * I(i, j)
 *                    +k(1,2) * I(i, j+1)
 *
 *                    +k(2,0) * I(i+1, j-1)
 *                    +k(2,1) * I(i+1, j)
 *                    +k(2,2) * I(i+1, j+1)
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

            acc += kernel[0][0] * pInImage->data[(row - 1)*w + col - 1];
            acc += kernel[0][1] * pInImage->data[(row - 1)*w + col ];
            acc += kernel[0][2] * pInImage->data[(row - 1)*w + col + 1];

            acc += kernel[1][0] * pInImage->data[row*w + col - 1];
            acc += kernel[1][1] * pInImage->data[row*w + col];
            acc += kernel[1][2] * pInImage->data[row*w + col + 1];

            acc += kernel[2][0] * pInImage->data[(row + 1)*w + col - 1];
            acc += kernel[2][1] * pInImage->data[(row + 1)*w + col];
            acc += kernel[2][2] * pInImage->data[(row + 1)*w + col + 1];


            pOutMatrix->data[row*pOutMatrix->width + col] = acc; // save computed pixel to output matrix

        }
    }

    // expand matrix:replicate top border

    for (uint32_t col = 1; col < pOutMatrix->width -1; col++) {
        uint32_t row_length = pOutMatrix->width;
        pOutMatrix->data[0 + col] = pOutMatrix->data[row_length + col]; // replicate first row
        pOutMatrix->data[row_length * (pOutMatrix->height -1) + col] = pOutMatrix->data[row_length * (pOutMatrix->height -2) + col]; // replicate last row
    }

    for (uint32_t row = 1; row < pOutMatrix->height -1; row++) {
        uint32_t col_length = pOutMatrix->height;
        pOutMatrix->data[col_length * row] = pOutMatrix->data[col_length * row +1]; // left border
        pOutMatrix->data[(col_length * row) + pOutMatrix->width -1] = pOutMatrix->data[(col_length * row) + pOutMatrix->width -2]; // right border  POSSIBLE LEAK: may destroy locality????

    }


    // corners: extends inner corners
    pOutMatrix->data[0] = pOutMatrix->data[pOutMatrix->width + 1]; // top-left
    pOutMatrix->data[pOutMatrix->width-1] = pOutMatrix->data[pOutMatrix->width - 2]; // top-right
    pOutMatrix->data[pOutMatrix->width * (pOutMatrix->height-1)] = pOutMatrix->data[pOutMatrix->width * (pOutMatrix->height-1) + 1];
    pOutMatrix->data[pOutMatrix->width * (pOutMatrix->height-1) + pOutMatrix->width -1] = pOutMatrix->data[pOutMatrix->width * (pOutMatrix->height-1) + pOutMatrix->width -2];


    return 0;

}

/* Works on GreyScale images */







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