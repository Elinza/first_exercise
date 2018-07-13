#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#define TIME_LEN 64



int main()
{
   time_t t;
   struct timeval tv;
   struct tm *tm;
   char buf[TIME_LEN];
   int msec = 0;

   gettimeofday(&tv, NULL);
   msec = tv.tv_usec / 1000;

   time(&t);
   tm = localtime(&t);
   strftime(buf, TIME_LEN, "%F %T", tm);

   printf("unix time :%ld\nlocal time :%s.%d\n", tv.tv_sec, buf, msec);

   return 0;
}

