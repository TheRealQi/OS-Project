#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#include "fcntl.h"

int main(){
    struct pstat stat;
    int test_num = 1;
    settickets(100);
    int t = 10;
    int size = 0;
    int fd;
    while(t){
	getpinfo(&stat);
	printf(1, "\n\n:::: Test %d ::::\n", test_num);
	for (int i = 0; i < NPROC; i++){
	     sleep(1);
	     if (stat.pid[i]){
	         printf(1,"Pid= %d | ticks = %d | tickets = %d | inuse = %d\n", stat.pid[i], stat.ticks[i], stat.tickets[i], stat.inuse[i]);
	         if(stat.tickets[i] == 30){
	             fd = open("process3", O_RDWR);
	             size = sizeof(stat.tickets[i]);
	             if(!write(fd, &stat.tickets[i], size)){
	                 printf(1, "failed writing to process3");
	                 printf(1, "%d", stat.tickets[i]);
	             }
	             close(fd);
	         }
	         else if(stat.tickets[i] == 20){
	             fd = open("process2", O_RDWR);
	             close(fd);
	         }
	         else if(stat.tickets[i] == 10){
	             fd = open("process2", O_RDWR);
	             close(fd);
	         }
	     }
	}
	t--;
	test_num++;
    }
    exit();
}
