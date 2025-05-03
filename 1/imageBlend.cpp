#include <iostream>
#include <opencv2/opencv.hpp>
#include <intrin.h>
#include "ipp.h"

using namespace cv;
using namespace std;

void blendImagesSerial(const Mat& frontROI, const Mat& logoImage, Mat& blendedROI, float alpha) {
    for (int y = 0; y < frontROI.rows; ++y) {
        const Vec3b* frontRow = frontROI.ptr<Vec3b>(y);
        const Vec3b* logoRow = logoImage.ptr<Vec3b>(y);
        Vec3b* blendedRow = blendedROI.ptr<Vec3b>(y);

        for (int x = 0; x < frontROI.cols; ++x) {
            const Vec3b& frontPixel = frontRow[x];
            const Vec3b& logoPixel = logoRow[x];
            Vec3b& blendedPixel = blendedRow[x];

            for (int c = 0; c < 3; ++c) {
                float frontVal = static_cast<float>(frontPixel[c]);
                float logoVal = static_cast<float>(logoPixel[c]);
                float blendedVal = frontVal + logoVal * alpha;

                blendedPixel[c] = static_cast<uchar>(min(max(blendedVal, 0.0f), 255.0f));
            }
        }
    }
}

void blendImagesSSE(const Mat& frontROI, const Mat& logoImage, Mat& blendedROI, float alpha) {
    __m128i zero = _mm_setzero_si128();
    __m128 alpha_val = _mm_set1_ps(alpha);
    __m128 all255 = _mm_set1_ps(255.0f);
    __m128 allFloatZero = _mm_set1_ps(0.0f);
    uchar** alphaAppliedLogo = new uchar * [logoImage.rows]; 

    for (int i = 0; i < logoImage.rows; ++i) {
        alphaAppliedLogo[i] = new uchar[logoImage.cols * 3]; 
    }

    for (int y = 0; y < logoImage.rows; ++y) {
        const uchar* logoRow = logoImage.ptr<uchar>(y);
        uchar* alphaAppliedRow = alphaAppliedLogo[y];

        int x = 0;
        for (; x <= (logoImage.cols * 3) - 4; x += 4) {
            __m128i logoPixel = _mm_loadu_si128(reinterpret_cast<const __m128i*>(& logoRow[x]));
            __m128i logoLo = _mm_unpacklo_epi8(logoPixel, zero);
            __m128 logoFloat = _mm_cvtepi32_ps(_mm_unpacklo_epi16(logoLo, zero));
            __m128 alphaAppliedVec = _mm_mul_ps(logoFloat, alpha_val);

            __m128i alphaAppliedVecInt = _mm_cvtps_epi32(alphaAppliedVec);

            __m128i alphaAppliedVecChar = _mm_packus_epi16(_mm_packs_epi32(alphaAppliedVecInt, zero), zero);
            *(int*)&alphaAppliedRow[x] = _mm_cvtsi128_si32(alphaAppliedVecChar);
        }

        for (; x < logoImage.cols * 3; x++) {
            char alphaAppliedPixel = logoRow[x] * alpha;
            alphaAppliedRow[x] = alphaAppliedPixel;
        }
    }

    for (int y = 0; y < logoImage.rows; ++y) {
        const uchar* frontRow = frontROI.ptr<uchar>(y);
        const uchar* logoRow = alphaAppliedLogo[y];
        uchar* blendedRow = blendedROI.ptr<uchar>(y);

        int x = 0;
        for (; x <= (logoImage.cols * 3) - 16; x += 16) {
            __m128i frontPixel = _mm_load_si128(reinterpret_cast<const __m128i*>(&frontRow[x]));
            __m128i logoPixel = _mm_load_si128(reinterpret_cast<const __m128i*>(&logoRow[x]));

            // Convert 8-bit integers to 16-bit integers
            __m128i frontPixelLo = _mm_unpacklo_epi8(frontPixel, _mm_setzero_si128());
            __m128i frontPixelHi = _mm_unpackhi_epi8(frontPixel, _mm_setzero_si128());
            __m128i logoPixelLo = _mm_unpacklo_epi8(logoPixel, _mm_setzero_si128());
            __m128i logoPixelHi = _mm_unpackhi_epi8(logoPixel, _mm_setzero_si128());

            __m128i blendIntLo = _mm_adds_epu16(logoPixelLo, frontPixelLo);
            __m128i blendIntHi = _mm_adds_epu16(logoPixelHi, frontPixelHi);

            __m128i blendedPixel = _mm_packus_epi16(blendIntLo, blendIntHi);

            _mm_store_si128(reinterpret_cast<__m128i*>(&blendedRow[x]), blendedPixel);
        }

        for (; x < logoImage.cols * 3; x++) {
            char blendedPixel = frontRow[x] + logoRow[x];
            blendedRow[x] = blendedPixel;
        }
    }
}

int main() {
   Ipp64u serialStart, serialEnd, serialTime;
   Ipp64u parallelStart, parallelEnd, parallelTime;

   Mat frontImage = imread("front.png", IMREAD_COLOR);
   Mat logoImage = imread("logo.png", IMREAD_COLOR);

   if (frontImage.empty() || logoImage.empty()) {
       cerr << "Error: Could not load one or both images!" << endl;
       return -1;
   }

   int xOffset = 0; 
   int yOffset = 0;

   if (xOffset + logoImage.cols > frontImage.cols || yOffset + logoImage.rows > frontImage.rows) {
       cerr << "Error: Logo image exceeds the boundaries of the front image!" << endl;
       return -1;
   }

   Mat serialBlendedImage = frontImage.clone();
   Mat serialROI = serialBlendedImage(Rect(xOffset, yOffset, logoImage.cols, logoImage.rows));

   serialStart = ippGetCpuClocks();
   blendImagesSerial(serialROI, logoImage, serialROI, 0.625f);
   serialEnd = ippGetCpuClocks();
   serialTime = serialEnd - serialStart;

   Mat parallelBlendedImage = frontImage.clone();
   Mat parallelROI = parallelBlendedImage(Rect(xOffset, yOffset, logoImage.cols, logoImage.rows));

   parallelStart = ippGetCpuClocks();
   blendImagesSSE(parallelROI, logoImage, parallelROI, 0.625f);
   parallelEnd = ippGetCpuClocks();
   parallelTime = parallelEnd - parallelStart;

   namedWindow("Serial Blended Image", WINDOW_AUTOSIZE);
   imshow("Serial Blended Image", serialBlendedImage);

   namedWindow("Parallel Blended Image", WINDOW_AUTOSIZE);
   imshow("Parallel Blended Image", parallelBlendedImage);

   waitKey(0);

   cout << "Serial Run time = " << (Ipp32s)serialTime << " cycles" << endl;
   cout << "Parallel Run time = " << (Ipp32s)parallelTime << " cycles" << endl;
   cout << "Speedup = " << (float)(serialTime) / (float)parallelTime << endl;

   return 0;
}
