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

struct input_information_struct{
    bool isInputFromFile;
    char * fileName;
    int outputName;
};

input_information_struct input_information;

void printHeader(){
    //print logs header
    std::cout << "";
}

void printLogs(int id, std::string state, int work){
    //timing shit here
    //https://stackoverflow.com/questions/1083142/what-s-the-correct-way-to-use-printf-to-print-a-clock-t
    //print the best way
    //-1 for work is not to print
    std::string outputFile = std::to_string(input_information.outputName);
    outputFile = outputFile + ".log";
    std::ofstream out(outputFile);
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    
    auto t2 = std::chrono::high_resolution_clock::now();
    //printf("%lu\n", total_t);
    //std::cout << total_t << std::endl;
    std::chrono::duration<double> elapsed = t2 - start_time;
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
        std::cout << state << " " << work << std::endl;
    }else{
        std::cout << state << " " << std::endl;
    }
    std::cout.rdbuf(coutbuf); //https://stackoverflow.com/a/10151286
}

void *producerExecute(void *){
    //read from the file or stdin as specified
    //exit and raise exit flag
    std::string readInput;
    std::ifstream inputFile;
    if (input_information.isInputFromFile){
        inputFile.open("input.txt");
    }
    while(!exitFlag){
        
        if (input_information.isInputFromFile){  //read from file
            if (inputFile.eof()){
                exitFlag = true;
                sem_wait(printLock);
                printLogs(0, "End", -1);
                sem_post(printLock);
                break;
            }
             std::getline(inputFile, readInput);
        }else{
            std::getline(std::cin, readInput);
        }
        //be careful string -> char
        char orTS = readInput[0];
        //https://stackoverflow.com/questions/21976936/convert-string-element-to-integer-c
        int inputParameterTS = readInput[1] - '0';  // convert string element to int
        if (orTS == 'S'){
            sem_wait(printLock);
            printLogs(0, "Sleep", inputParameterTS);
            sem_post(printLock);
            Sleep(inputParameterTS);
        }else if (orTS == 'T'){
            sem_wait(emptyCount);
            sem_wait(queue_lock);
            sem_wait(printLock);
            printLogs(0, "Work", inputParameterTS);
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
    while (!exitFlag && !*emptyQueue){
        sem_wait(printLock);
        printLogs(*ID, "Ask", -1);
        sem_post(printLock);
        sem_wait(fullCount);
        //sem_wait(fullCount);
        sem_wait(queue_lock);
        if(input->size() == 0){
            *emptyQueue = true;
            //std::cout << "the value has changed: " << emptyQueue << std::endl;
        }else{
            //work in here
            int inputT = input->front();
            input->pop();
            sem_wait(printLock);
            printLogs(*ID, "Recieve", 10);
            sem_post(printLock);
            Trans(inputT);
            sem_wait(printLock);
            printLogs(*ID, "Complete", 10);
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
    //printHeader();s  ss
    if (strcmp(argv[2], "<") == 0){
        //filename for output
        std::cout << "182" << std::endl;
        input_information.isInputFromFile = true;
        input_information.fileName = argv[3];
        input_information.outputName = 0;
    }else if (strcmp(argv[3], "<") == 0){
        std::cout << "187" << std::endl;
        input_information.isInputFromFile = true;
        input_information.fileName = argv[4];
        input_information.outputName = atoi(argv[2]);
    }else{
        std::cout << "192" << std::endl;
        input_information.isInputFromFile = false;
        input_information.outputName = atoi(argv[2]);
    }
    input_information.isInputFromFile = true;
    start_time = std::chrono::high_resolution_clock::now();
    int number_consumer = atoi(argv[1]);
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
        sem_post(fullCount);
    }
    //closing and terminating the locks
    return 0;
}
