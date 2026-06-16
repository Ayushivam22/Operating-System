#include <thread>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void taskA(atomic<int> &cnt)
{
    for (int i = 0; i < 1000000; i++)
    {
        cnt++;
    }
}
void taskB(atomic<int> &cnt)
{
    for (int i = 0; i < 1000000; i++)
    {
        cnt++;
    }
}
int main()
{
    atomic<int> cnt(0);
    thread t1(taskA, ref(cnt));
    thread t2(taskB, ref(cnt));

    t1.join();
    t2.join();

    cout << "Final count: " << cnt << endl;
    return 0;
}