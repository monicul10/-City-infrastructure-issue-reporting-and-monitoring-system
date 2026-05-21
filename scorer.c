#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct Report {
    int ReportID;
    char InspectorName[48];
    double Latitude;
    double Longitude;
    char Issue[32];
    int Severitylevel;
    time_t Timestamp;
    char Description[96];
}Report;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    char *district_name = argv[1];
    char path[256];
    sprintf(path, "%s/reports.dat", district_name);

    int fd=open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char inspectori[100][48] = {0};
    int scoruri[100]={0};
    int total_inspectori=0;

    Report r;

    while (read(fd, &r, sizeof(Report))>0) {
        int gasit=-1;

        for (int i=0;i<total_inspectori;i++) {
            if (strcmp(inspectori[i],r.InspectorName)==0) {
                gasit=i;
                break;
            }
        }

        if (gasit!=-1) {
            scoruri[gasit++]=r.Severitylevel;
        }else {
            strncpy(inspectori[total_inspectori],r.InspectorName,48);
            scoruri[total_inspectori++]=r.Severitylevel;
            total_inspectori++;
        }
    }

    close(fd);

    for (int i=0;i<total_inspectori;i++) {
        printf("InspectorName: %s -> Scor:%d\n",inspectori[i], scoruri[i]);
    }
    return 0;
}