#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

using namespace std;


// run command : g++ semaphore.cpp -std=c++20

binary_semaphore sem(1);

int cnt = 0;
void task(){
    for(int i = 0; i < 100000; i++){
        sem.acquire();
        cnt++;
        sem.release();
    }
}

int main()
{
    thread t1(task);
    thread t2(task);

    cout << "\nStarting threads...\n";
    t1.join();
    t2.join();
    cout << cnt <<'\n';
}