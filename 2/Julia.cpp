#include <iostream>
#include <complex>
#include <SFML/Graphics.hpp>
#include <omp.h>
#include "ipp.h"

using namespace std;
using namespace sf;

void juliaSetCalculationSerial(int width, int height, int maxIterations, complex<double> c, vector<vector<bool>>& board) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            complex<double> z(
                (x - width / 2.0) * 4.0 / width,   
                (y - height / 2.0) * 4.0 / height 
            );

            int iterations = 0;
            while (iterations < maxIterations) {
                z = z * z + c;
                if (abs(z) > 2) break;
                ++iterations;
            }

            board[x][y] = iterations == maxIterations;
        }
    }
}

void juliaSetRenderingSerial(int width, int height, vector<vector<bool>>& board, Ipp64u& time) {
    Ipp64u start, end;

    RenderWindow window(VideoMode(width, height), "Julia Set Serial");

    Image image;
    image.create(width, height, Color::Black);

    start = ippGetCpuClocks();
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (board[x][y]) {
                image.setPixel(x, y, Color::Red); 
            }
            else {
                image.setPixel(x, y, Color::Blue);
            }
        }
    }
    end = ippGetCpuClocks();
    time = (end - start);

    Texture texture;
    texture.loadFromImage(image);
    Sprite sprite(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

void juliaSetCalculationParallel(int width, int height, int maxIterations, complex<double> c, vector<vector<bool>>& board) {
    #pragma omp parallel for collapse(2) schedule(dynamic)
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                complex<double> z(
                    (x - width / 2.0) * 4.0 / width,   
                    (y - height / 2.0) * 4.0 / height 
                );

                int iterations = 0;
                while (iterations < maxIterations) {
                    z = z * z + c; 
                    if (abs(z) > 2) break;
                    ++iterations;
                }

                board[x][y] = iterations == maxIterations;
            }
        }
}

void juliaSetRenderingParallel(int width, int height, vector<vector<bool>>& board, Ipp64u& time) {
    Ipp64u start, end;

    RenderWindow window(VideoMode(width, height), "Julia Set Prallel");

    Image image;
    image.create(width, height, Color::Black);

    start = ippGetCpuClocks();
    #pragma omp parallel for num_threads(2)
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (board[x][y]) {
                    image.setPixel(x, y, Color::Red);
                }
                else {
                    image.setPixel(x, y, Color::Blue);
                }
            }
        }
    end = ippGetCpuClocks();
    time = (end - start);

    Texture texture;
    texture.loadFromImage(image);
    Sprite sprite(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

int main() {
    Ipp64u start, end, serialCalculationTime, serialRenderingTime, parallelCalculationTime, parallelRenderingTime;

    int width = 800;
    int height = 600;
    int maxIterations = 1000;

    vector<vector<bool>> board(width, vector<bool>(height));

    //complex<double> c(0.355, 0.355);
    complex<double> c(-0.5, 0.5);
    //complex<double> c(-0.7, 0.27015);

    start = ippGetCpuClocks();
    juliaSetCalculationSerial(width, height, maxIterations, c, board);
    end = ippGetCpuClocks();
    serialCalculationTime = (end - start);
    juliaSetRenderingSerial(width, height, board, serialRenderingTime);

    start = ippGetCpuClocks();
    juliaSetCalculationParallel(width, height, maxIterations, c, board);
    end = ippGetCpuClocks();
    parallelCalculationTime = (end - start);
    juliaSetRenderingParallel(width, height, board, parallelRenderingTime);

    cout << "Serial Calculation Run time = " << serialCalculationTime << " cycles" << endl;
    cout << "Parallel Calculation Run time = " << parallelCalculationTime << " cycles" << endl;
    cout << "Calculation Speedup = " << static_cast<double>(serialCalculationTime) / static_cast<double>(parallelCalculationTime) << endl << endl;

    cout << "Serial Rendering Run time = " << serialRenderingTime << " cycles" << endl;
    cout << "Parallel Rendering Run time = " << parallelRenderingTime << " cycles" << endl;
    cout << "Rendering Speedup = " << static_cast<double>(serialRenderingTime) / static_cast<double>(parallelRenderingTime) << endl << endl;

    cout << "Total Speedup = " << static_cast<double>(serialCalculationTime + serialRenderingTime) / 
                                  static_cast<double>(parallelCalculationTime + parallelRenderingTime) << endl;

    return 0;
}
