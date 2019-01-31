//
// Created by marco on 21/01/19.
//
#include "../edge_detector.h"
#include <vector>

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;


Matrix getGaussian(int size, double sigma);
void print(std::vector<Array> const &input);
cv::Mat imageConvolution(cv::Mat inputImage, Matrix kernel_t);


cv::Mat edgeDetector::Canny(cv::Mat inputImage, cannyKernel kernel_t ) {

    Matrix gaussianKernel = getGaussian(kernel_t.size, kernel_t.sigma);
    cv::Mat blurred = imageConvolution(inputImage,gaussianKernel);
    blurred.convertTo(blurred,CV_8UC1,255,0);
    cv::Mat output = edgeDetector::Sobel(blurred);
    return output;

}

void print(std::vector<Array> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        for (int j=0; j < input.size(); j++) {
            std::cout << input[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

Matrix getGaussian(int size, double sigma) {
    Matrix kernel(size, Array(size));
    double sum=0.0;
    int i,j;

    int low = (-1)* floor(size/2);
    int high = floor(size/2);
    int magnitude = 1;

    for (i=low ; i<=high ; i++) {
        for (j=low ; j<=high ; j++) {
            kernel[i+high][j+high] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[i+high][j+high];
        }
    }


    // normalize values
    for (i=0 ; i<size ; i++) {
        for (j=0 ; j<size ; j++) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}


cv::Mat imageConvolution(cv::Mat inputImage, Matrix kernel_t) {

    cv::Mat outputImage = cv::Mat(cv::Size(inputImage.rows, inputImage.cols), CV_64F,cv::Scalar(0));

    inputImage.convertTo(inputImage,CV_64F, 1,0);
    double size = kernel_t.size();

    double max =0;
    for (int i=0 ; i<inputImage.rows ; i++) {
        for (int j=0 ; j<inputImage.cols ; j++) {
            if (max < inputImage.at<double>(i,j) )
                max = inputImage.at<double>(i,j) ;
        }
    }

    inputImage /= max;

    int offset = floor(size / 2);
    for (int i = offset; i < inputImage.rows -offset ; i++) {
        for (int j = offset; j < inputImage.cols -offset ; j++) {

            double convolution_sum = 0;

            for (int k=-offset; k<= offset; k++) {
                for (int l=-offset; l <= offset; l++) {
                    convolution_sum += kernel_t[k+offset][l+offset] * inputImage.at<double>(i+k, j+l);
                }
            }

            outputImage.at<double>(i,j) = convolution_sum;
        }
    }

    return outputImage;
}