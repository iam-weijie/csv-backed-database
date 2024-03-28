#ifndef DB_H
#define DB_H
#define HANDLE_SIZE 32
#define COMMENT_SIZE 64

typedef struct Record {
        char handle[HANDLE_SIZE];
        char comment[COMMENT_SIZE];
        unsigned long follower_count;
        unsigned long date_last_modified;
} Record;

typedef struct Database {
        Record *records;
        size_t size;
        size_t capacity;
} Database;

Database db_create();

void db_append(Database * db, Record const * item);

Record * db_index(Database * db, int index);

Record * db_lookup(Database * db, char const * handle);

void db_free(Database * db);

void db_load_csv(Database * db, char const * path);

void db_write_csv(Database * db, char const * path);

#endif
