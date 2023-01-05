/*************************************************************************
> FileName: sqlite-get-table.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 05 Jan 2023 12:28:43 PM CST
 ************************************************************************/

#include <glib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>

#include <map>
#include <string>

#define DROP_TABLE  "DROP TABLE IF EXISTS [match_files];" 
    
#define CREATE_TABLE \
    "CREATE TABLE match_files (" \
    "   `full_path`         TEXT            NOT NULL," \
    "   `policy_id_tms`     TEXT," \
    "   `file_type`         VARCHAR(32)     NOT NULL," \
    "   `file_size`         VARCHAR(32)," \
    "   `detect_result`     TEXT," \
    "   `extends`           TEXT," \
    "   PRIMARY KEY (full_path)" \
    ");"

#define INSERT_TABLE \
    "INSERT INTO match_files (" \
    "`full_path`, `policy_id_tms`, `file_type`, `file_size`, `detect_result`, `extends`) " \
    " VALUES ('%s', '%s', '%s', '%s', '%s', '%s')"


bool sqlite_execute(sqlite3* sqlite, const char* sql)
{
    char*           error = NULL;

    int ret = sqlite3_exec (sqlite, sql, NULL, 0, &error);

    if (error) {
        printf ("%s, %s\n", sql, error);
        sqlite3_free(error);
    }

    return ret == SQLITE_OK;
}

typedef struct _SensitiveDoc
{
    std::string                             fileType;
    std::string                             fileSize;
    std::string                             scanResult;
    std::string                             fileFullPath;

    std::map<std::string, std::string>      extends;
    std::map<std::string, unsigned long>    policyIDMTimes;

} SensitiveDoc;

std::pair<std::string, unsigned long> parse_key_value (const std::string& str)
{
    if (str.empty()) {
        return std::pair<std::string, unsigned long>();
    }

    std::string sstr = str.substr (1, str.size() - 2);
    std::string policyID;
    unsigned long tim = 0;
    int pos = sstr.find (":", 0);
    if (std::string::npos != pos) {
        policyID = sstr.substr (0, pos);
        std::string t = sstr.substr (pos + 1, sstr.size() - pos - 1);
        tim = atol (t.c_str());
    }
    //printf ("-->%s - %lu\n", policyID.c_str(), tim);

    return std::pair<std::string, unsigned long> (policyID, tim);
}

bool parse_policy_id_times (const std::string& item, std::map<std::string, unsigned long>& policyIDMTimes/*out*/)
{
    if (item.size() <= 0) {
        return false;
    }

    int pos = 0;
    int startPos = pos;

    //printf ("%s\n", item.c_str());

    while (true) {
        pos = item.find (",,", startPos);
        if (pos == std::string::npos) {
            break;
        }

        std::string str = item.substr(startPos, pos + 1 - startPos);
        startPos = pos + 1;
        //printf ("str1:%s\n", str.c_str());
        policyIDMTimes.insert (parse_key_value (str));
    }

    // 最后一个
    if (startPos >= pos) {
        pos = item.find (",", startPos + 1);
        if (std::string::npos != pos) {
            pos = item.size();
            std::string str = item.substr (startPos, pos - startPos);
            //printf ("str2:%s\n", str.c_str());
            policyIDMTimes.insert (parse_key_value (str));
        }
    }

    return true;
}

std::string join_policy_id_times (const std::map<std::string, unsigned long>& p)
{
    if (p.size() <= 0) {
        return "";
    }

    std::string str = "";
    for (std::map<std::string, unsigned long>::const_iterator it = p.begin(); it != p.end(); ++it) {
        if (it->first.empty() && it->second > 0) {
            str += (std::string(",") + it->first + std::string(":") + std::to_string(it->second) + std::string(","));
        }
    }

    return str;
}

void replace_files_db_item (sqlite3* sqlite, const SensitiveDoc& doc)
{
    if (NULL == sqlite || doc.fileFullPath.empty()) {
        return ;
    }

    g_autofree char* sql = g_strdup_printf ("DELETE FROM match_files WHERE full_path='%s'", doc.fileFullPath.c_str());
    if (!sqlite_execute (sqlite, sql)) {
        printf ("sql: %s, error!", sql);
        return;
    }

    sql = g_strdup_printf (INSERT_TABLE, doc.fileFullPath.c_str(), join_policy_id_times(doc.policyIDMTimes).c_str(), doc.fileType.c_str(), doc.fileSize.c_str(), doc.scanResult.c_str(), "");
    if (!sqlite_execute (sqlite, sql)) {
        printf ("sql: %s, error!", sql);
        return;
    }
}

