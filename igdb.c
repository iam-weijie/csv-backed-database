#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "database.h"

// declare a global flag to track unsaved changes
int unsaved_changes = 0;

void set_unsaved_changes() {
	unsaved_changes = 1;	
}

void reset_unsaved_changes() {
	unsaved_changes = 0;
}

// function to check if there are unsaved changes
int has_unsaved_changes() {
	return unsaved_changes;
}

// print out a record
void print_record(Record *r) {
	// Unix to ymd
        struct tm *time = localtime(&(r->date_last_modified));
        char ymd[20];
        strftime(ymd, sizeof(ymd), "%Y-%m-%d %H:%M", time);

        printf("%-15s | %-9lu | %-16s | %-34s\n", r->handle, r->follower_count, ymd, r->comment);
}

// print out all the records
void print_database(Database *db) {
	printf("%-15s | %-9s | %-16s | %-34s\n", "HANDLE", "FOLLOWERS", "LAST MODIFIED", "COMMENT");
        printf("----------------|-----------|------------------|--------------------------------\n");
        
	for(size_t i = 0; i < (*db).size; i++) {
		Record *r = db_index(db, i);
		
		// checking record to avoid segmentation fault
		if (r != NULL)
			print_record(r);
		else
			fprintf(stderr, "Error: Invalid record at index %zu\n", i);
	}
}

int main_loop(Database * db)
{
	char command[100];
	unsigned long followers;
	char comment[COMMENT_SIZE];
	
	printf("Loaded %zu records.\n", (*db).size);
	printf("> ");

	while(fgets(command, sizeof(command), stdin)) {
		command[strcspn(command, "\r\n")] = 0; // Remove newline characters
		
		// list the records
		if (strcmp(command, "list") == 0) {
			print_database(db);
			printf("> ");
		} 
		
		// add a record to the end
		else if (strncmp(command, "add", 3) == 0) {
			char *token = strtok(command, " "); 
			char *handle;	

			token = strtok(NULL, " "); //move to next token
			if (token == NULL)		
				printf("Error: usage: add HANDLE FOLLOWERS\n");
			else {
				handle = token;
				
				token = strtok(NULL, " ");

				if (token == NULL) 
					printf("Error: usage: add HANDLE FOLLOWERS\n");
				else if (strlen(handle) > HANDLE_SIZE-1)
                                	printf("Error: handle too long.\n");
				else {
					char *endptr;
                                        errno = 0;
                                        followers = strtol(token, &endptr, 10);
					
					// check int and trailing char
					if(errno != 0 || *endptr != '\0' || followers < 0) {
					//another way: (sscanf(token, "%lu", &followers) != 1)
						printf("Error: follower count must be an integer\n");
					}
					else if (db_lookup(db, handle) != NULL)
						printf("Error: handle %s already exists.\n", handle);
					else { 
						printf("Comment> ");
						fgets(comment, sizeof(comment), stdin);
						comment[strcspn(comment, "\r\n")] = 0;	
					
						// check for comma in comment
						if (strchr(comment, ',') != NULL)
							printf("Error: comment cannot contain commas.\n");
						else if (strlen(comment) > COMMENT_SIZE-1)
                					printf("Error: comment too long.\n");
						else {
							// add record to the database
							Record new_record;
							strncpy(new_record.handle, handle, HANDLE_SIZE);
							new_record.follower_count = followers;
							strncpy(new_record.comment, comment, COMMENT_SIZE);
							new_record.date_last_modified = time(NULL);
							
							db_append(db, &new_record);
							set_unsaved_changes();
						}
					}
				}	
			}
			printf("> ");
		} 
		
		// update an existing record
		else if (strncmp(command, "update", 6) == 0 ) {
			char *token = strtok(command, " ");
                        char *handle;

                        token = strtok(NULL, " "); //move to next token
                        if (token == NULL)
                                printf("Error: usage: update HANDLE FOLLOWERS\n");
                        else {
                                handle = token;

                                token = strtok(NULL, " ");

                                if (token == NULL)
                                        printf("Error: usage: update HANDLE FOLLOWERS\n");
                                else {
					char *endptr;
					errno = 0;
					followers = strtol(token, &endptr, 10);
					
					Record *record = db_lookup(db, handle);
					if (record == NULL)
                                        	printf("Error: no entry with handle %s\n", handle);
					
					// check int conversion and trailing char	
                                	else if (errno != 0 || *endptr != '\0' || followers < 0) {
						//(sscanf(token, "%lu", &followers) != 1)
                                        	printf("Error: follower count must be an integer\n");
					}
					else {
                                        	printf("Comment> ");
                                        	fgets(comment, sizeof(comment), stdin);
                                        	comment[strcspn(comment, "\r\n")] = 0;

                                        	// check for comma in comment
                                        	if (strchr(comment, ',') != NULL)
                                                	printf("Error: comment cannot contain commas.\n");
                                        	else if (strlen(comment) > COMMENT_SIZE-1)
                                                	printf("Error: comment too long.\n");
                                        	else {
                                                	// update record to the database
                                        		record->follower_count = followers;
							strncpy(record->comment, comment, COMMENT_SIZE);	
							record->date_last_modified = time(NULL);
							set_unsaved_changes();
						}
					}
                                }
                        }
                        printf("> ");
		} 
		
		// to save into database.csv	
		else if (strcmp(command, "save") ==0) {
			db_write_csv(db, "database.csv");
			printf("Wrote %zu records.\n", (*db).size);
			printf("> ");
			reset_unsaved_changes();
		}

		// to exit
                else if (strcmp(command, "exit") == 0) {
                        if (has_unsaved_changes()) {
                                printf("Error: you did not save your changes. Use `exit fr` to force exiting anyway.\n");
                                printf("> ");
                        } else {
                                db_free(db);
				exit(EXIT_SUCCESS);
			}
                }

		// force exit
		else if (strcmp(command, "exit fr") == 0) {
			db_free(db);
			exit(EXIT_SUCCESS);
		}

		// invalid operation
		else {
			printf("Error: Invald operation. Allowed operations: list, add, update, save, exit\n");
			printf("> ");
		}
	}
}

int main()
{
    Database db = db_create();
    db_load_csv(&db, "database.csv");
    return main_loop(&db);
}
