/*************************************************************************
> FileName: sqlite-read.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 02 Aug 2022 03:08:33 PM CST
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

   char sql [128] = "SELECT * FROM sqlite;";

   for (int i = 0; i < 10000000; ++i) {
       while (!sqlite_lock());

       // 读取数据、打印
       sqlite3_stmt* stmt = NULL;
       int ret = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
       if (SQLITE_OK == ret) {
           while (SQLITE_DONE != sqlite3_step (stmt)) {
               const int id = sqlite3_column_int (stmt, 0);
               const unsigned char* name = sqlite3_column_text (stmt, 1);
               printf ("id: %d, name: %s\n", id, name);
           }
       }
       if (stmt)    sqlite3_finalize (stmt);

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


