#include "CityManager.h"


int main(int argc, char *argv[]) {
    if (argc!=4) {
        printf("Nu sunt destule argumente");
        exit(-1);
    }

    int file_handler=open("reports.dat", O_RDONLY,S_IWUSR);

    char *role=argv[2];
    int flag;
    if (strcmp(role, "manager")==0) {
        flag=chmod("reports.dat", O_APPEND|S_IWUSR);
        if (flag==-1) {
            perror("chmod");
        }
        flag=chmod("district.cfg", O_APPEND|S_IWUSR);
        if (flag==-1) {
            perror("chmod");
        }
        flag=chmod("logged_district.log", O_APPEND|S_IWUSR);
        if (flag==-1) {
            perror("chmod");
        }
    }else if(strcmp(role, "inspector")==0) {
        flag=chmod("reports.dat", O_APPEND|S_IWUSR);
        if (flag==-1) {
            perror("chmod");
        }
        flag=chmod("district.cfg", O_APPEND);
        if (flag==-1) {
            perror("chmod");
        }
        flag=chmod("logged_district.log", O_APPEND);
        if (flag==-1) {
            perror("chmod");
        }
    }else {
        printf("You don't have access");
    }

    close(file_handler);

    return 0;

}


