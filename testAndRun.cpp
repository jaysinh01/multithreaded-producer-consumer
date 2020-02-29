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
#include <vector>
#include "TandS.cpp"
#include "printing.hpp"



    

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
            printLogs(0, "End     ", -1);
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
            printLogs(0, "Sleep   ", inputParameterTS);
            updateSleep();
            sem_post(printLock);
            Sleep(inputParameterTS);
        }else if (orTS == 'T'){
            sem_wait(emptyCount);
            sem_wait(queue_lock);
            sem_wait(printLock);
            printLogs(0, "Work    ", inputParameterTS);
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
        printLogs(*ID, "Ask     ", -1);
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
            printLogs(*ID, "Recieve ", 10);
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

    pthread_t ThreadVector[number_consumer];
    int IDarray[number_consumer];
    
    //initialize the semaphores
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
        pthread_join(consumer_thread, NULL);
    }
    printFooter();
    fclose(stdout);
    //closing and terminating the locks
    return 0;
}
