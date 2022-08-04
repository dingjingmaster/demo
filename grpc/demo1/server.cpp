/*************************************************************************
> FileName: server.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 04 Aug 2022 09:37:06 AM CST
 ************************************************************************/
#include <string>
#include <memory>
#include <iostream>

#include <stdio.h>

#include <grpcpp/grpcpp.h>

#include "msg.pb.h"
#include "msg.grpc.pb.h"

using grpc::Server;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::ServerContext;

class MyMsgServer final : public MsgService::Service 
{
    virtual Status GetMsg (ServerContext* ctx, const MsgRequest* req, MsgResponse* rsp) override
    {
        std::string str1 ("Hello ");
        rsp->set_msg (str1 + req->name());
        rsp->set_num1 (32);
        rsp->set_num2 (3.1415);

        return Status::OK;
    }
};

void RunServer () 
{
    std::string serverAddr("0.0.0.0:50000");

    MyMsgServer service;

    // 创建工厂类
    ServerBuilder builder;

    // 监听端口
    builder.AddListeningPort (serverAddr, grpc::InsecureServerCredentials());

    // 注册服务
    builder.RegisterService(&service);

    // 创建和启动一个RPC服务器
    std::unique_ptr<Server> server(builder.BuildAndStart());

    std::cout << "server listening on " << serverAddr << std::endl;

    server->Wait();
}


int main (int argc, char* argv[])
{
    RunServer();

    return 0;
}