void print_doc (const SensitiveDoc& doc)
{

    printf ("full_path:%s\n", doc.fileFullPath.c_str());
    printf ("file_type:%s\n", doc.fileType.c_str());
    printf ("file_size:%s\n", doc.fileSize.c_str());
    printf ("detect_result:%s\n", doc.scanResult.c_str());
    printf ("policy_id_tms:\n");
    for (std::map<std::string, unsigned long>::const_iterator it = doc.policyIDMTimes.begin(); it != doc.policyIDMTimes.end(); ++it) {
        printf ("\t%s: %lu", it->first.c_str(), it->second);
    }
}

int main (int argc, char* argv[])
{
    sqlite3*        sqlite = NULL;
    char*           error = NULL;
    char**          dbResult = NULL;
    int             row = 0;
    int             column = 0;
    int             ret = 0;

    ret = sqlite3_open("match-files.db", &sqlite);
    if (0 != ret) {
        printf ("数据库创建失败! 错误: %s\n", sqlite3_errmsg(sqlite));
        sqlite3_close(sqlite);
        return -1;
    }

    // 创建数据库表结构
    sqlite_execute (sqlite, DROP_TABLE);

    if (!sqlite_execute (sqlite, CREATE_TABLE)) {
        printf ("创建表结构失败!\n");
        sqlite3_close (sqlite);

        return -1;
    }
    
    // 插入数据
    /**
    "INSERT INTO match_files (" \
    "`full_path`, `policy_id_tms`, `file_type`, `file_size`, `detect_result`, `extends`) " \
    " VALUES ('%s', '%s', '%s', '%s', '%s', '%s')"
     */
    for (int i = 0; i < 10; ++i) {
        g_autofree char* name = g_strdup_printf ("/tmp/%d.txt", i);
        g_autofree char* sql = NULL;
        if (i == 0) {
            sql = g_strdup_printf (INSERT_TABLE, name, "", "txt", "20", "result....", "extends ...");
        } else if (i == 1) {
            sql = g_strdup_printf (INSERT_TABLE, name, ",1:212121,", "txt", "20", "result....", "extends ...");
        } else if (i == 2) {
            sql = g_strdup_printf (INSERT_TABLE, name, ",1:212121,,2:12334234,", "txt", "20", "result....", "extends ...");
        } else if (i == 3) {
            sql = g_strdup_printf (INSERT_TABLE, name, ",1:212121,,2:12334234,,3:123123123", "txt", "20", "result....", "extends ...");
        } else {
            sql = g_strdup_printf (INSERT_TABLE, name, ",1:212121,,2:12334234,,3:123132123,,4:23423,", "txt", "20", "result....", "extends ...");
        }

        if (!sqlite_execute (sqlite, sql)) {
            continue;
        }
    }


    ret = sqlite3_get_table (sqlite, "select full_path, policy_id_tms, file_type, file_size, detect_result, extends from match_files;", &dbResult, &row, &column, &error);
    if (NULL != error) {
        printf ("get_table 执行错误: %s\n", error);
        sqlite3_free_table (dbResult);
        return -1;
    }

    printf ("rows: %d\ncolumn: %d\n\n", row, column);

    for (int i = 0; i <= row; ++i) {
        SensitiveDoc doc;
        for (int j = 0; j < column; ++j) {
            printf ("%s\t", dbResult[i * column + j]);
            if (i != 0) {
                switch (j) {
                    case 0: {
                        doc.fileFullPath = dbResult[i * column + j];
                        break;
                    }
                    case 1: {
                        // policy_id_tms
                        //printf ("%d\n", i);
                        parse_policy_id_times (dbResult[i * column + j], doc.policyIDMTimes);
                        //printf ("\n\n");
                        break;
                    }
                    case 2: {
                        doc.fileType = dbResult[i * column + j];
                        break;
                    }
                    case 3: {
                        doc.fileSize = dbResult[i * column + j];
                        break;
                    }
                    case 4: {
                        doc.scanResult = dbResult[i * column + j];
                        break;
                    }
                    case 5: {
                        // extends
                        break;
                    }
                }
            }
        }

        //
        if (i != 0) {
            print_doc (doc);
            replace_files_db_item (sqlite, doc);
        }

        printf ("\n\n");
    }
    printf ("\n");



    sqlite3_free_table (dbResult);
    sqlite3_close(sqlite);

    return 0;

    (void)argc;
    (void)argv;
}
