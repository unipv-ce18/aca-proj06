//
// Created by marco on 07/01/19.
//

#ifndef ACAPROJECT_COMMON_H
#define ACAPROJECT_COMMON_H

#include <cmath>

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

/* Return the norm2 of a vector */
static inline int16_t norm2(int16_t x, int16_t y)
{
    return sqrt(x*x + y*y);
}

#endif //ACAPROJECT_COMMON_H
