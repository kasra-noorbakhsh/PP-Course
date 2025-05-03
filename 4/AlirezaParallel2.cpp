#include <iostream>
#include <Windows.h>
#include <omp.h>

using namespace std;

int m, n, k;
int num_solution = 0;
LARGE_INTEGER frequency;
LARGE_INTEGER start, finish;
double elapsedTime;

void makeBoard(char** board) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = '_';
        }
    }
}

void displayBoard(char** board) {
    string output = "";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            output += " " + string(1, board[i][j]) + " ";
        }
        output += "\n";
    }
    output += "\n";
    cout << output;
}

void attack(int i, int j, char a, char** board)
{
    if ((i + 2) < m && (j - 1) >= 0) {
        board[i + 2][j - 1] = a;
    }
    if ((i - 2) >= 0 && (j - 1) >= 0) {
        board[i - 2][j - 1] = a;
    }
    if ((i + 2) < m && (j + 1) < n) {
        board[i + 2][j + 1] = a;
    }
    if ((i - 2) >= 0 && (j + 1) < n) {
        board[i - 2][j + 1] = a;
    }
    if ((i + 1) < m && (j + 2) < n) {
        board[i + 1][j + 2] = a;
    }
    if ((i - 1) >= 0 && (j + 2) < n) {
        board[i - 1][j + 2] = a;
    }
    if ((i + 1) < m && (j - 2) >= 0) {
        board[i + 1][j - 2] = a;
    }
    if ((i - 1) >= 0 && (j - 2) >= 0) {
        board[i - 1][j - 2] = a;
    }
}

bool canPlace(int i, int j, char** board)
{
    if (board[i][j] == '_')
        return true;
    else
        return false;
}

void place(int i, int j, char k, char a, char** board, char** new_board) {
    for (int y = 0; y < m; y++) {
        for (int z = 0; z < n; z++) {
            new_board[y][z] = board[y][z];
        }
    }
    new_board[i][j] = k;
    attack(i, j, a, new_board);
}

void kkn(int k, int sti, int stj, char** board) {
    if (k == 0) {
        displayBoard(board);
        num_solution++;
    }
    else {
        for (int i = sti; i < m; i++) {
            for (int j = (i == sti ? stj : 0); j < n; j++) {
                if (canPlace(i, j, board)) {
                    char** new_board = new char* [m];
                    for (int x = 0; x < m; x++) {
                        new_board[x] = new char[n];
                        for (int y = 0; y < n; y++) {
                            new_board[x][y] = board[x][y];
                        }
                    }
                    place(i, j, 'K', 'A', board, new_board);
                    #pragma omp task 
                    {
                        kkn(k - 1, i, j, new_board);
                        for (int x = 0; x < m; x++) {
                            delete[] new_board[x];
                        }
                        delete[] new_board;
                    }
                }
            }
        }
    }
}

int main() {
    m = 4, n = 4, k = 3;
    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&start);

    char** board = new char* [m];
    for (int i = 0; i < m; i++) {
        board[i] = new char[n];
        for (int j = 0; j < n; j++) {
            board[i][j] = '_';
        }
    }

    #pragma omp parallel 
    {
        #pragma omp single nowait
        kkn(k, 0, 0, board);
    }

    QueryPerformanceCounter(&finish);

    elapsedTime = static_cast<double>(finish.QuadPart - start.QuadPart) / frequency.QuadPart;
    cout << "Elapsed time: " << elapsedTime << " seconds " << " Total number of solutions: " << num_solution << endl;

    for (int i = 0; i < m; i++) {
        delete[] board[i];
    }
    delete[] board;

    return 0;
}
