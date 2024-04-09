#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

const int grid_size = 50;
const int max_iterations = 50;

void printGrid(const vector<vector<int>>& grid) {
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            if (grid[i][j] == 1) {
                cout << "*";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int countNeighbors(const vector<vector<int>>& grid, int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int ni = (row + i + grid_size) % grid_size;
            int nj = (col + j + grid_size) % grid_size;
            count += grid[ni][nj];
        }
    }
    return count;
}

int main() {
    vector<vector<int>> current_grid(grid_size, vector<int>(grid_size, 0));
    vector<vector<int>> next_grid(grid_size, vector<int>(grid_size, 0));

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            current_grid[i][j] = rand() % 2;
        }
    }

    auto start_time = chrono::high_resolution_clock::now();

    for (int iter = 0; iter < max_iterations; ++iter) {
#pragma omp parallel for
        for (int i = 0; i < grid_size; ++i) {
            for (int j = 0; j < grid_size; ++j) {
                int neighbors = countNeighbors(current_grid, i, j);
                if (current_grid[i][j] == 1) {
                    if (neighbors == 2 || neighbors == 3) {
                        next_grid[i][j] = 1;
                    }
                    else {
                        next_grid[i][j] = 0;
                    }
                }
                else {
                    if (neighbors == 3) {
                        next_grid[i][j] = 1;
                    }
                    else {
                        next_grid[i][j] = 0;
                    }
                }
            }
        }

#pragma omp parallel for
        for (int i = 0; i < grid_size; ++i) {
            for (int j = 0; j < grid_size; ++j) {
                current_grid[i][j] = next_grid[i][j];
            }
        }

        cout << "Generation: " << iter + 1 << endl;
        printGrid(current_grid);
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << "Simulation completed in " << duration << " milliseconds." << endl;

    return 0;
}