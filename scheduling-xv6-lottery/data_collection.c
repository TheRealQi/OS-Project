#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "fcntl.h"

int main() {
    struct pstat stat;
    settickets(100);
    // create 3 processes with different tickets (1:2:3 ratio)
    int pid1 = fork();
    if (pid1 == 0) {
        settickets(10);
        for (;;);
    }
    int pid2 = fork();
    if (pid2 == 0) {
        settickets(20);
        for (;;);
    }
    int pid3 = fork();
    if (pid3 == 0) {
        settickets(30);
        for (;;);
    }
    printf(1,"Process 10, Process 20, Process 30\n");
    sleep(1);
    //int j = 1;
    while (1) {
    	//printf(1, "%d,", j);
    	//j++;
        getpinfo(&stat);
        for (int i = 0; i < NPROC; i++) {
            if (stat.pid[i] == pid1) {
//                printf(1, "pid: %d, tickets: %d, ticks: %d\n", stat.pid[i], stat.tickets[i], stat.ticks[i]);
                printf(1, "%d,", stat.ticks[i]);
            }
            if (stat.pid[i] == pid2) {
                printf(1, "%d,", stat.ticks[i]);
            }
            if (stat.pid[i] == pid3) {
                printf(1, "%d\n", stat.ticks[i]);
            }
        }
    }
    exit();
}
