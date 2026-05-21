#include "CityManager.h"

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Utilizare: %s --role <r> --user <u> --district <d> --action\n", argv[0]);
        return 1;
    }

    const char *role     = argv[2]; // manager
    const char *user     = argv[4]; // alice
    const char *district = argv[6]; // downtown

    char *action = "";
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--add") == 0) action = "add";
        else if (strcmp(argv[i], "--list") == 0) action = "list";
        else if (strcmp(argv[i], "--remove_district") == 0) action = "remove_district";
        else if (strcmp(argv[i], "--view") == 0) action = "view";
    }

    // Verificare Rol
    if (strcmp(role, "manager") != 0 && strcmp(role, "inspector") != 0) {
        printf("Eroare: Rol invalid (%s). Acces refuzat.\n", role);
        return 1;
    }

    if (strcmp(action, "add") == 0) {
        Report r;
        printf("Introdu ID Raport: "); scanf("%d", &r.ReportID);
        printf("Introdu Categorie: ");
        fgets(r.Issue, sizeof(r.Issue), stdin);
        r.Issue[strcspn(r.Issue, "\n")] = '\0';
        printf("Introdu Severitate (1-5): "); scanf("%d", &r.Severitylevel);
        printf("Introdu Coordonata X: "); scanf("%lf", &r.Latitude);
        printf("Introdu Coordonata Y: "); scanf("%lf", &r.Longitude);
        getchar();
        printf("Introdu Descriere: "); fgets(r.Description, 96, stdin);

        add_report(district, user, role, r);
        notify_monitor(district); // Pentru Phase 2
    }
    else if (strcmp(action, "list") == 0) {
        list_reports(district);
    }
    else if (strcmp(action, "view") == 0) {
        int id_to_view = 0;
        for(int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--view") == 0 && i+1 < argc)
                id_to_view = atoi(argv[i+1]);
        }
        view_report(district, id_to_view);
    }
    else if (strcmp(action, "remove_district") == 0) {
        remove_district(district, role);
    }

    return 0;
}