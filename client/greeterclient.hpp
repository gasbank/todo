#pragma once

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::Status;
using helloworld::Greeter;

#include <vector>

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel);
    std::string SayHello(const std::string& user);
    std::string AddNewTodo(const std::string& user);

private:
    std::unique_ptr<Greeter::Stub> stub_;
};
