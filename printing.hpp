//
//  printing.hpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-29.
//

#ifndef printing_hpp
#define printing_hpp

#include <stdio.h>
#include <sys/times.h> // for timing functions
#include <thread>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <mutex> // for binary lock
#include <semaphore.h> // for sem functions
#include <fcntl.h> // for O_CREAT | O_EXCL
#include <utility> //for pair
#include <queue>
#include <string> // std::stoi
#include <stdio.h> // for file reading and writing
#include <fstream> // for file reading and writing
#include <cstring>
#include <chrono>
#include <iomanip>


struct summaryStruct{
    int workCount;
    int askCount;
    int receiveCount;
    int completeCount;
    int sleepCount;
    std::chrono::duration<double> totalTime;
    std::vector<int> *threadWorkCount = new std::vector<int>;
};



void printFooter();

void updateAsk();

void updateReceive();

void updateComplete();

void updateSleep();

void updateTotalTime(std::chrono::duration<double> updateTime);

void updateWork();

void updateWorkCount(int ID);

void fillWorkCount(int num);

void printLogs(int id, std::string state, int work);


#endif /* printing_hpp */
