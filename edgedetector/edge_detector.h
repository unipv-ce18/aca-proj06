//
// Created by marco on 20/01/19.
//

#ifndef ACAPROJECT_EDGE_DETECTOR_H
#define ACAPROJECT_EDGE_DETECTOR_H

#include "common.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

namespace edgeDetector {


    /* SOBEL EDGE DETECTOR */
    cv::Mat Sobel(cv::Mat inputImage);
    cv::Mat Sobel(cv::Mat inputImage, int x_Left, int y_Top, int x_Right, int y_Bottom);
    cv::Mat ompSobel(cv::Mat inputImage, CHUNK_TYPE chunkType, int nChunks, int nThreads);

    /* CANNY EDGE DETECTOR */
    cv::Mat Canny(cv::Mat inputImage, cannyKernel kernel_t );

}


#endif //ACAPROJECT_EDGE_DETECTOR_H
