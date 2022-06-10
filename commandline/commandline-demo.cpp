/*************************************************************************
> FileName: commandline-demo.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年04月27日 星期一 10时27分10秒
 ************************************************************************/


#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
 
void showUsage() {
  //cout << "Usage: " << PACKAGE_NAME << " [options] URL ..." << endl;
  cout << "Options:" << endl;
  cout << " -d, --dir=DIR              The directory to store downloaded file." << endl;
  cout << " -o, --out=FILE             The file name for downloaded file." << endl;
  cout << " -l, --log=LOG              The file path to store log. If '-' is specified," << endl;
  cout << "                            log is written to stdout." << endl;
  cout << " -D, --daemon               Run as daemon." << endl;
  cout << " -s, --split=N              Download a file using s connections. s must be" << endl;
  cout << "                            between 1 and 5. If this option is specified the" << endl;
  cout << "                            first URL is used, and the other URLs are ignored." << endl;
  cout << " --http-proxy=HOST:PORT     Use HTTP proxy server. This affects to all" << endl;
  cout << "                            URLs." << endl;
  cout << " --http-user=USER           Set HTTP user. This affects to all URLs." << endl;
  cout << " --http-passwd=PASSWD       Set HTTP password. This affects to all URLs." << endl;
  cout << " --http-proxy-user=USER     Set HTTP proxy user. This affects to all URLs" << endl;
  cout << " --http-proxy-passwd=PASSWD Set HTTP proxy password. This affects to all URLs." << endl;
  cout << " --http-auth-scheme=SCHEME  Set HTTP authentication scheme. Currently, BASIC" << endl;
  cout << "                            is the only supported scheme." << endl;
  cout << " -v, --version              Print the version number and exit." << endl;
  cout << " -h, --help                 Print this message and exit." << endl;
  cout << "URL:" << endl;
  cout << " You can specify multiple URLs. All URLs must point to the same file" << endl;
  cout << " or a download fails." << endl;
  cout << "Examples:" << endl;
  cout << " Download a file by 1 connection:" << endl;
  cout << "  aria2c http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections:" << endl;
  cout << "  aria2c -s 2 http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections, each connects to a different server." << endl;
  cout << "  aria2c http://AAA.BBB.CCC/file.zip http://DDD.EEE.FFF/GGG/file.zip" << endl;
  cout << "Reports bugs to <tujikawa at rednoah dot com>" << endl;
}
 
int main(int argc, char* argv[]) {
  bool stdoutLog = false;
  string logfile;
  string dir;
  string ufilename;
  int split = 0;
  bool daemonMode = false;
  int c;
 
 
  while(1) {
    int optIndex = 0;
    int lopt;
    static struct option longOpts[] = {
      { "daemon", no_argument, NULL, 'D' },
      { "dir", required_argument, NULL, 'd' },
      { "out", required_argument, NULL, 'o' },
      { "log", required_argument, NULL, 'l' },
      { "split", required_argument, NULL, 's' },
      { "http-proxy", required_argument, &lopt, 1 },
      { "http-user", required_argument, &lopt, 2 },
      { "http-passwd", required_argument, &lopt, 3 },
      { "http-proxy-user", required_argument, &lopt, 4 },
      { "http-proxy-passwd", required_argument, &lopt, 5 },
      { "http-auth-scheme", required_argument, &lopt, 6 },
      { "version", no_argument, NULL, 'v' },
      { "help", no_argument, NULL, 'h' },
      { 0, 0, 0, 0 }
    };
    c = getopt_long(argc, argv, "Dd:o:l:s:vh", longOpts, &optIndex);
    printf("返回值： %d\n",c);
    if(c == -1) {
      break;
    }
    switch(c) {
    case 0:{
      switch(lopt) {
      case 1: {
	printf("1: %s\n",optarg);
	break;
      }
      case 2:
	printf("2: %s\n",optarg);
	break;
      case 3:
	printf("3: %s\n",optarg);
	break;
      case 4:
	printf("4: %s\n",optarg);
	break;
      case 5: 
	printf("5: %s\n",optarg);
	break;
      case 6:
	printf("6: %s\n",optarg);
	break;
      }
      break;
    }
    case 'D':
      printf("D: %s\n",optarg);
      break;
    case 'd':
      printf("d: %s\n",optarg);
      break;
    case 'o':
      printf("o: %s\n",optarg);
      break;
    case 'l':
     printf("l: %s\n",optarg);
      break;
    case 's':
      printf("s: %s\n",optarg);
      break;
    case 'v':
      printf("s: %s\n",optarg);
      //showVersion();
      exit(0);
    case 'h':
      showUsage();
      exit(0);
    default:
      showUsage();
      exit(1);
    }
  }
  return 0;
}
