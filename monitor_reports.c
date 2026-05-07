// #include <signal.h>
// #include <string.h>
// #include <sys/wait.h>
#include "CityManager.h"
#define PID_FILE ".monitor_pid"


void handle_sigusr1(int sig) {  //se executa cand se adauga un report
    const char msg[]="A fost adaugat un raport";
    write(STDOUT_FILENO,msg,strlen(msg));
}

void handle_sigint(int sig) {
    const char msg[]="Se inchide fisierul";
    write(STDOUT_FILENO,msg,strlen(msg));
    unlink(PID_FILE);
    exit(0);
}

int main() {
    int fd=open(PID_FILE,O_WRONLY|O_CREAT|O_TRUNC,0644);
    if (fd==-1) {
        perror("open");
        exit(1);
    }

    char pid[16];
    int len=sprintf(pid,"%d",getpid());
    write(fd,pid,len);
    //close(fd);

    struct sigaction act;
    //memset(&act,0,sizeof(act));
    act.sa_handler=handle_sigusr1;
    //act.sa_flags=SA_RESTART;
    sigaction(SIGUSR1,&act,NULL);

    struct sigaction sa_int;
    //memset(&sa_int, 0 ,sizeof(sa_int));
    sa_int.sa_handler=handle_sigint;
    sigaction(SIGINT,&sa_int,NULL);

    printf("Pornit.Pid=%d\n",getpid());
    printf("Ctrl+C pentru stop\n");

    while(1) {
        pause();
    }
    return 0;
}

//gcc monitor_reports.c -o monitor_reports
//./monitor_reports