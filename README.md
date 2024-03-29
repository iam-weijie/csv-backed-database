# A CSV-backed database written in C

This is a command-line program called `igdb` in C for managing a persistent database stored as a CSV file.

## What’s in the database?

The database in this repository is for tracking Instagram accounts. One record in the database stores the following information.
• Handle, e.g. @spottedmcgill
• Follower count, e.g. 14900
• Comment, e.g. a bit cringe tbh • Date last modified, e.g. 1710521259
This record would be represented in CSV form as
@spottedmcgill,14900,a bit cringe tbh,1710521259

## Compiling the program

To compile the program, use the Makefile. To use a Makefile, just run `make` in the directory in
which the Makefile is present. This will separately compile `database.c` into `database.o` and
`igdb.c` into `igdb.o` before linking both `.o` files together into the executable `igdb`.

## User interactions with the database

Here is a description of the commands that are supported.
• list – print out the whole database formatted as a table.
• add HANDLE FOLLOWERS – adds a new entry to the end of the database. That handle must not already exist in the database. A follow-up prompt is generated to ask the user for the value of the comment field.
• update HANDLE FOLLOWERS – updates an existing entry for the given handle. An entry for that handle must be present. A follow-up prompt is generated to ask the user for the value of the comment field.
• save – writes the database out to the file database.csv
• exit – quits the program, but warns the user about unsaved changes if any.
