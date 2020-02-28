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
//#include "TandS.cpp"

std::queue<int> *input = new std::queue<int>;
bool exitFlag = false;
sem_t* emptyCount = new sem_t;
sem_t* fullCount = new sem_t;
sem_t* queue_lock = new sem_t;
clock_t start;
bool  * emptyQueue = new bool(false);
//std::mutex producer_lock; // might not need this

struct input_information_struct{
    bool isInputFromFile;
    char * fileName;
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
    long tics_per_second = sysconf(_SC_CLK_TCK);
    struct tms time;
    clock_t end = times(&time);
    clock_t total_t = ((double)(end - start))/tics_per_second;
    printf("%lu\n", total_t);
    std::cout << total_t << std::endl;
    std::cout << "time here";
    std::cout << "ID= " << id << " ";
    if (state == "Work" or state == "Recieve"){
        //int *sval;
        //sem_getvalue(fullCount, sval);
        std::cout << "queue here";
    }else{
        std::cout << " no queue here";
    }
    std::cout << state << " ";
    if (work != -1){
        std::cout << work << std::endl;
    }
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
                printLogs(0, "End", -1);
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
            printLogs(0, "Sleep", inputParameterTS);
//            Sleep(inputParameterTS);
        }else if (orTS == 'T'){
            sem_wait(emptyCount);
            sem_wait(queue_lock);
            printLogs(0, "Work", inputParameterTS);
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
    int value;
    int * ID = (int *) ID_args;
    while (!exitFlag && !*emptyQueue){
        //printLogs(* ID, "ASK", -1);
       
        sem_wait(fullCount);
        //sem_wait(fullCount);
        sem_wait(queue_lock);
        if(input->size() == 0){
            *emptyQueue = true;
            std::cout << "the value has changed: " << emptyQueue << std::endl;
        }else{
            //work in here
            input->pop();
        }
        int error = sem_getvalue(fullCount, &value);
        
        std::cout << "the eorro is: " << error << std::endl;
        std::cout << "the value is :" << value;
        printLogs(*ID, "WORKING", 10);
        sleep(1.3);
        sem_post(queue_lock);
        sem_post(emptyCount);
    }
    return (void *) ID_args;
}


int main(int argc, char* argv[]){
    //printHeader();s
    input_information.isInputFromFile = true;
    struct tms time;
    start = times(&time);
    int number_consumer = 10;
//    std::cout << "I am here" << std::endl;
    //std::vector<pthread_t> ThreadVector;
    pthread_t ThreadVector[number_consumer];
    int IDarray[number_consumer];
    sem_init(fullCount, 0, 0);
    int value;
    sem_getvalue(fullCount, &value);
    std::cout << "The calue of FullCount should be 5: " << value;
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
    for (auto consumer_thread : ThreadVector){
        std::cout << "waiting" << std::endl;
        pthread_join(consumer_thread, NULL);
        std::cout << std::endl << "done" << std::endl;
        sem_post(fullCount);
    }
    //closing and terminating the locks
    return 0;
}


//
//void consumerExecuteFUCK(int id){
//    // from the queue which is a pair execute T or S and exit when exit flag is raised only when the queue is empty
//    while(true){
//        if (exitFlag){
//            //make sure to check if the queue is empty
//        }else{
//            std::cout << "ID: " << id;
//            //printLogs(id, "Ask", -1);
//            sem_wait(fullCount);
//            queue_lock->lock();
//            // do the shit here
//            int executeInput =  input->front();
//            input->pop();
//            printLogs(id, "Recieve", executeInput);
////            Trans(executeInput.second); //execute T
//            printLogs(id, "Complete", executeInput);
//            queue_lock->unlock();
//            sem_post(emptyCount);
//        }
//    }
//}
