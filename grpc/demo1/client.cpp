/*************************************************************************
> FileName: client.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 04 Aug 2022 01:42:36 PM CST
 ************************************************************************/

#include <memory>
#include <string>
#include <iostream>

#include <grpcpp/grpcpp.h>

#include "msg.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


class MsgClient
{
public:
    MsgClient (std::shared_ptr<Channel> channel) : stub_(MsgService::NewStub(channel))
    {
    }

    MsgResponse GetMsg (const std::string& user, int num1, double num2)
    {
        MsgRequest request;

        request.set_name(user);
        request.set_num1(num1);
        request.set_num2(num2);

        // 服务器返回端
        MsgResponse reply;

        ClientContext context;

        Status status = stub_->GetMsg(&context, request, &reply);

        if (status.ok()) {
            return reply;
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;

            return reply;
        }
    }

private:
    std::unique_ptr<MsgService::Stub> stub_;
};

int main (int argc, char* argv[])
{
    MsgClient z_msg (grpc::CreateChannel("127.0.0.1:50000", grpc::InsecureChannelCredentials()));

    std::string user("world");

    MsgResponse reply = z_msg.GetMsg(user, 234, 3.14);

    std::cout << reply.msg() << std::endl;

    printf ("num1 = %d; num2 = %f\n", reply.num1(), reply.num2());

    return 0;
}
