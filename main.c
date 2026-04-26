#include "CityManager.h"


int main(int argc, char *argv[]) {


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


    Report r1 = {1, "", 45.0, 25.0, "Gropi", 5, 0, "Descriere test"};
    add_report("District1", "Ion", "manager", r1);

    Report r2 = {2, "", 46.0, 26.0, "Iluminat", 3, 0, "Descriere test 2"};
    add_report("District2", "Vasile", "inspector", r2);

    close(file_handler);

    return 0;

}


