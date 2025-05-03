#include <iostream>
#include <opencv2/opencv.hpp>
#include <intrin.h>
#include "ipp.h"

using namespace cv;
using namespace std;

void serialMotionDetector(const string& videoPath, Ipp64u& serialTime) {
    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error opening video file." << endl;
        return;
    }

    Mat prevFrame, currentFrame, motionFrame;

    cap >> prevFrame;
    if (prevFrame.empty()) {
        cerr << "Error reading the first frame." << endl;
        return;
    }

    cvtColor(prevFrame, prevFrame, COLOR_BGR2GRAY);

    namedWindow("Original Video", WINDOW_NORMAL);
    namedWindow("Serial Motion Detected", WINDOW_NORMAL);

    serialTime = 0;

    while (true) {
        cap >> currentFrame;
        if (currentFrame.empty()) {
            break;
        }

        Mat grayFrame;
        cvtColor(currentFrame, grayFrame, COLOR_BGR2GRAY);
        motionFrame = Mat::zeros(grayFrame.size(), CV_8UC1);

        int rows = grayFrame.rows;
        int cols = grayFrame.cols;

        Ipp64u start = ippGetCpuClocks();
        for (int i = 0; i < rows; i++) {
            uchar* prevRow = prevFrame.ptr<uchar>(i);
            uchar* currRow = grayFrame.ptr<uchar>(i);
            uchar* motionRow = motionFrame.ptr<uchar>(i);

            for (int j = 0; j < cols; j++) {
                int diff = abs(currRow[j] - prevRow[j]);
                motionRow[j] = (diff > 30) ? 255 : 0;
            }
        }
        Ipp64u end = ippGetCpuClocks();
        serialTime += (end - start);

        imshow("Original Video", currentFrame);
        imshow("Serial Motion Detected", motionFrame);

        prevFrame = grayFrame.clone();

        if (waitKey(30) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
}

void parallelMotionDetector(const string& videoPath, Ipp64u& parallelTime) {
    __m128i all30 = _mm_set1_epi8(30);
    __m128i all255 = _mm_set1_epi8(255);

    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error opening video file." << endl;
        return;
    }

    Mat prevFrame, currentFrame, motionFrame;

    cap >> prevFrame;
    if (prevFrame.empty()) {
        cerr << "Error reading the first frame." << endl;
        return;
    }

    cvtColor(prevFrame, prevFrame, COLOR_BGR2GRAY);

    namedWindow("Original Video", WINDOW_NORMAL);
    namedWindow("Parallel Motion Detected", WINDOW_NORMAL);

    parallelTime = 0;

    while (true) {
        cap >> currentFrame;
        if (currentFrame.empty()) {
            break;
        }

        Mat grayFrame;
        cvtColor(currentFrame, grayFrame, COLOR_BGR2GRAY);

        motionFrame = Mat::zeros(grayFrame.size(), CV_8UC1);

        int rows = grayFrame.rows;
        int cols = grayFrame.cols;

        Ipp64u start = ippGetCpuClocks();
        for (int i = 0; i < rows; i++) {
            uchar* prevRow = prevFrame.ptr<uchar>(i);
            uchar* currRow = grayFrame.ptr<uchar>(i);
            uchar* motionRow = motionFrame.ptr<uchar>(i);

            for (int j = 0; j < cols; j += 16) {
                __m128i prevPixels = _mm_load_si128((__m128i*) & prevRow[j]);
                __m128i currPixels = _mm_load_si128((__m128i*) & currRow[j]);

                __m128i diff = _mm_abs_epi8(_mm_sub_epi8(currPixels, prevPixels));

                __m128i motionPixels = _mm_cmpgt_epi8(diff, all30);

                _mm_storeu_si128((__m128i*) & motionRow[j], motionPixels);
            }
        }
        Ipp64u end = ippGetCpuClocks();
        parallelTime += (end - start);

        imshow("Original Video", currentFrame);
        imshow("Parallel Motion Detected", motionFrame);

        prevFrame = grayFrame.clone();

        if (waitKey(30) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
}

int main() {
   Ipp64u serialTime, parallelTime;

   string videoPath = "Q4.mp4";

   serialMotionDetector(videoPath, serialTime);
   parallelMotionDetector(videoPath, parallelTime);

   cout << "Serial Run time = " << serialTime << " cycles" << endl;
   cout << "Parallel Run time = " << parallelTime << " cycles" << endl;
   cout << "Speedup = " << static_cast<float>(serialTime) / static_cast<float>(parallelTime) << endl;

   return 0;
}
