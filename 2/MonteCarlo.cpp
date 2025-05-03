#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <random>
#include "ipp.h"

using namespace std;

double estimatePiSerial(int num_points) {
    int points_inside_circle = 0;

    mt19937 rng;
    rng.seed(time(0));
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i <= num_points; i++) {
        double x1 = dist(rng);
        double y1 = dist(rng);
        if (x1 * x1 + y1 * y1 <= 1) {
            ++points_inside_circle;
        }
    }

    return 4.0 * points_inside_circle / num_points;
}

double estimatePiParallel(int num_points) {
    int points_inside_circle = 0;

    mt19937 rng;
    rng.seed(time(0) + omp_get_thread_num());
    uniform_real_distribution<double> dist(0.0, 1.0);

    #pragma omp parallel for reduction(+:points_inside_circle)
        for (int i = 0; i <= num_points; i++) {
            double x1 = dist(rng);
            double y1 = dist(rng);
            if (x1 * x1 + y1 * y1 <= 1) {
                ++points_inside_circle;
            }
        }

    return 4.0 * points_inside_circle / num_points;
}

int main() {
    Ipp64u serialStart, serialEnd, serialTime;
    Ipp64u parallelStart, parallelEnd, parallelTime;
    const int NUM_POINTS = 100000;

    serialStart = ippGetCpuClocks();
    double pi_estimate = estimatePiSerial(NUM_POINTS);
    serialEnd = ippGetCpuClocks();
    serialTime = serialEnd - serialStart;

    parallelStart = ippGetCpuClocks();
    double pi_estimateP = estimatePiParallel(NUM_POINTS);
    parallelEnd = ippGetCpuClocks();
    parallelTime = parallelEnd - parallelStart;

    cout << "Serial Run time = " << serialTime << " cycles" << endl;
    cout << "Estimated value of pi: " << pi_estimate << endl << endl;
    cout << "Parallel Run time = " << parallelTime << " cycles" << endl;
    cout << "Estimated value of pi: " << pi_estimateP << endl << endl;
    cout << "Speedup = " << (float)(serialTime) / (float)parallelTime << endl;

    return 0;
}