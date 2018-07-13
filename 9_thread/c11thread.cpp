#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <time.h>

#define RUN_TIME 1000*1000*50
#define NUM 4

using namespace std;



atomic_int_fast64_t p(0);
atomic_int_fast64_t q(0);
atomic_int_fast64_t r(0);
atomic_int_fast64_t s(0);

void Thread(int c)
{
    int64_t i = 0;

    for (i = 0; i != RUN_TIME; i++) {
        if (c & 0x01)
            p++;
        if (c & 0x02)
            q++;
        if (c & 0x04)
            r++;
        if (c & 0x08)
            s++;
    }
};

int main()
{
    time_t start, end;
    start = time(NULL);

    std::thread t1(Thread, 0x0f);
    std::thread t2(Thread, 0x0b);
    std::thread t3(Thread, 0x0d);
    std::thread t4(Thread, 0x0a);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    end = time(NULL);
    printf("start=%ld,end=%ld,time: %lds\n", start, end, end - start);
    return 0;
}
