#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define RUN_TIME 1000*1000*50



struct foo
{
    int64_t f_count;
    pthread_mutex_t f_lock;
    char f_name;
}p, q, r, s;

void FooAlloc (struct foo *fp)
{
    fp->f_count = 0;
    pthread_mutex_init(&fp->f_lock, NULL);
}

void FooAdd(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void *Thread1(void *arg)
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++) {
        FooAdd(&p);
        FooAdd(&q);
        FooAdd(&r);
        FooAdd(&s);
    }
    pthread_exit((void *)0);
}

void *Thread2(void *arg)
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++){
        FooAdd(&p);
        FooAdd(&q);
        FooAdd(&s);
    }
    pthread_exit((void *)0);
}

void *Thread3(void *arg)
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++){
        FooAdd(&p);
        FooAdd(&r);
        FooAdd(&s);
    }
    pthread_exit((void *)0);
}

void *Thread4(void *arg)
{
    int i = 0;
    for ( i = 0; i < RUN_TIME; i++){
        FooAdd(&q);
        FooAdd(&s);
    }
    pthread_exit((void *)0);
}

void FooRele(struct foo *fp)
{
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
}

int main()
{
    int err;
    void *tret;
    time_t start,end;
    pthread_t tid1, tid2, tid3, tid4;
    start = time(NULL);
    FooAlloc(&p);
    FooAlloc(&q);
    FooAlloc(&r);
    FooAlloc(&s);
    err = pthread_create(&tid1, NULL, Thread1, NULL);
    err = pthread_create(&tid2, NULL, Thread2, NULL);
    err = pthread_create(&tid3, NULL, Thread3, NULL);
    err = pthread_create(&tid4, NULL, Thread4, NULL);

    err = pthread_join(tid1, &tret);
    err = pthread_join(tid2, &tret);
    err = pthread_join(tid3, &tret);
    err = pthread_join(tid4, &tret);
    printf("p=%ld\nq=%ld\nr=%ld\ns=%ld\n", p.f_count, q.f_count,
            r.f_count, s.f_count);
    FooRele(&p);
    FooRele(&q);
    FooRele(&r);
    FooRele(&s);
    end = time(NULL);
    printf("start:%ld,end:%ld,time:%lds\n", start, end, end - start);
    return 0;
}
