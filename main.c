#include "CityManager.h"

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Utilizare: %s --district <id> --user <nume> --role <manager/inspector> --action <add/list/update/remove>\n", argv[0]);
        return 1;
    }

    const char *dist = argv[2];
    const char *user = argv[4];
    const char *role = argv[6];
    const char *action = argv[8];

    // Verificare rol
    if (strcmp(role, "manager") != 0 && strcmp(role, "inspector") != 0) {
        printf("Eroare: Rol invalid (%s). Acces refuzat.\n", role);
        return 1;
    }

    if (strcmp(action, "add") == 0) {
        Report r;
        printf("Introdu ID Raport: "); scanf("%d", &r.ReportID);
        printf("Introdu Categorie: "); scanf("%s", r.Issue);
        printf("Introdu Severitate (1-5): "); scanf("%d", &r.Severitylevel);
        getchar(); // consuma newline-ul ramas
        printf("Introdu Descriere: "); fgets(r.Description, 96, stdin);

        add_report(dist, user, role, r);
        log_operation(dist); // Logarea operatiunii
    }

    else if (strcmp(action, "list") == 0) {
        list_reports(dist);
        log_operation(dist);
    }

    else if (strcmp(action, "update") == 0) {
        if (argc < 10) {
            printf("Lipseste valoarea pragului! (--value <nr>)\n");
            return 1;
        }
        int new_val = atoi(argv[10]);
        update_threshold(dist, new_val, role);
        log_operation(dist);
    }

    else if (strcmp(action, "filter") == 0) {
        if (argc < 10) {
            printf("Lipseste conditia! (ex: severity:>=:3)\n");
            return 1;
        }
         filter_reports(dist, argv[10]);
    }

    else if (strcmp(action, "remove") == 0) {
        if (argc < 10) {
            printf("Lipseste ID-ul raportului!\n");
            return 1;
        }
        int id_to_del = atoi(argv[10]);
        remove_report(dist, id_to_del, role);
        log_operation(dist);
    }

    return 0;
}


