//
// Created by marco on 07/01/19.
//

#ifndef ACAPROJECT_COMMON_H
#define ACAPROJECT_COMMON_H

#include <cmath>
#include <cstdlib>
#include <cstdint>


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

static inline kernel_t initKernelX (kernel_t kernelX) {
    kernelX[0][0] = -1;
    kernelX[0][1] = 0;
    kernelX[0][2] = 1;

    kernelX[1][0] = -2;
    kernelX[1][1] = 0;
    kernelX[1][2] = 2;

    kernelX[2][0] = -1;
    kernelX[2][1] = 0;
    kernelX[2][2] = 1;

    return kernelX;

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

/* Return the norm2 of a vector */
static inline int16_t norm2(int16_t x, int16_t y)
{
    return sqrt(x*x + y*y);
}

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
    }

    normalize_matrix_to_image(&unNormalizedGradient,pOutImage);

}



#define print_mat(pMat) \
do { \
        printf("Image width: %d, height: %d\n", (pMat)->width, (pMat)->height); \
        for (uint32_t i = 0; i < (pMat)->height; i++) {  \
                for (uint32_t j = 0; j < (pMat)->width; j++) { \
                        printf("%d ", (pMat)->data[i*(pMat)->width + j]); \
                } \
                puts(""); \
        } \
} while (0)



#endif //ACAPROJECT_COMMON_H
