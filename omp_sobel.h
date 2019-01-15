//
// Created by marco on 10/01/19.
//

#ifndef ACAPROJECT_OMP_SOBEL_H
#define ACAPROJECT_OMP_SOBEL_H

#include "sobel_seq.h"
#include <omp.h>
#include <cstdlib>
#include <cstdint>
#include <iostream>

enum CHUNK_TYPE {
    NONE,
    VERTICAL,
    HORIZONTAL,
    BLOCKS
};


cv::Mat ompSobel(cv::Mat inputImage, CHUNK_TYPE chunkType, int nChunks, int nThreads) {

    blocco blocks[nChunks]; // blocks subdivision
    cv::Mat blocksMat[nChunks];
    cv::Mat outImage; // output image

    switch (chunkType) {

        case NONE:
        {

        }

        case VERTICAL:
        {

        }

        case HORIZONTAL:
        {
            for (int i = 0; i < nChunks; i++) {
                blocks[i].altezza = inputImage.rows / nChunks;
                blocks[i].larghezza = inputImage.cols;
                blocks[i].startRow = i * blocks[i].altezza;
                blocks[i].endRow = blocks[i].startRow + blocks[i].altezza;

                if (i == nChunks-1) {
                    // ultimo blocco
                    blocks[i].endRow = inputImage.rows;
                    blocks[i].altezza = blocks[i].endRow - blocks[i].startRow;
                }
            }

            #pragma omp parallel for schedule(static) shared(inputImage,blocks) num_threads(nThreads)
            for (int i = 0; i < nChunks; i++) {
                cv::Mat s = Sobel(inputImage,0,i*blocks[i].altezza,blocks[i].larghezza,i*blocks[i].altezza + blocks[i].altezza);
                blocksMat[i] = s;
            }

            cv::vconcat(blocksMat, nChunks, outImage);
            break;
        }

        case BLOCKS:
        {

        }

        default:
            std::cerr << "Invalid CHUNK_TYPE:\n {NONE, VERTICAL, HORIZONTAL, BLOCKS}" << std::endl;break;



    }

    return outImage;

}
#endif //ACAPROJECT_OMP_SOBEL_H
