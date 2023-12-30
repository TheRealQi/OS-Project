#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

int main(){
    struct pstat stat;
    settickets(10);
    int test_num = 0;
    while(1){
    	sleep(50);
	getpinfo(&stat);
	printf(1, "\n\n:::: Test %d ::::\n", test_num);
	for (int i = 0; i < NPROC; i++){
	     sleep(1);
	     if (stat.pid[i]){
	         printf(1,"Pid= %d | ticks = %d | tickets = %d | inuse = %d\n", stat.pid[i], stat.ticks[i], stat.tickets[i], stat.inuse[i]);
	     }
	}
	test_num++;
    }
    exit();
}
