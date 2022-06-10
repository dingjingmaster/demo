/*************************************************************************
> FileName: sqlite.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年08月17日 星期二 16时07分36秒
 ************************************************************************/


#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("test.db", &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
}
