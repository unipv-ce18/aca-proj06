
#include "edgedetector/edge_detector.h"

#include <dirent.h>
#include <omp.h>
#include <iostream>
#include <fstream>

#define MAX_FILES 5000

int NUM_THREADS = 2;
int NUM_CHUNKS = 2;
char *inputImgName;
char *inputImgFolder = "./test/";
cv::Mat img;

typedef struct times {
    double parallel;
    double parallelH;
    double parallelV;
};

times doSobel(const char* inputImgName, bool logging = false);
std::vector<times> readFolder();
void averageTimes(std::vector<times> totalTimes);

int main(int argc, char *argv[]) {

    if (argc > 1 && argc < 5) {
        // arguments mode
        if (argc >= 2) {
            inputImgName = argv[1];
            if (argc >= 3) {
                NUM_THREADS = atoi(argv[2]);
                if (argc == 4) {
                    NUM_CHUNKS = atoi(argv[3]);
                }
            }
        }
        else {
            std::cerr << "Usage:\nsobel input_image.jpg\nsobel input_image #num_threads\nsobel input_image #num_threads #num_chunks\n" << std::endl;
        }
    }


    std::vector<times> computedTimes = readFolder();
    averageTimes(computedTimes);




/*
    for (int nThread = 1; nThread <= 8; nThread++) {
        std::ofstream myfile;
        std::string name = "results/" + std::to_string(nThread) + ".txt";
        myfile.open (name);
        for (int nChunks = 2; nChunks <= 1024; nChunks *= 2) {

            double omp_start = omp_get_wtime();
            cv::Mat outParallel = edgeDetector::ompSobel(img,VERTICAL,nChunks,nThread);
            double omp_end = omp_get_wtime();
            double duration = (omp_end - omp_start) * 1000;

            myfile << nChunks << "\t" << duration << "\n";

        }
        myfile.close();
    }



    cannyKernel kernel_t;
    kernel_t.size = 5;
    kernel_t.sigma = 1;

    cv::Mat blurred = edgeDetector::Canny(img,kernel_t);

*/
/*
    cv::namedWindow("Input image", CV_WINDOW_NORMAL);
    imshow("Input image", img);
    cv::waitKey(0);



    cv::namedWindow("Output image", CV_WINDOW_NORMAL);
    imshow("Output image", out);
    cv::waitKey(0);

    cv::namedWindow("Output blocks image [V]", CV_WINDOW_NORMAL);
    imshow("Output blocks image [V]", outParallelV);
    cv::waitKey(0);
    */
    return 0;
}

std::vector<times> readFolder() {

    std::vector<times> totalTimes(MAX_FILES);


    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (inputImgFolder)) != NULL) {
        int i = 0;
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == 8) { // 8 stands for image
                std::string buf(inputImgFolder);
                buf.append(ent->d_name);
                times tmpTimes;
                tmpTimes = doSobel(buf.c_str(),false);

                totalTimes[i] = tmpTimes;

              //  std::cout << tmpTimes.parallel << " - " << tmpTimes.parallelH << " - " << tmpTimes.parallelV << std::endl;
                i++;
            }

        }
        closedir (dir);
        std::cout << "File analizzati " << i << std::endl;
    } else {
        /* could not open directory */
        perror ("");
    }
    return totalTimes;
}

void averageTimes(std::vector<times> totalTimes) {

    double sumParallel = 0;
    double sumParallelH = 0;
    double sumParallelV = 0;

    int counter = 0;
    for(std::vector<times>::iterator it = totalTimes.begin(); it != totalTimes.end(); ++it) {

        if (it.base()->parallel == 0 && it.base()->parallelV == 0 && it.base()->parallelH == 0) {
            continue;
        }

        sumParallel += it.base()->parallel;
        sumParallelH += it.base()->parallelH;
        sumParallelV += it.base()->parallelV;
        counter++;
        /* std::cout << *it; ... */
    }

    double avgParallel = sumParallel / counter;
    double avgParallelH = sumParallelH / counter;
    double avgParallelV = sumParallelV / counter;

    std::cout << "Number of images: " << counter << std::endl;
    std::cout << "Average parallel time: " << avgParallel << "\nAverage parallel time [H]: " << avgParallelH << "\nAverage parallel time [V]:" << avgParallelV << std::endl;

}

times doSobel(const char* InputImgName, bool logging) {

    times tmpTimes;
    double start, end;

    try {

        img = imread(InputImgName , cv::IMREAD_GRAYSCALE);

    } catch( cv::Exception& e ) {

        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;

    }

    if(img.data == nullptr)
        std::cerr <<"Error loading the image --> " << InputImgName << "\n";

    cv::Mat out = img.clone();

    start = omp_get_wtime();

    out = edgeDetector::Sobel(img);

    end = omp_get_wtime();


    double duration_msec = double(end-start) * 1000;
    tmpTimes.parallel = duration_msec;


    double omp_start = omp_get_wtime();

    cv::Mat outParallelH = edgeDetector::ompSobel(img,HORIZONTAL,NUM_CHUNKS,NUM_THREADS);

    double omp_end = omp_get_wtime();

    double parallel_duration_h_msec = double(omp_end-omp_start) * 1000;

    tmpTimes.parallelH = parallel_duration_h_msec;




    double omp_start_vertical = omp_get_wtime();

    cv::Mat outParallelV = edgeDetector::ompSobel(img,VERTICAL,NUM_CHUNKS,NUM_THREADS);

    double omp_end_vertical = omp_get_wtime();

    double parallel_duration_v_msec = double(omp_end_vertical-omp_start_vertical) * 1000;

    tmpTimes.parallelV = parallel_duration_v_msec;

    if (logging) {

        std::cout << "======== " << InputImgName << std::endl;
        std::cout << "Execution time (ms): " << duration_msec << std::endl;
        std::cout << "Parallel Execution time [HORIZONTAL] (ms): " << parallel_duration_h_msec << std::endl;
        std::cout << "SpeedUp [H]: " << (duration_msec / parallel_duration_h_msec) << std::endl;
        std::cout << "Parallel Execution time [VERTICAL] (ms): " << parallel_duration_v_msec << std::endl;
        std::cout << "SpeedUp [V]: " << (duration_msec / parallel_duration_v_msec) << std::endl;
        std::cout << "==============================================================" << std::endl;
    }
        return tmpTimes;
}