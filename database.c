#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "database.h"
#define INITIAL_CAPACITY 4

// create a new database with initial size 0 and capacity 4
Database db_create() {
	Database db;
	db.records = malloc(INITIAL_CAPACITY * sizeof(Record));
	if (db.records == NULL) {
		fprintf(stderr, "Memory allocation failure\n");
		exit(EXIT_FAILURE);
	}
	db.size = 0;
	db.capacity = INITIAL_CAPACITY;
	return db;
}

// append a record to the database
void db_append(Database *db, Record const *item) {
	if ((*db).size >= (*db).capacity) {
		(*db).capacity *= 2;
		(*db).records = realloc((*db).records, (*db).capacity * sizeof(Record));
		if ((*db).records == NULL) {
         	fprintf(stderr, "Memory allocation failed\n");
            	exit(EXIT_FAILURE);
        	}
	}
	(*db).records[(*db).size++] = *item;
}

// return a pointer to the item at the given index
Record *db_index(Database *db, int index) {
	if (index < 0 || index >= (*db).size) {
		fprintf(stderr, "Index out of bond\n");
		return NULL;
	}
	return &((*db).records[index]);
}

// return a pointer to the first item that has the same handle
Record *db_lookup(Database *db, char const *handle) {
	for (size_t i = 0; i < (*db).size; i++) {
		if (strcmp((*db).records[i].handle, handle) == 0)
			return &((*db).records[i]);
	} return NULL;
}

// free memory
void db_free(Database *db) {
	free((*db).records);
}

// parse a single line of CSV data into one record
Record parse_record(char const *line) {
	Record record;
	char *token = strtok((char *)line, ",");
	strncpy(record.handle, token, HANDLE_SIZE);
	token = strtok(NULL, ",");
	record.follower_count = strtoul(token, NULL, 10);
    	token = strtok(NULL, ",");
    	strncpy(record.comment, token, COMMENT_SIZE);
    	token = strtok(NULL, ",");
    	record.date_last_modified = strtoul(token, NULL, 10);
   	return record;
}

// appends the records from the read file to database
void db_load_csv(Database *db, char const *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "Failed to open file %s\n", path);
		return;
	}

	char *line;
	size_t len = 0;

	while (getline(&line, &len, file) != -1) {
		Record record = parse_record(line);
		db_append(db, &record);
	}

	free(line);
	fclose(file);
}

// overwrite the file with contents of the database, in csv format
void db_write_csv(Database *db, char const *path) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
                fprintf(stderr, "Failed to open file %s\n", path);
                return;
        }
	
	for (size_t i = 0; i < (*db).size; i++) {
		fprintf(file, "%s, %lu, %s, %lu\n", (*db).records[i].handle, (*db).records[i].follower_count, (*db).records[i].comment, (*db).records[i].date_last_modified);
	}

	fclose(file);
}


