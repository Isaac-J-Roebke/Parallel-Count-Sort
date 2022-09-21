#include <omp.h>
#include <iostream>
#include <vector>
#include <random>
#include "CStopWatch.h"

std::random_device rd;                           // only used once to initialise (seed) engine
std::mt19937 rng(rd());                          // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(0, 100000); // guaranteed unbiased

std::vector<int> generateList(int n){

    std::vector<int> retValue;
    for (int i = 0; i < n; i++){
        retValue.push_back(uni(rng));
    }

    return retValue;
}

void countSort(std::vector<int>& a, int NumT, int chunk){  
    std::vector<int> temp;
    int n, count;

    n = a.size();
    temp.resize(n);

    #pragma omp parallel num_threads(NumT) shared(temp)
    {
        for(int i=0; i<n; i++){
            count = 0;

                #pragma omp for schedule(static, chunk)
                for(int j=0; j<n; j++){
                    if(a[j] < a[i])                { count++;}
                    else if(a[j] == a[i] && j < i) { count++;}
                }

                temp[count] = a[i];
        }
    }

    a = temp;
    temp.clear();
}

void printVector(std::vector<int> myList){

    for(int i=0; i < myList.size(); i++){
        std::cout << myList[i] << "\n";
    }
}
int main(){
    
    CStopWatch timer;
    int numThreads = 12;
    double time;
    double avgTime;

    std::cout << "n" << ", threads" << ", time" << '\n';

    for (int i = 1; i <= numThreads; i++) {
        for (int j = 10000; j <= 100000; j += 10000) {
            std::vector<int> sortList;
            avgTime = 0.0;
            for (int k=0; k < 10; k++) {
                time = 0.0;
                sortList = generateList(j);
                timer.startTimer();
                countSort(sortList, i, (j/i));
                timer.stopTimer();

                time = timer.getElapsedTime();
                avgTime += time;
            }
            avgTime = time / 10.0;

            std::cout << j << " , " << i << " , " << avgTime << '\n';
        }
    }
    
    return 0;
}