package main

import (
	"context"
	"fmt"
	"github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
	"log"
	"net"

	"google.golang.org/grpc"
	pb "github.com/gasbank/todo/protobuf"
)

const (
port = ":50051"
)

var db *sqlx.DB

type server struct {
	pb.UnimplementedTodolistServer
}

func (s *server) AddTodo(ctx context.Context, in *pb.TodoRequest) (*pb.TodoReply, error) {
	log.Printf("Received: %v", in.GetItem().GetTodo())
	query := `INSERT todos VALUE ?`
	_, err := db.Exec(query, in.GetItem().GetTodo())
	if err != nil {
		return &pb.TodoReply{ResponseCode: "50000" + in.GetItem().GetTodo()}, err
	}
	return &pb.TodoReply{ResponseCode: "20000" + in.GetItem().GetTodo()}, nil
}
func (s *server) RemoveTodo(ctx context.Context, in *pb.TodoRequest) (*pb.TodoReply, error) {
	log.Printf("Received: %v", in.GetItem().GetTodo())
	query := `DELETE FROM todos WHERE todo=?`
	_, err := db.Exec(query, in.GetItem().GetTodo())
	if err != nil {
		return &pb.TodoReply{ResponseCode: "50000" + in.GetItem().GetTodo()}, err
	}
	return &pb.TodoReply{ResponseCode: "Hello again " + in.GetItem().GetTodo()}, nil
}
func (s *server) GetTodoAll(ctx context.Context, in *pb.EmptyRequest) (*pb.TodoAllResponse, error) {
	log.Println("Received: %v")
	return &pb.TodoAllResponse{}, nil
}


func main() {

	var err error
	mysqlConfig := &mysql.Config{
		User:                 "root",
		Passwd:               "password",
		Net:                  "tcp",
		Addr:                 fmt.Sprintf("%s:%s", "127.0.0.1", "3306"),
		DBName:               "todo_db",
		AllowNativePasswords: true,
	}

	dsn := mysqlConfig.FormatDSN()

	db, err = sqlx.Connect("mysql", dsn)
	if err != nil {
		fmt.Printf("err in %v: %v", db, err)
	}

	fmt.Println("started")
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
