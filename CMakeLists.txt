cmake_minimum_required(VERSION 4.0)
project(_City_infrastructure_issue_reporting_and_monitoring_system C)

set(CMAKE_C_STANDARD 11)

add_executable(_City_infrastructure_issue_reporting_and_monitoring_system
        CityManager.c
        main.c
        CityManager.h)
