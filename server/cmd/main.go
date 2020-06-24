package main

import (
	"context"
	"fmt"
	pb "github.com/gasbank/todo/protobuf"
	"github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
	"google.golang.org/grpc"
	"log"
	"net"
	"os"
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
	query := `INSERT INTO todos (todo) VALUES (?)`
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
	log.Println("GetTodoAll called")
	query := `SELECT * FROM todos`
	rows, err := db.Query(query)
	if err != nil {
		return &pb.TodoAllResponse{Items:nil}, err
	}
	var todoList []*pb.Todo
	for rows.Next() {
		var todo *pb.Todo
		err = rows.Scan(&todo)
		todoList = append(todoList, todo)
	}

	return &pb.TodoAllResponse{Items:todoList}, nil
}


func main() {
	testPassword := os.Getenv("TODO_DB_TEST_PASSWORD")
	if len(testPassword) == 0 {
		testPassword = "password"
	}

	var err error
	mysqlConfig := &mysql.Config{
		User:                 "root",
		Passwd:               testPassword,
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
