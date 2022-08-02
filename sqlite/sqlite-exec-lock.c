/*************************************************************************
> FileName: sqlite-exec-lock.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 02 Aug 2022 02:52:23 PM CST
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sqlite3.h>

#define LOCK_FILE   ".lock"

bool sqlite_lock();
bool sqlite_unlock();
void init_lock_file ();
void execute (sqlite3* db, const char* sql);

int main(int argc, char* argv[])
{
   sqlite3 *db;
   int rc;

   rc = sqlite3_open("sqlite.db", &db);
   if (rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   const char* delete = "DELETE FROM sqlite;";
   execute (db, delete);

   char buf [2048] = {0};

   for (int i = 0; i < 10000000; ++i) {
       memset (buf, 0, sizeof buf);
       snprintf (buf, sizeof buf - 1, "INSERT INTO sqlite (`id`, `name`) VALUES (%d, '%s');", i, "a");
       while (!sqlite_lock());
       execute (db, buf);
       while (!sqlite_unlock());
   }

   sqlite3_close(db);
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

void execute (sqlite3* db, const char* sql) 
{
   char* error = NULL;

   sqlite3_exec (db, sql, NULL, NULL, &error);
   if (error) {
       puts (error);
       sqlite3_free (error);
   }
}

