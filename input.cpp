//
//  input.cpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-20.
//

#include "input.hpp"
#include <cstring>
//#include "TandS.cpp"
/*
std::queue<std::pair<char, int>> input;
bool exitFlag = false;
sem_t* emptyCount;
sem_t* fullCount;
std::mutex queue_lock;
clock_t start;
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
    printf("%lu", total_t);
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

void producerExecute(){
    //read from the file or stdin as specified
    //exit and raise exit flag
    std::string readInput;
    std::ifstream inputFile;
    if (input_information.isInputFromFile){
        inputFile.open(input_information.fileName, std::ifstream::in);
    }
    while(!exitFlag){
        sem_wait(emptyCount);
        queue_lock.lock();
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
        std::pair <char,int> toBeInstered (orTS, inputParameterTS);
        if (orTS == 'S'){
            printLogs(0, "Sleep", inputParameterTS);
//            Sleep(inputParameterTS);
        }else if (orTS == 'T'){
            printLogs(0, "Work", inputParameterTS);
            input.push(toBeInstered);
        }
        queue_lock.unlock();
        
    }
    //it only reaches here if EOF is reached and since we forcibly broke out from while loop
    // without unlocking we will have to unlock it right here
    queue_lock.unlock();
    sem_post(fullCount);
    
}

void consumerExecute(int id){
    // from the queue which is a pair execute T or S and exit when exit flag is raised only when the queue is empty
    while(true){
        if (exitFlag){
            //make sure to check if the queue is empty
        }else{
            printLogs(id, "Ask", -1);
            sem_wait(fullCount);
            queue_lock.lock();
            // do the shit here
            std::pair <char,int> executeInput =  input.front();
            input.pop();
            printLogs(id, "Recieve", executeInput.second);
//            Trans(executeInput.second); //execute T
            printLogs(id, "Complete", executeInput.second);
            queue_lock.unlock();
            sem_post(emptyCount);
        }
    }
}


int main(int argc, char* argv[]){
    //printHeader();
    printf("hello");
    if (strcmp(argv[3], "<") == 0){
        input_information.isInputFromFile = true;
//        input_information.fileName = argv[4];
    }else{
        input_information.isInputFromFile = false;
    }
    int numConsumers = std::stoi(argv[1]);
    //queue watching semaphores
    printf("im here 132");
    emptyCount = sem_open("/empty", O_CREAT | O_EXCL, 0644, 2*numConsumers); // returns -1 (=FAILED) on OS X
    fullCount = sem_open("/empty", O_CREAT | O_EXCL, 0644, 0); // here it should not be 0
    std::thread producerThread(producerExecute);
    //create number of consumers specifed
    //https://stackoverflow.com/questions/54551371/creating-thread-inside-a-for-loop-c
    std::vector<std::thread> ThreadVector;
    for(int i=0; i<numConsumers; i++)
    {
        ThreadVector.emplace_back([&](){consumerExecute(i+1);}); // Pass by reference here, make sure the object lifetime is correct
    }
    //wait for producer
    producerThread.join();
    //wait for all the consumers to finish
    for(auto& consumerWait: ThreadVector)
    {
        consumerWait.join();
    }
    
    //closing and terminating the locks
    return 0;
}
*/
