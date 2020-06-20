#pragma once

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

#include <vector>

#ifdef BAZEL_BUILD
#include "examples/protos/todolist.grpc.pb.h"
#else
#include "todolist.grpc.pb.h"
#endif

class TodoListClient {
public:
    TodoListClient(std::shared_ptr<Channel> channel);
    std::string AddTodo(const std::string& todoStr);
    std::string RemoveTodo(const std::string& todoStr);
    std::vector<std::string> GetTodoAll();
private:
    std::unique_ptr<protobuf::Todolist::Stub> stub_;
};
