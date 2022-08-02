/*************************************************************************
> FileName: sqlite-exec.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年08月17日 星期二 16时07分36秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sqlite3.h>

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
       execute (db, buf);
       //sleep (1);
   }

   sqlite3_close(db);
}

void read_t (sqlite3* db)
{
    char* error = NULL;
    const char* sql = "select * from sqlite;";

    sqlite3_exec (db, sql, NULL, NULL, &error);
    if (error) {
        puts (error);
        sqlite3_free (error);
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

