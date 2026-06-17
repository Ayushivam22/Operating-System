#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <chrono>

using namespace std;

// run command : g++ counting_semaphore.cpp -std=c++20

counting_semaphore<3> sem(3);

mutex m;

void worker(int id)
{
    {
        lock_guard<mutex> lock(m);
        cout << "\nThread " << id << " waiting...\n";
    }

    sem.acquire();

    {
        lock_guard<mutex> lock(m);
        cout << "\nThread " << id << " entered CS\n";
    }

    this_thread::sleep_for(chrono::seconds(3));

    {
        lock_guard<mutex> lock(m);
        cout << "\nThread " << id << " leaving CS\n";
    }

    sem.release();
}

int main()
{
    vector<thread> threads;

    for (int i = 1; i <= 10; i++)
    {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads)
        t.join();

    return 0;
}