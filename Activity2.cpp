#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    const int size = 100000000;
    int *v1, *v2, *v3;
    v1 = new int[size];
    v2 = new int[size];
    v3 = new int[size];

    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }
    auto stop = high_resolution_clock::now();
    cout << "Sequential execution time: "
         << duration_cast<milliseconds>(stop - start).count()
         << " ms" << endl;

    int totalSum = 0;

    const char* schedules[] = {"static", "dynamic", "guided"};
    int chunkSizes[] = {100, 1000, 10000}; 

    for (const char* scheduleType : schedules) {
        for (int chunk : chunkSizes) {
            totalSum = 0;
            start = high_resolution_clock::now();

            #pragma omp parallel for schedule(static, chunk) reduction(+:totalSum)
            for (int i = 0; i < size; i++) {
                v3[i] = v1[i] + v2[i];
                totalSum += v3[i];
            }

            stop = high_resolution_clock::now();
            cout << "Schedule: " << scheduleType << ", Chunk size: " << chunk
                 << " -> Execution time: " 
                 << duration_cast<milliseconds>(stop - start).count()
                 << " ms, Total Sum: " << totalSum << endl;
        }
    }

    delete[] v1;
    delete[] v2;
    delete[] v3;
    return 0;
}
