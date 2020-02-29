//
//  printing.cpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-29.
//

#include "printing.hpp"

extern std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
extern std::queue<int> *input;

struct summaryStruct summary;

void printFooter(){
    //print logs header
    std::cout << "Summary:" << std::endl;
    std::cout << "   Work      " << summary.workCount << std::endl;
    std::cout << "   Ask       " << summary.askCount << std::endl;
    std::cout << "   Recieve   " << summary.receiveCount << std::endl;
    std::cout << "   Complete  " << summary.completeCount << std::endl;
    std::cout << "   Sleep     " << summary.sleepCount << std::endl;
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
    
}
