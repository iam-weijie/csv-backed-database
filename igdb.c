#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "database.h"

int main_loop(Database * db)
{
	char command[100];
	char handle[HANDLE_SIZE];
	unsigned long followers;
	
	printf("Loaded %zu records.\n", (*db).size);
	printf("> ");

	while(fgets(command, sizeof(command), stdin)) {
		command[strcspn(command, "\r\n")] = 0; // Remove newline characters

		if (strcmp(command, "list") == 0) {
			printf("%-15s | %-9s | %-16s | %s\n", "HANDLE", "FOLLOWERS", "LAST MODIFIED", "COMMENT");
			printf("----------------|-----------|------------------|--------------------------------\n");
			for(size_t i = 0; i < (*db).size; i++) {
				// Unix to ymd
				struct tm *time = localtime(&(*db).records[i].date_last_modified);
				char ymd[20];
				strftime(ymd, sizeof(ymd), "%Y-%m-%d %H:%M", time);

				printf("%-15s | %-9lu | %-16s | %s\n", db->records[i].handle, db->records[i].follower_count, ymd, db->records[i].comment);
			}
			printf("> ");
		} else if (strcmp(command, "add") == 0) {
			scanf("%s %lu", handle, &followers);
		} else if (strcmp(command, "update") == 0 ) {
			scanf("%s %lu", handle, &followers);
		} else if (strcmp(command, "exit") == 0) {

		} else if (strcmp(command, "save") ==0) {

		}
	}
	return 0;
}

int main()
{
    Database db = db_create();
    db_load_csv(&db, "database.csv");
    return main_loop(&db);
}
