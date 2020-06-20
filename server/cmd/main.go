package main

import (
"context"
"log"
"net"

"google.golang.org/grpc"
pb "github.com/gasbank/todo/protobuf"
)

const (
port = ":50051"
)

type server struct {
	pb.UnimplementedTodolistServer
}

func (s *server) addTodo(ctx context.Context, in *pb.TodoRequest) (*pb.TodoReply, error) {
	log.Printf("Received: %v", in.GetItem())
	return &pb.TodoReply{ReponseCode: "Hello " + in.GetItem()}, nil
}
func (s *server) removeTodo(ctx context.Context, in *pb.TodoRequest) (*pb.TodoReply, error) {
	return &pb.TodoReply{ReponseCode: "Hello again " + in.GetItem()}, nil
}

func main() {
	lis, err := net.Listen("tcp", port)
	if err != nil {
	log.Fatalf("failed to listen: %v", err)
	}
	s := grpc.NewServer()
	pb.RegisterTodolistServer(s, &server{})
	if err := s.Serve(lis); err != nil {
	log.Fatalf("failed to serve: %v", err)
	}
}
