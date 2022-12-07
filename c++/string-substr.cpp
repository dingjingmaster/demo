/*************************************************************************
> FileName: string-substr.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Dec 2022 08:56:44 AM CST
 ************************************************************************/
#include <stdio.h>
#include <iostream>

#include <list>
#include <string>
    
void replace_all(std::string& inout, std::string what, std::string with)
{
    for (std::string::size_type pos = 0; inout.npos != (pos = inout.find(what, pos)); pos += with.length()) {
        inout.replace(pos, what.length(), with);
    }
}

bool report_path_valid (std::string& reportPath, std::string& scanDirs, std::string& exceptDirs)
{
    if (reportPath.empty() || reportPath.length() <= 0) {
        return false;
    }
    if (scanDirs.empty() || scanDirs.length() <= 0 || "/" == scanDirs ) {
        return true;
    }

    if (scanDirs[scanDirs.length() - 1] != '|') {
        scanDirs = scanDirs.append("|");
    }


    //bool finished = false;
    std::list<std::string> scanDirList;
    for (int pos = 0, start = 0; scanDirs.npos != (pos = scanDirs.find("|", pos)); ++pos, start = pos) {
        std::string scanDir;
        scanDir = scanDirs.substr(start, pos - start);

        if (scanDir[scanDir.length() - 1] == '|') {
            scanDir = scanDir.erase(scanDir.length() - 1, 1);
        }

        if (scanDir[scanDir.length() - 1] != '/') {
            scanDir.append("/");
        }
        scanDirList.push_back(scanDir);
    }

    std::list<std::string> exceptDirList;
    if (!exceptDirs.empty() && "" != exceptDirs) {
        //finished = false;

        if (exceptDirs[exceptDirs.length() - 1] != '|') {
            exceptDirs = exceptDirs.append("|");
        }

        for (int pos = 0, start = 0; exceptDirs.npos != (pos = exceptDirs.find("|", pos)); ++pos, start = pos) {
            std::string exceptDir;
            exceptDir = exceptDirs.substr(start, pos - start);

            if (exceptDir[exceptDir.length() - 1] == '|') {
                exceptDir = exceptDir.erase(exceptDir.length() - 1, 1);
            }

            if (exceptDir[exceptDir.length() - 1] != '/') {
                exceptDir.append("/");
            }
            exceptDirList.push_back(exceptDir);
        }
    }

#if 0
    std::cout << "\n\n扫描路径:\n";
    for (std::list<std::string>::iterator it = scanDirList.begin(); it != scanDirList.end(); ++it) {
        std::cout << *it << "\t";
    }
    std::cout << "\n";

    std::cout << "\n\n例外路径:\n";
    for (std::list<std::string>::iterator it = exceptDirList.begin(); it != exceptDirList.end(); ++it) {
        std::cout << *it << "\t";
    }
    std::cout << "\n";
#endif

    // 检查上报文件是否属于扫描路径
    bool ret = false;
    for (std::list<std::string>::iterator it = scanDirList.begin(); it != scanDirList.end(); ++it) {
        if (0 == reportPath.find(*it, 0)) {
            ret = true;
            break;
        }
    }

    // 检查上报文件是否属于例外路径
    for (std::list<std::string>::iterator it = exceptDirList.begin(); it != exceptDirList.end(); ++it) {
        if (0 == reportPath.find(*it, 0)) {
            ret = false;
            break;
        }
    }

    return ret;
}

int main (int argc, char* argv[])
{
    std::string str1 = "/home//dingjing|/home/dingjing//aa|";           // 指定扫描路径
    std::string str2 = "/home/dingjing/aa/cc|/home/dingjing/aa/dd|";    // 例外路径
                                                                        
    std::string str11 = "/home//dingjing";                              // 指定扫描路径
    std::string str22 = "/home/dingjing/aa/cc";                         // 例外路径
                                                                        //
    std::string str111 = "/home//dingjing";                             // 指定扫描路径
    std::string str222 = "";                                            // 例外路径
                                                                        //
    std::string str1111 = "/home/abc|/home/ddk|/home/xxx";              // 指定扫描路径
    std::string str2222 = "/home/abc/|/home/ff";                        // 例外路径
                                                                        
    std::string str3 = "/home/dingjing/aa/bb.txt";                      // 指定上报文件路径
    std::string str4 = "/home/dingjing/aa/cc/bb.txt";                   // 指定上报文件路径
                                                                        
    std::string str5 = "/home/xxx/bb.txt";                              // 指定上报文件路径
    std::string str6 = "/home/abc/cc/bb.txt";                           // 指定上报文件路径


    // 从这里开始

    // 解决扫描路径
    std::cout << "扫描路径1: " << str1 << std::endl;
    replace_all(str1, "//", "/");
    std::cout << "扫描路径2: " << str1 << std::endl << std::endl;

    std::cout << "例外路径1: " << str2 << std::endl;
    replace_all(str2, "//", "/");
    std::cout << "例外路径2: " << str2 << std::endl << std::endl;

    std::cout << "扫描路径1: " << str11 << std::endl;
    replace_all(str11, "//", "/");
    std::cout << "扫描路径2: " << str11 << std::endl << std::endl;

    std::cout << "例外路径1: " << str22 << std::endl;
    replace_all(str22, "//", "/");
    std::cout << "例外路径2: " << str22 << std::endl << std::endl;

    std::cout << "扫描路径1: " << str111 << std::endl;
    replace_all(str111, "//", "/");
    std::cout << "扫描路径2: " << str111 << std::endl << std::endl;

    std::cout << "例外路径1: " << str222 << std::endl;
    replace_all(str222, "//", "/");
    std::cout << "例外路径2: " << str222 << std::endl << std::endl;


    // 根据扫描路径和上报路径，判断上报路径是否属于扫描范围内
    std::cout << "res: " << str3 << " -- " << (report_path_valid(str3, str1, str2) ? "true" : "false") << std::endl;
    std::cout << "res: " << str4 << " -- " << (report_path_valid(str4, str1, str2) ? "true" : "false") << std::endl;

    // 根据扫描路径和上报路径，判断上报路径是否属于扫描范围内
    std::cout << "res: " << str3 << " -- " << (report_path_valid(str3, str11, str22) ? "true" : "false") << std::endl;
    std::cout << "res: " << str4 << " -- " << (report_path_valid(str4, str11, str22) ? "true" : "false") << std::endl;

    // 根据扫描路径和上报路径，判断上报路径是否属于扫描范围内
    std::cout << "res: " << str3 << " -- " << (report_path_valid(str3, str111, str222) ? "true" : "false") << std::endl;
    std::cout << "res: " << str4 << " -- " << (report_path_valid(str4, str111, str222) ? "true" : "false") << std::endl;

    // 根据扫描路径和上报路径，判断上报路径是否属于扫描范围内
    std::cout << "res: " << str5 << " -- " << (report_path_valid(str5, str1111, str2222) ? "true" : "false") << std::endl;
    std::cout << "res: " << str6 << " -- " << (report_path_valid(str6, str1111, str2222) ? "true" : "false") << std::endl;



    return 0;
}
