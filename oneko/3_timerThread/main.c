#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_TICK  9999

int NekoTickCount;
int NekoStateCount;
int RaiseWindowDelay = 0;
long IntervalTime = 125000L;

static void NullFunction();
static void NullFunction()
{
  /* No Operation */
#if defined(SYSV) || defined(SVR4)
  signal(SIGALRM, NullFunction);
#endif /* SYSV || SVR4 */
}

int main() {
    signal(SIGALRM, NullFunction);

    struct itimerval      Value;

    timerclear(&Value.it_interval);
    timerclear(&Value.it_value);

    Value.it_interval.tv_usec = IntervalTime;
    Value.it_value.tv_usec = IntervalTime;
    setitimer(ITIMER_REAL, &Value, 0);

    do {
        //TickCount();
        if (++NekoTickCount >= MAX_TICK) {
            NekoTickCount = 0;
        }
        if (NekoTickCount % 2 == 0) {
            if (NekoStateCount < MAX_TICK) {
                NekoStateCount++;
            }
        }
        //TickCount();

        printf("Thread: %d\n", NekoStateCount);

        //Interval();
        pause();
        if (RaiseWindowDelay>0) RaiseWindowDelay--;
        //Interval();
  } while (1);
	
    return 0;
}