#include "sobel.h"
#include "common.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {

    //Mat img = imread("bovino.jpg", IMREAD_COLOR);
    Mat img = imread("./img/8k/b.jpg" , IMREAD_GRAYSCALE);

    image *pInImage , InImage = IMAGE_INITIALIZER;
    image *pOutImage, OutImage = IMAGE_INITIALIZER;

    // populate InputImage structure with width, height and data
    pInImage = &InImage;
    pOutImage = &OutImage;

    pInImage->width = img.cols;
    pInImage->height = img.rows;
    pInImage->data = img.data;


    sobel(pInImage,pOutImage);


    if(img.data == nullptr)
        cerr <<"Error loading the image\n";

    namedWindow("Input image", WINDOW_NORMAL);
    imshow("Input image", img);
    waitKey(0);

    Mat out = Mat(pOutImage->height, pOutImage->width, CV_8UC1, pOutImage->data);

    namedWindow("Output image", WINDOW_NORMAL);
    imshow("Output image", out);
    waitKey(0);
/*
    std::cout << img << endl;
    std::cout << out << endl;
*/
    return 0;
}

