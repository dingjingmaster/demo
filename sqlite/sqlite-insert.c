/*************************************************************************
> FileName: sqlite-insert.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 19 Apr 2023 09:01:50 AM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <stdbool.h>

#define LOCK_FILE   ".lock.lock"

bool sqlite_lock();
bool sqlite_unlock();
void init_lock_file ();
void execute (sqlite3* db, const char* sql);

int main (int argc, char* argv[])
{
    int rc;
    sqlite3* db = NULL;

    rc = sqlite3_open ("sqlite.db", &db);
    if (rc) {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    //while (!sqlite_lock());
    //const char* delete = "DELETE FROM sqlite;";
    //execute (db, delete);
    //while (!sqlite_unlock());

    char buf[2048] = {0};
    while (1) {
        memset (buf, 0, sizeof(buf));
        snprintf (buf, sizeof(buf) - 1, "INSERT INTO sqlite (`id`, `name`) VALUES (%d, '%s')", rand() % 999999999, "a");
        while (!sqlite_lock());
        execute (db, buf);
        while (!sqlite_unlock());
        printf ("%s\n", buf);
        usleep(10000);
    }

}

void execute (sqlite3* db, const char* sql)
{
    char* error = NULL;

    sqlite3_exec (db, sql, NULL, NULL, &error);
    if (error) {
        puts (error);
        sqlite3_free(error);
    }
}
static FILE* fl = NULL; 

bool sqlite_lock()
{
    init_lock_file ();

    if (!fl) {
        fl = fopen (LOCK_FILE, "a+");
        if (!fl) {
            printf ("fopen error!\n");
            exit(-1);
        }
    }

    if (flock (fl->_fileno, LOCK_EX | LOCK_NB) == 0) {
        printf ("locked\n");
        return true;
    }
    
    return false;
}

bool sqlite_unlock()
{
    init_lock_file ();

    if (fl) {
        if (flock (fl->_fileno, LOCK_UN) == 0) {
            printf ("unlocked\n");
            return true;
        }
    } else {
        return true;
    }

    return false;
}

void init_lock_file ()
{
    if (access (LOCK_FILE, F_OK)) {
        if (creat (LOCK_FILE, 0777)) {
            printf ("creat()failed!\n");
            exit(-1);
        }
    }
}


