//
//  testAndRun.cpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-24.
//

#include "testAndRun.hpp"
//
//  input.cpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-20.
//
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
#include "TandS.cpp"

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
std::queue<int> *input = new std::queue<int>;
bool exitFlag = false;
sem_t* emptyCount = new sem_t;
sem_t* fullCount = new sem_t;
sem_t* queue_lock = new sem_t;
sem_t* printLock = new sem_t;
bool  * emptyQueue = new bool(false);

//std::mutex producer_lock; // might not need this

struct summaryStruct{
    int workCount;
    int askCount;
    int receiveCount;
    int completeCount;
    int sleepCount;
    std::chrono::duration<double> totalTime;
    std::vector<int> *threadWorkCount = new std::vector<int>;
};

struct summaryStruct summary;

void printFooter(){
    //print logs header
    std::cout << "Summary:" << std::endl;
    std::cout << "   Work     " << summary.workCount << std::endl;
    std::cout << "   Ask      " << summary.askCount << std::endl;
    std::cout << "   Recieve  " << summary.receiveCount << std::endl;
    std::cout << "   Complete " << summary.completeCount << std::endl;
    std::cout << "   Sleep    " << summary.sleepCount << std::endl;
    int threadNum = 1;
    for (auto threadWork: *summary.threadWorkCount){
        std::cout << "   Thread #" << threadNum << " ";
        std::cout << threadWork << std::endl;
        threadNum++;
    }
    std::cout << "Transactions per second: " << summary.threadWorkCount->size()/summary.totalTime.count() << std::endl;
    
}

void updateAsk(){
    summary.askCount++;
}

void updateReceive(){
    summary.receiveCount++;
}

void updateComplete(){
    summary.completeCount++;
}

void updateSleep(){
    summary.sleepCount++;
}

void updateTotalTime(std::chrono::duration<double> updateTime){
    summary.totalTime += updateTime;
}

void updateWork(){
    summary.workCount++;
}

void updateWorkCount(int ID){
    int oldWork = summary.threadWorkCount->at(ID-1);
    int newWork = oldWork + 1;
    summary.threadWorkCount->at(ID-1) = newWork;
}

void fillWorkCount(int num){
    for (int i = 0; i < num; i++){
        summary.threadWorkCount->push_back(0);
    }
}

void printLogs(int id, std::string state, int work){
    //timing shit here
    //https://stackoverflow.com/questions/1083142/what-s-the-correct-way-to-use-printf-to-print-a-clock-t
    //print the best way
    //-1 for work is not to print
    
    auto t2 = std::chrono::high_resolution_clock::now();
    //printf("%lu\n", total_t);
    //std::cout << total_t << std::endl;
    std::chrono::duration<double> elapsed = t2 - start_time;
    updateTotalTime(elapsed);
    std::cout << std::fixed << std::setprecision(3) << elapsed.count();
    std::cout << "    ID= " << id << " ";
    if (state == "Work" or state == "Recieve"){
        //int *sval;
        //sem_getvalue(fullCount, sval);
        std::cout << "Q = " << input->size() << "  ";
    }else{
        std::cout << "       ";
    }
    
    if (work != -1){
        std::cout << state << "      " << work << std::endl;
    }else{
        std::cout << state << std::endl;
    }
    //std::cout.rdbuf(coutbuf); //https://stackoverflow.com/a/10151286
    
}

void *producerExecute(void *){
    //read from the file or stdin as specified
    //exit and raise exit flag
    std::string readInput;
    std::ifstream inputFile;
   
    while(!exitFlag){
        
            //read from file
        if (std::cin.eof()){
            exitFlag = true;
            sem_wait(printLock);
            printLogs(0, "End", -1);
            sem_post(printLock);
            break;
        }
        std::getline(std::cin, readInput);
        
        //be careful string -> char
        char orTS = readInput[0];
        //https://stackoverflow.com/questions/21976936/convert-string-element-to-integer-c
        int inputParameterTS = readInput[1] - '0';  // convert string element to int
        if (orTS == 'S'){
            sem_wait(printLock);
            printLogs(0, "Sleep", inputParameterTS);
            updateSleep();
            sem_post(printLock);
            Sleep(inputParameterTS);
        }else if (orTS == 'T'){
            sem_wait(emptyCount);
            sem_wait(queue_lock);
            sem_wait(printLock);
            printLogs(0, "Work", inputParameterTS);
            updateWork();
            sem_post(printLock);
            input->push(inputParameterTS);
            sem_post(queue_lock);
            sem_post(fullCount);
        }
    }
    //it only reaches here if EOF is reached and since we forcibly broke out from while loop
    // without unlocking we will have to unlock it right here
    
    return (void *) NULL;
}


void *consumerExecute(void * ID_args){
    //
    //int value;
    int * ID = (int *) ID_args;
    while (!(exitFlag && input->size() == 0)){
        sem_wait(printLock);
        printLogs(*ID, "Ask", -1);
        updateAsk();
        sem_post(printLock);
        sem_wait(fullCount);
        //sem_wait(fullCount);
        sem_wait(queue_lock);
        if(input->size() == 0){
            *emptyQueue = true;
            sem_post(fullCount);
            //std::cout << "the value has changed: " << emptyQueue << std::endl;
        }else{
            //work in here
            int inputT = input->front();
            input->pop();
            updateWorkCount(*ID);
            sem_wait(printLock);
            printLogs(*ID, "Recieve", 10);
            updateReceive();
            sem_post(printLock);
            Trans(inputT);
            sem_wait(printLock);
            printLogs(*ID, "Complete", 10);
            updateComplete();
            sem_post(printLock);
        }
        //int error = sem_getvalue(fullCount, &value);
        
        //std::cout << "the eorro is: " << error << std::endl;
        //std::cout << "the value is :" << value;
       
        //sleep(1.3);
        sem_post(queue_lock);
        sem_post(emptyCount);
    }
    return (void *) ID_args;
}


int main(int argc, char* argv[]){
    
    
    start_time = std::chrono::high_resolution_clock::now();
    int number_consumer = atoi(argv[1]);
    int outputName = atoi(argv[2]);
    std::string outputFile = std::to_string(outputName);
    outputFile = outputFile + ".log";
    const char * c = outputFile.c_str();
    freopen(c,"w",stdout);
    fillWorkCount(number_consumer);
    std::cout << "consumers" << number_consumer << std::endl;
//    std::cout << "I am here" << std::endl;
    //std::vector<pthread_t> ThreadVector;
    pthread_t ThreadVector[number_consumer];
    int IDarray[number_consumer];
    sem_init(printLock, 0, 1);
    sem_init(fullCount, 0, 0);
    sem_init(emptyCount, 0, 5);
    sem_init(queue_lock, 0, 1);
    for (int i=0; i<number_consumer; i++){
        IDarray[i] = i+1;
    }
    pthread_t parentThread;
    //parent
    pthread_create(&parentThread, NULL, producerExecute, NULL);
    
    for (int i=0; i<number_consumer; i++){
//        std::cout << "inf for loop" << std::endl;
//        std::cout << "creating new thread" << std::endl;
        pthread_create(&ThreadVector[i], NULL, consumerExecute, &IDarray[i]);
//        std::cout << "created new thread" << std::endl;
    }
    pthread_join(parentThread, NULL);
    
    sem_post(fullCount);
    for (auto consumer_thread : ThreadVector){
       std::cout << "waiting" << std::endl;
        pthread_join(consumer_thread, NULL);
        std::cout << std::endl << "done" << std::endl;
    }
    printFooter();
    fclose(stdout);
    //closing and terminating the locks
    return 0;
}
