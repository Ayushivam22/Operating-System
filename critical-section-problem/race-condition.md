# Race Condition

## Definition

A **race condition** is a concurrency problem that occurs when two or more processes or threads access and modify shared data simultaneously, and the final result depends on the timing or order of their execution.

In simple words:

> A race condition occurs when multiple threads "race" to update the same shared resource without proper synchronization.

---

## Why Does a Race Condition Occur?

A race condition happens when:
1. Multiple threads share the same data.
2. At least one thread modifies the shared data.
3. There is no synchronization mechanism (mutex, lock, atomic operation, etc.).

The problem is not that multiple threads access the same variable; the problem is that operations like `counter++` are **not atomic**.

---

## Understanding `counter++`

Although `counter++` looks like a single statement, internally it is roughly equivalent to:

```text
1. Read the value from memory.
2. Add 1.
3. Write the updated value back to memory.
```

This sequence is called a **Read-Modify-Write** operation.

---

## Example of a Race Condition

Consider the following code:

```cpp
#include <iostream>
#include <thread>
using namespace std;

void increment(int &counter) {
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }
}

int main() {
    int counter = 0;

    thread t1(increment, ref(counter));
    thread t2(increment, ref(counter));

    t1.join();
    t2.join();

    cout << "Counter = " << counter << endl;
}
```

Expected output:

```text
Counter = 2000000
```

Actual output may vary:

```text
Counter = 1987421
Counter = 1999156
Counter = 1978634
```

The result depends on how the operating system schedules the threads.

---

## How Does the Race Condition Happen?

Suppose:

```text
counter = 5
```

Two threads execute `counter++` simultaneously.

| Step | Thread 1 | Thread 2 | Memory |
|------|-----------|-----------|---------|
| 1 | Read `counter` = 5 | | 5 |
| 2 | | Read `counter` = 5 | 5 |
| 3 | Add 1 → 6 | | 5 |
| 4 | | Add 1 → 6 | 5 |
| 5 | Write 6 | | 6 |
| 6 | | Write 6 | 6 |

The final value becomes **6** instead of **7**.

One increment is lost because both threads worked on the same old value.

---

## Race Condition vs Critical Section

### Critical Section
A **critical section** is the part of the program that accesses shared resources.

Example:

```cpp
counter++;
```

This line is a critical section because it modifies shared data.

### Race Condition
A **race condition** occurs when multiple threads execute the critical section simultaneously without synchronization.

| Term | Meaning |
|------|----------|
| Critical Section | Code that accesses shared data. |
| Race Condition | Incorrect behavior due to concurrent unsynchronized access to the critical section. |

---

## Why Doesn't the Bug Always Appear?

Sometimes the operating system schedules one thread to finish before the other starts:

```text
Thread 1: counter++ ... counter++ (all increments)
Thread 2: counter++ ... counter++ (all increments)
```

In this case, the final answer appears correct.

However, the program is still incorrect because the scheduler is free to interleave the instructions differently during another execution.

A race condition is defined by the **possibility** of incorrect behavior, not by whether the bug appears every time.

---

## Preventing Race Conditions

### 1. Mutex

A mutex ensures that only one thread can execute the critical section at a time.

```cpp
#include <mutex>

std::mutex m;

void increment(int &counter) {
    for (int i = 0; i < 1000000; i++) {
        std::lock_guard<std::mutex> lock(m);
        counter++;
    }
}
```

The mutex automatically locks before entering the critical section and unlocks when leaving it.

---

### 2. Atomic Variables

For simple operations like counters, `std::atomic` can be used.

```cpp
#include <atomic>

std::atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }
}
```

Atomic operations are guaranteed to execute without interference from other threads.

---

### 3. Semaphores and Monitors

Operating systems also provide higher-level synchronization mechanisms such as:
- Semaphores
- Monitors
- Condition Variables

These are commonly used when coordinating multiple threads.

---

## Real-Life Analogy

Imagine two people updating the same whiteboard.

Initial value:

```text
Counter = 5
```

Both people read the value **5** at the same time.

- Person A writes **6**.
- Person B, who also read **5**, writes **6**.

The final value is **6**, even though two increments occurred.

The problem is not that they share the whiteboard; the problem is that they update it simultaneously without coordinating.

A mutex acts like a key to the room containing the whiteboard—only one person can enter and modify it at a time.

---

## Race Condition vs Deadlock

| Feature | Race Condition | Deadlock |
|-----------|---------------|-----------|
| Cause | Unsynchronized access to shared data. | Circular waiting for resources. |
| Result | Incorrect or unpredictable output. | Threads stop making progress. |
| CPU Usage | Threads continue executing. | Threads remain blocked. |

---

## Key Points

- Race conditions occur in concurrent programs when shared data is modified without synchronization.
- Passing a variable by reference creates shared access but does not automatically make operations thread-safe.
- Operations like `counter++` are not atomic.
- Race conditions can produce different results on different executions.
- Mutexes, atomics, semaphores, and monitors are common techniques to prevent race conditions.

---

## Interview Definition

> A **race condition** is a concurrency problem that occurs when multiple threads or processes access and modify shared data simultaneously without proper synchronization, causing the program's behavior to depend on the order or timing of execution. Race conditions typically occur inside critical sections and are prevented using synchronization mechanisms such as mutexes, semaphores, or atomic operations.