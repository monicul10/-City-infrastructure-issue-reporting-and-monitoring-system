#ifndef _CITY_INFRASTRUCTURE_ISSUE_REPORTING_AND_MONITORING_SYSTEM_CITYMANAGER_H
#define _CITY_INFRASTRUCTURE_ISSUE_REPORTING_AND_MONITORING_SYSTEM_CITYMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
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

void add_report(const char *district_id, const char *user, const char *role, Report r);
void list_reports(const char *district_id);
void update_threshold(const char *district_id, int value, const char *role);
//void log_operation(const char *district_id, const char *user, const char *role, const char *op);
int check_access(const char *path,const char*role,int required_bit_manager, int required_bit_inspector);

#endif //_CITY_INFRASTRUCTURE_ISSUE_REPORTING_AND_MONITORING_SYSTEM_CITYMANAGER_H