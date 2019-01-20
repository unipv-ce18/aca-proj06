//
// Created by marco on 10/01/19.
//

#ifndef ACAPROJECT_OMP_SOBEL_H
#define ACAPROJECT_OMP_SOBEL_H

#include "../edge_detector.h"
#include <omp.h>


cv::Mat edgeDetector::ompSobel(cv::Mat inputImage, CHUNK_TYPE chunkType, int nChunks, int nThreads) {


    cv::Mat blocksMat[nChunks];
    cv::Mat outImage; // output image

    switch (chunkType) {

        case NONE:
        {

        }

        case VERTICAL:
        {

            bloccoV blocks[nChunks]; // blocks subdivision
            for (int i = 0; i < nChunks; i++) {
                blocks[i].altezza = inputImage.rows;
                blocks[i].larghezza = inputImage.cols / nChunks;
                blocks[i].startCol = i * blocks[i].larghezza;
                blocks[i].endCol = blocks[i].startCol + blocks[i].larghezza;

                if (i == nChunks-1) {
                    // ultimo blocco
                    blocks[i].endCol = inputImage.cols;
                    blocks[i].larghezza = blocks[i].endCol - blocks[i].startCol;
                }
            }

            #pragma omp parallel for schedule(static) shared(inputImage,blocks) num_threads(nThreads)
            for (int i = 0; i < nChunks; i++) {
                cv::Mat s = Sobel(inputImage,blocks[i].larghezza * i ,0,(blocks[i].larghezza * i) + blocks[i].larghezza,blocks[i].altezza);
                blocksMat[i] = s;
            }

            cv::hconcat(blocksMat, nChunks, outImage);
            break;


        }

        case HORIZONTAL:
        {

            int altezza = inputImage.rows / nChunks;

            bloccoH blocks[nChunks]; // blocks subdivision
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

            #pragma omp parallel for schedule(static) shared(inputImage,blocks,altezza) num_threads(nThreads)
            for (int i = 0; i < nChunks; i++) {
                cv::Mat s = Sobel(inputImage,0,i*altezza,blocks[i].larghezza,i*altezza + blocks[i].altezza);
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