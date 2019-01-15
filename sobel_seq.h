//
// Created by marco on 12/01/19.
//

#ifndef ACAPROJECT_SOBEL_SEQ_H
#define ACAPROJECT_SOBEL_SEQ_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <omp.h>
#define KERNEL_DIM 3


const int xKernel[KERNEL_DIM][KERNEL_DIM] = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
};

const int yKernel[KERNEL_DIM][KERNEL_DIM] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
};

typedef struct grad {
    int gradX;
    int gradY;
};

typedef struct blocco {
    int startRow;
    int endRow;
    int altezza;
    int larghezza;
};

grad gradient(cv::Mat I, int x, int y) {

    grad gradient;

    int Xgradient = 0, Ygradient = 0;

    Xgradient = xKernel[0][0] * I.at<uchar>(x-1,y-1)  +
                xKernel[0][1] * I.at<uchar>(x-1, y)   +
                xKernel[0][2] * I.at<uchar>(x-1, y+1) +
                xKernel[1][0] * I.at<uchar>(x,y-1)    +
                xKernel[1][1] * I.at<uchar>(x,y)      +
                xKernel[1][2] * I.at<uchar>(x, y+1)   +
                xKernel[2][0] * I.at<uchar>(x+1,y-1)    +
                xKernel[2][1] * I.at<uchar>(x+1,y)      +
                xKernel[2][2] * I.at<uchar>(x+1, y+1);

    gradient.gradX = Xgradient;

    Ygradient = yKernel[0][0] * I.at<uchar>(x-1,y-1)  +
                yKernel[0][1] * I.at<uchar>(x-1, y)   +
                yKernel[0][2] * I.at<uchar>(x-1, y+1) +
                yKernel[1][0] * I.at<uchar>(x,y-1)    +
                yKernel[1][1] * I.at<uchar>(x,y)      +
                yKernel[1][2] * I.at<uchar>(x, y+1)   +
                yKernel[2][0] * I.at<uchar>(x+1,y-1)    +
                yKernel[2][1] * I.at<uchar>(x+1,y)      +
                yKernel[2][2] * I.at<uchar>(x+1, y+1);


    gradient.gradY = Ygradient;

    return gradient;

}

int norm2(int x, int y) {
    return sqrt(x*x + y*y);
}

cv::Mat Sobel(cv::Mat inputImage) {

    cv::Mat outImage(cv::Size(inputImage.cols, inputImage.rows), CV_8UC1, cv::Scalar(0));
    //std::cout << "Width: " << outImage.cols << "height: " << outImage.rows << std::endl;

    // compute 3x3 convolution without 1st and last rows and 1st and last column
    // because convolution is not well defined over borders.
    for(int i=1; i < inputImage.rows ; i++) {
        for(int j=1; j < inputImage.cols -1; j++) {
            try {
            grad _gradient = gradient(inputImage,i,j);
            int gradVal = norm2( _gradient.gradX, _gradient.gradY );

            gradVal = gradVal > 255 ? 255:gradVal;
            gradVal = gradVal < 0 ? 0 : gradVal;
            gradVal = gradVal < 50 ? 0 : gradVal; // threshold

            outImage.at<uchar >(i,j) = gradVal;
            //
            } catch (const cv::Exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }


    return outImage;
}

cv::Mat Sobel(cv::Mat inputImage, int x_Left, int y_Top, int x_Right, int y_Bottom) {

    cv::Mat outImage(cv::Size((x_Right - x_Left),(y_Bottom - y_Top)), CV_8UC1, cv::Scalar(0));
    //std::cout << "Width: " << outImage.cols << "height: " << outImage.rows << std::endl;

    // compute 3x3 convolution without 1st and last rows and 1st and last column
    // because convolution is not well defined over borders.
    if (y_Top == 0){
        y_Top +=1;
        y_Bottom +=1;
    }


    for(int i=y_Top; i < y_Bottom; i++) {
        for(int j=x_Left; j < x_Right-1; j++) {
            try {
                grad _gradient = gradient(inputImage,i,j);
                int gradVal = norm2( _gradient.gradX, _gradient.gradY );

                gradVal = gradVal > 255 ? 255:gradVal;
                gradVal = gradVal < 0 ? 0 : gradVal;
                gradVal = gradVal < 50 ? 0 : gradVal; // threshold

                outImage.at<uchar >(i-y_Top,j-x_Left) = gradVal;
                //
            } catch (const cv::Exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    return outImage;
}

#endif //ACAPROJECT_SOBEL_SEQ_H
