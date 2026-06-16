#include <thread>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

mutex m;

void taskA(int &cnt){
    for(int i = 0 ; i < 1000000 ;i++)
    {
        m.lock();
        cnt++;
        m.unlock();
    }
}
void taskB(int &cnt){
    for(int i = 0 ; i < 1000000 ;i++)
    {
        m.lock();
        cnt++;
        m.unlock();
    }
}
int main()
{
    int cnt = 0;
    thread t1(taskA,ref(cnt));
    thread t2(taskB,ref(cnt));

    t1.join();
    t2.join();

    cout << "Final count: " << cnt << endl;
    return 0;
}