#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;
int main(int argc, char *argv[]) {
    //Mat img = imread("bovino.jpg", IMREAD_COLOR);
    Mat img = imread("bovino.jpg", IMREAD_GRAYSCALE);
    if(img.data == nullptr)
        cerr <<"Error loading the image\n";

    namedWindow("Output image", WINDOW_NORMAL);
    imshow("Output image", img);
    waitKey(0);

    return 0;
}