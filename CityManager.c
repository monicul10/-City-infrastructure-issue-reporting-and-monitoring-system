#include "CityManager.h"

void get_mode_str(mode_t mode, char *str) {
    strcpy(str, "---------");
    if (mode & S_IRUSR) str[0] = 'r';
    if (mode & S_IWUSR) str[1] = 'w';
    if (mode & S_IXUSR) str[2] = 'x';
    if (mode & S_IRGRP) str[3] = 'r';
    if (mode & S_IWGRP) str[4] = 'w';
    if (mode & S_IXGRP) str[5] = 'x';
    if (mode & S_IROTH) str[6] = 'r';
}

int check_access(const char *path, const char *role, int required_bit_manager, int required_bit_inspector) {
    struct stat st;

    if (stat(path, &st) == -1) {
        perror("stat");
        return 0; //nu exista fisierul
    }

    // Extragem doar bitii de permisiune
    mode_t current_mode = st.st_mode & 0777;

    if (strcmp(role, "manager") == 0) {
        // Managerii sunt owneri; verificam bitii de owner [cite: 29, 35]
        if (current_mode & required_bit_manager) return 1;
    }
    else if (strcmp(role, "inspector") == 0) {
        // Inspectorii sunt in grup; verificam bitii de grup [cite: 29]
        if (current_mode & required_bit_inspector) return 1;
    }

    printf("Acces refuzat pentru rolul %s pe fisierul %s\n", role, path);
    return 0;
}

void add_report(const char *district_id,const char *user, const char *role, Report r) {
    char path[256];
    char link_name[256];

    sprintf(path,"%s/reports.dat",district_id);

    if (mkdir(district_id,0750)==-1) {  //creeaza directorul daca nu exista
        chmod(district_id,0750); //daca exista -> verifica permisiuni
    }

    int fd=open(path,O_WRONLY| O_CREAT| O_APPEND,0664);
    if (fd<0) {
        perror("open");
        exit(1);
    }

    chmod(path, 0664);

    r.Timestamp=time(NULL);
    strncpy(r.InspectorName,user,48);

    if ( write(fd, &r, sizeof(Report))!=sizeof(Report)) {
        perror("write");
    }
    close(fd);

    sprintf(link_name, "active_reports-%s", district_id);
    unlink(link_name); // Șterge link-ul vechi dacă există
    symlink(path, link_name);

    //log_operation(district_id,user,role,"add");
}

void list_reports(const char *district_id) {
    char path[256];
    sprintf(path,"%s/reports.dat",district_id);

    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        exit(1);
    }

    char perm_str[11];
    get_mode_str(st.st_mode, perm_str);

    printf("Informatii Fisier\n");
    printf("Permisiuni:%s\n",perm_str);
    printf("Dimensiune:%ld bytes\n",(long)st.st_size);
    printf("Ultima modificare:%s\n",ctime(&st.st_mtime));
    printf("\n");

    int fd=open(path,O_RDONLY);
    if (fd<0) {
        perror("open");
        exit(1);
    }

    Report r;
    int count=0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        printf("ID: %d | Inspector: %s | Category: %s | Severity: %d\n",
                r.ReportID, r.InspectorName, r.Issue, r.Severitylevel);
    }
    close(fd);
}

void view_report(const char *district_id,int id) {
    char path[256];
    sprintf(path,"%s/reports.dat",district_id);

    int fd=open(path,O_RDONLY);
    if (fd<0) {
        perror("open");
        exit(1);
    }

    Report r;
    int found=0;

    while (read(fd,&r,sizeof(Report)) == sizeof(Report)) {
        if (r.ReportID==id) {
            printf("Report details - ID->%d\n",r.ReportID);
            printf("Inspector: %s\n",r.InspectorName);
            printf("Category: %s\n",r.Issue);
            printf("Severity: %d\n",r.Severitylevel);
            char *time=ctime(&r.Timestamp);
            printf("Time: %s\n",time);
            printf("Description: %s\n",r.Description);

            found=1;
            break;
        }
    }

    if (!found) {
        perror("read"); //raportul cu id x, nu a fost gasit in district y
        exit(1);
    }
    close(fd);
}

void remove_report(const char *district_id,int report_id, const char *role) {

    if (strcmp(role,"manager")!=0) {
        printf("You don't have access\n");
        exit(1);
    }

    char path[256];
    sprintf(path,"%s/reports.dat",district_id);

    int fd=open(path,O_RDWR);
    if (fd<0) {
        perror("open");
        exit(1);
    }

    Report r;
    off_t pos=0;
    int found=0;

    while (read(fd,&r,sizeof(Report)) == sizeof(Report)) {
        if (r.ReportID==report_id) {
            found=1;
            break;
        }
        pos += sizeof(Report);
    }

    if (found) {
        Report next;
        off_t curr_pos;

        while (read(fd,&next,sizeof(Report)) == sizeof(Report)) {
            curr_pos=lseek(fd,0,SEEK_CUR);
            lseek(fd,pos,SEEK_SET);
            write(fd,&next,sizeof(Report));

            pos += sizeof(Report);
            lseek(fd,curr_pos,SEEK_SET);
        }

        if (ftruncate(fd,pos)==-1) {
            perror("ftruncate");
        }else {
            printf("Report %d deleted\n",report_id);
        }
    }else {
        printf("%d report was not found\n",report_id);
    }

    close(fd);
}

void remove_district(const char *district_dir,const char *role) {
  pid_t pid=fork();
  if (pid<0) {
    perror("fork");
    exit(1);
  }
  pid_t status=wait(&status);

  if(pid==0){
      printf("Child process created\n");
      if(role=="manager") {
            char *comanda="rm -rf ";
            execlp(comanda,comanda,district_dir,NULL);
            printf("Status child process = %d\n",status);
      }
  }
}


void update_threshold(const char *district, int new, const char *role) {
    if (strcmp(role,"manager")!=0) {
        printf("You don't have access\n");
        return;
    }
    char path[256];
    sprintf(path,"%s/district.cfg",district);

    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return;
    }

    mode_t current_mode = st.st_mode & 0777; //se izoleaza bitii de permisiune
    if (current_mode != 0640) {
        printf("Diagnosis: Incorrect permissions \n");
        return;
    }

    int fd=open(path,O_WRONLY|O_TRUNC);
    if (fd<0) {
        perror("open");
        exit(1);
    }

    char buff[16];
    int length= sprintf(buff,"%d\n",new);

    if (write(fd,buff,length)==-1) {
        perror("write");
    }else {
        printf("Severity level updated.\n");
    }
    close(fd);
}
