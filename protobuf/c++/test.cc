/*************************************************************************
> FileName: test.cc
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 25 Jul 2022 09:45:08 AM CST
 ************************************************************************/

#include <string>
#include <iostream>
#include "test.pb.h"

using namespace std;

using namespace test1;

int main (int argc, char* argv[])
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    test1::Response p1;
    p1.set_data("data");
    p1.set_status(1);

    cout << "=================================\n";
    cout << "p1.data: " << p1.data() << "\n" 
        << "p1.status: " << p1.status() << "\n";

    cout << "serialize string:" << p1.SerializeAsString() << "\n";
    cout << "=================================\n";

    // 反序列化 bool ParseFromString(const string& data);


    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
