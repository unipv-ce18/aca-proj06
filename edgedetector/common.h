//
// Created by marco on 07/01/19.
//

#ifndef ACAPROJECT_COMMON_H
#define ACAPROJECT_COMMON_H

enum CHUNK_TYPE {
    NONE,
    VERTICAL,
    HORIZONTAL,
    BLOCKS
};

typedef struct bloccoH {
    int startRow;
    int endRow;
    int altezza;
    int larghezza;
};

typedef struct bloccoV {
    int startCol;
    int endCol;
    int altezza;
    int larghezza;
};

typedef struct cannyKernel {
    int size;
    double sigma;
};
#endif //ACAPROJECT_COMMON_H
