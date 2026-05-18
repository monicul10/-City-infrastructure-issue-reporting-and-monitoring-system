#include "CityManager.h"

void start_monitor() {
    int pipefd[2];
    if (pipe(pipefd) == -1) { perror("pipe"); return; }

    pid_t hub_mon_pid = fork();
    if (hub_mon_pid == 0) {
        close(pipefd[1]);

        pid_t monitor_pid = fork();
        if (monitor_pid == 0) { // Procesul Monitor
            dup2(pipefd[1], STDOUT_FILENO); // Redirectare stdout spre pipe
            close(pipefd[0]);
            execl("./monitor_reports", "monitor_reports", NULL);
            exit(1);
        }

        char buffer[1024];
        int bytes;
        while ((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes] = '\0';
            printf("[HUB-MONITOR] %s", buffer);
            if (strstr(buffer, "ERROR") || strstr(buffer, "ending")) {
                printf("\n[HUB-MON] Monitorul s-a oprit.\n");
                break;
            }
            //fflush(stdout);
        }
        close(pipefd[0]);
        exit(0);
    }
}

void calculating_scores(char **district,int count) {
    for (int i=0;i<count;i++) {
        int pid[2];
        pipe(pid);

        if (fork()==0) {
            dup2(pid[1], STDOUT_FILENO);
            close(pid[0]);
            execl("./monitor_scores", "monitor_scores", NULL);
            exit(1);
        }
        close(pid[1]);

        char res[1024];
        read(pid[0], res, sizeof(res));
        printf("District scores: %s --- %s",district[0],res);
        close(pid[0]);
    }
}


int main() {

    return 0;
}