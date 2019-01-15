#include "sobel_seq.h"
#include "omp_sobel.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


int main(int argc, char *argv[]) {

    double start, end;

    //Mat img = imread("bovino.jpg", IMREAD_COLOR);
    cv::Mat img = imread("./img/lena.png" , cv::IMREAD_GRAYSCALE);

    if(img.data == nullptr)
        std::cerr <<"Error loading the image\n";

    cv::Mat out = img.clone();

    start = omp_get_wtime();

    out = Sobel(img,0,0,img.cols,img.rows);

    end = omp_get_wtime();


    double duration_msec = double(end-start) * 1000;

    std::cout << "Execution time (ms): " << duration_msec << std::endl;


    double omp_start = omp_get_wtime();

    cv::Mat outParallel = ompSobel(img,HORIZONTAL,4,4);

    double omp_end = omp_get_wtime();

    duration_msec = double(omp_end-omp_start) * 1000;

    std::cout << "Parallel Execution time (ms): " << duration_msec << std::endl;

    cv::namedWindow("Input image", CV_WINDOW_NORMAL);
  //  cv::resizeWindow("Input image",1200,1200);
    imshow("Input image", img);


    cv::namedWindow("Output blocks image", CV_WINDOW_NORMAL);
   // cv::resizeWindow("Output image",1200,1200);
    imshow("Output blocks image", outParallel);
    cv::waitKey(0);

    return 0;
}

