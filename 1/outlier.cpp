#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <intrin.h>
#include "ipp.h"

using namespace std;

float calculateMean(const vector<float>& data) {
    int size = data.size();
    float sum1 = 0.0;
    float sum2 = 0.0;
    float sum3 = 0.0;
    float sum4 = 0.0;

    for (int i = 0; i < size; i += 4) {
        sum1 += data[i];
        sum2 += data[i + 1];
        sum3 += data[i + 2];
        sum4 += data[i + 3];
    }

    float sum = (sum1 + sum2) + (sum3 + sum4);
    return sum / size;
}

float calculateMeanSSE(const vector<float>& data) {
    __m128 sumVec = _mm_setzero_ps();
    int size = data.size();
    int i = 0;

    for (; i <= size - 4; i += 4) {
        __m128 dataVec = _mm_load_ps(&data[i]);
        sumVec = _mm_add_ps(sumVec, dataVec);
    }

    sumVec = _mm_hadd_ps(sumVec, sumVec);
    sumVec = _mm_hadd_ps(sumVec, sumVec);

    float sumArray[4];
    _mm_storeu_ps(sumArray, sumVec);
    float sum = sumArray[0];

    return sum / size;
}

float calculateStdDev(const vector<float>& data, float mean) {
    int size = data.size();
    float sum1 = 0.0;
    float sum2 = 0.0;
    float sum3 = 0.0;
    float sum4 = 0.0;
    float diff1;
    float diff2;
    float diff3;
    float diff4;

    for (int i = 0; i < size; i += 4) {
        diff1 = data[i] - mean;
        sum1 += diff1 * diff1;
        diff2 = data[i + 1] - mean;
        sum2 += diff2 * diff2;
        diff3 = data[i + 2] - mean;
        sum3 += diff3 * diff3;
        diff4 = data[i + 3] - mean;
        sum4 += diff4 * diff4;
    }

    float sum = (sum1 + sum2) + (sum3 + sum4);

    return sqrt(sum / size);
}

float calculateStdDevSSE(const vector<float>& data, float mean) {
    __m128 sumVec = _mm_setzero_ps();
    __m128 meanVec = _mm_set1_ps(mean);

    int size = data.size();
    int i = 0;

    for (; i <= size - 4; i += 4) {
        __m128 dataVec = _mm_load_ps(&data[i]);
        __m128 diffVec = _mm_sub_ps(dataVec, meanVec);
        __m128 sqDiffVec = _mm_mul_ps(diffVec, diffVec);
        sumVec = _mm_add_ps(sumVec, sqDiffVec);
    }

    sumVec = _mm_hadd_ps(sumVec, sumVec);
    sumVec = _mm_hadd_ps(sumVec, sumVec);

    float sumArray[4];
    _mm_storeu_ps(sumArray, sumVec);
    float sum = sumArray[0];

    return sqrt(sum / size);
}

void detectOutliers(const vector<float>& data, float mean, float stdDev, vector<float>& outliers) {
    for (float x : data) {
        float zScore = (x - mean) / stdDev;
        if (fabs(zScore) > 2.5) {
            outliers.push_back(x);
        }
    }
}

void detectOutliersSSE(const vector<float>& data, float mean, float stdDev, vector<float>& outliers) {
    const int size = data.size();
    const float threshold = 2.5f;

    vector<float> tempOutliers;
    tempOutliers.reserve(size);

    __m128 meanVec = _mm_set1_ps(mean);
    __m128 stdDevVec = _mm_set1_ps(stdDev);
    __m128 posThresholdVec = _mm_set1_ps(threshold);
    __m128 negThresholdVec = _mm_set1_ps(-threshold);

    int i = 0;
    for (; i <= size - 4; i += 4) {
        __m128 dataVec = _mm_load_ps(&data[i]);

        __m128 diffVec = _mm_sub_ps(dataVec, meanVec);
        __m128 zScoreVec = _mm_div_ps(diffVec, stdDevVec);

        __m128 maskPos = _mm_cmpgt_ps(zScoreVec, posThresholdVec);
        __m128 maskNeg = _mm_cmplt_ps(zScoreVec, negThresholdVec);

        __m128 mask = _mm_or_ps(maskPos, maskNeg);

        int maskBits = _mm_movemask_ps(mask);

        for (int j = 0; j < 4; ++j) {
            if (maskBits & (1 << j)) {
                tempOutliers.push_back(data[i + j]);
            }
        }
    }

    outliers.insert(outliers.end(), tempOutliers.begin(), tempOutliers.end());
}

int main() {
   Ipp64u serialStart, serialEnd, serialTime;
   Ipp64u parallelStart, parallelEnd, parallelTime;

   const size_t size = 1 << 20;
   vector<float> data(size);
   vector<float> outliers;
   vector<float> outliersSSE;

   random_device rd;
   mt19937 gen(rd());
   normal_distribution<> dis(0.5, 0.1);

   for (size_t i = 0; i < size; ++i) {
       data[i] = dis(gen);
   }

   serialStart = ippGetCpuClocks();
   float mean = calculateMean(data);
   float stdDev = calculateStdDev(data, mean);
   detectOutliers(data, mean, stdDev, outliers);
   serialEnd = ippGetCpuClocks();
   serialTime = serialEnd - serialStart;

   parallelStart = ippGetCpuClocks();
   float meanSSE = calculateMeanSSE(data);
   float stdDevSSE = calculateStdDevSSE(data, meanSSE);
   detectOutliersSSE(data, meanSSE, stdDevSSE, outliersSSE);
   parallelEnd = ippGetCpuClocks();
   parallelTime = parallelEnd - parallelStart;

   cout << "Serial Run time = " << (Ipp32s)serialTime << " cycles" << endl;
   cout << "Serial, Number of outliers: " << outliers.size() << endl << endl;
   cout << "Parallel Run time = " << (Ipp32s)parallelTime << " cycles" << endl;
   cout << "Parallel, Number of outliers: " << outliersSSE.size() << endl << endl;
   cout << "Speedup = " << static_cast<float>(serialTime) / static_cast<float>(parallelTime) << endl;

   return 0;
}
