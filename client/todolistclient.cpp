/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "todolistclient.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using protobuf::Todolist;
using protobuf::TodoRequest;
using protobuf::EmptyRequest;
using protobuf::TodoReply;
using protobuf::Todo;
using protobuf::TodoAllResponse;

TodoListClient::TodoListClient(std::shared_ptr<Channel> channel)
    : stub_(Todolist::NewStub(channel)) {}

// Assembles the client's payload, sends it and presents the response back
// from the server.
std::string TodoListClient::AddTodo(const std::string& todoStr) {
    // Data we are sending to the server.
    TodoRequest request;

    Todo* todo = new Todo();
    todo->set_todo(strdup(todoStr.c_str()));
    request.set_allocated_item(todo);

    // Container for the data we expect from the server.
    TodoReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->addTodo(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.responsecode();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";
    }
}

std::string TodoListClient::RemoveTodo(const std::string& todoStr) {
    // Data we are sending to the server.
    TodoRequest request;

    Todo* todo = new Todo();
    todo->set_todo(todoStr.c_str());
    request.set_allocated_item(todo);

    // Container for the data we expect from the server.
    TodoReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->RemoveTodo(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        return reply.responsecode();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";
    }
}
    
std::vector<std::string> TodoListClient::GetTodoAll() {
    // Data we are sending to the server.
    EmptyRequest request;

    // Container for the data we expect from the server.
    TodoAllResponse reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->getTodoAll(&context, request, &reply);

    std::vector<std::string> todoList;
    // Act upon its status.
    if (status.ok()) {
        for (int i = 0; i < reply.items_size(); i++)
        {   
            todoList.push_back(reply.items(i).todo());
        }
            
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        todoList.push_back("RPC failed");
    }
    return todoList;
}

int grpcworld_todolist_main(int argc, char** argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    std::string target_str;
    std::string arg_str("--target");
    if (argc > 1) {
        std::string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != std::string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                target_str = arg_val.substr(start_pos + 1);
            }
            else {
                std::cout << "The only correct argument syntax is --target=" << std::endl;
                return 0;
            }
        }
        else {
            std::cout << "The only acceptable argument is --target=" << std::endl;
            return 0;
        }
    }
    else {
        //target_str = "localhost:50051";
        target_str = "192.168.35.118:50051";
    }

    TodoListClient todoList(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string todoItem("hohoho");
    std::string reply = todoList.AddTodo(todoItem);
    std::cout << "TodoReply received: " << reply << std::endl;

    return 0;
}
