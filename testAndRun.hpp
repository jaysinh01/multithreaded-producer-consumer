//
//  testAndRun.hpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-24.
//

#ifndef testAndRun_hpp
#define testAndRun_hpp

//#include <stdio.h>
#include <sys/times.h> // for timing functions
#include <thread>
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
//#include "printing.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
std::queue<int> *input = new std::queue<int>;
bool exitFlag = false;
sem_t* emptyCount = new sem_t;
sem_t* fullCount = new sem_t;
sem_t* queue_lock = new sem_t;
sem_t* printLock = new sem_t;
bool  * emptyQueue = new bool(false);

void *consumerExecute(void * ID_args);


void *producerExecute(void *);

#endif /* testAndRun_hpp */
