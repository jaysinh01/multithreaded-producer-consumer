//
//  threading.cpp
//  multithreaded-producer-consumer
//
//  Created by Jaysinh Parmar on 2020-02-19.
//https://en.wikipedia.org/wiki/Semaphore_(programming)
//peusdo code for producer - consumer problem

#include "threading.hpp"

#include <sys/times.h> // for timing functions
#include <thread>
#include <iostream>
#include <unistd.h>
#include <mutex> // for binary lock
#include <semaphore.h> // for sem functions
#include <fcntl.h> // for O_CREAT | O_EXCL
#include <utility> //for pair
#include <queue>

/*

clock_t start;
//https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-2%3A-Counting-Semaphores
sem_t* s;

//http://www.cplusplus.com/reference/mutex/lock/
std::mutex consumer_lock;

void bar(int n){
    consumer_lock.lock();
    long tics_per_second = sysconf(_SC_CLK_TCK);
    sleep(n);
    std::cout << "im here " << n << std::endl;
    struct tms team;
    clock_t end = times(&team);
    clock_t total_t = ((double)(end - start))/tics_per_second;
    printf("%lu", total_t);
    consumer_lock.unlock();
}

void foo(int n){
    
    consumer_lock.lock();
    long tics_per_second = sysconf(_SC_CLK_TCK);
    sleep(n);
    std::cout << "im here " << n << std::endl;
    struct tms team;
    clock_t end = times(&team);
    clock_t total_t = ((double)(end - start))/tics_per_second;
    printf("%lu", total_t);
    consumer_lock.unlock();
}

int main(){
    //https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/apis/ipcsemo.htm
    s = sem_open("/empty", O_CREAT | O_EXCL, 0644, 2); // returns -1 (=FAILED) on OS X
    std::cout << s << " sema" << std::endl;
    //sem_wait(s);
    struct tms nice;
    start = times(&nice);
    //std::thread thread_obj(foo, 10);
    std::thread number2(foo, 5);
    //std::thread number3(foo, 7);
    std::vector<std::thread> ThreadVector;

    for(int i=0; i<2; i++)
    {
        ThreadVector.emplace_back([&](){foo(4);}); // Pass by reference here, make sure the object lifetime is correct

    }
    for(auto& t: ThreadVector)
    {
        t.join();

    }
    //thread_obj.join();
    number2.join();
    //number3.join();
    sem_close(s);
    sem_unlink("/empty");
    //ternimate all the threads when the queue is empty
    //have another signal that prevents
    return 0;
}
*/
