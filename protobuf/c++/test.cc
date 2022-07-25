/*************************************************************************
> FileName: test.cc
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 25 Jul 2022 09:45:08 AM CST
 ************************************************************************/

#include <string>
#include "test.pb.h"

using namespace std;

using namespace test1;

int main (int argc, char* argv[])
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;


    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
