// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.24.0
// 	protoc        v3.11.2
// source: protobuf/todolist.proto

package helloworld

import (
	context "context"
	proto "github.com/golang/protobuf/proto"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

// This is a compile-time assertion that a sufficiently up-to-date version
// of the legacy proto package is being used.
const _ = proto.ProtoPackageIsVersion4

// The request message containing the user's name.
type TodoRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Item string `protobuf:"bytes,1,opt,name=item,proto3" json:"item,omitempty"`
}

func (x *TodoRequest) Reset() {
	*x = TodoRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protobuf_todolist_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TodoRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TodoRequest) ProtoMessage() {}

func (x *TodoRequest) ProtoReflect() protoreflect.Message {
	mi := &file_protobuf_todolist_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TodoRequest.ProtoReflect.Descriptor instead.
func (*TodoRequest) Descriptor() ([]byte, []int) {
	return file_protobuf_todolist_proto_rawDescGZIP(), []int{0}
}

func (x *TodoRequest) GetItem() string {
	if x != nil {
		return x.Item
	}
	return ""
}

// The response message containing the greetings
type TodoReply struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	ReponseCode string `protobuf:"bytes,1,opt,name=reponseCode,proto3" json:"reponseCode,omitempty"`
}

func (x *TodoReply) Reset() {
	*x = TodoReply{}
	if protoimpl.UnsafeEnabled {
		mi := &file_protobuf_todolist_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TodoReply) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TodoReply) ProtoMessage() {}

func (x *TodoReply) ProtoReflect() protoreflect.Message {
	mi := &file_protobuf_todolist_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TodoReply.ProtoReflect.Descriptor instead.
func (*TodoReply) Descriptor() ([]byte, []int) {
	return file_protobuf_todolist_proto_rawDescGZIP(), []int{1}
}

func (x *TodoReply) GetReponseCode() string {
	if x != nil {
		return x.ReponseCode
	}
	return ""
}

var File_protobuf_todolist_proto protoreflect.FileDescriptor

var file_protobuf_todolist_proto_rawDesc = []byte{
	0x0a, 0x17, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2f, 0x74, 0x6f, 0x64, 0x6f, 0x6c,
	0x69, 0x73, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x08, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x62, 0x75, 0x66, 0x22, 0x21, 0x0a, 0x0b, 0x54, 0x6f, 0x64, 0x6f, 0x52, 0x65, 0x71, 0x75, 0x65,
	0x73, 0x74, 0x12, 0x12, 0x0a, 0x04, 0x69, 0x74, 0x65, 0x6d, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09,
	0x52, 0x04, 0x69, 0x74, 0x65, 0x6d, 0x22, 0x2d, 0x0a, 0x09, 0x54, 0x6f, 0x64, 0x6f, 0x52, 0x65,
	0x70, 0x6c, 0x79, 0x12, 0x20, 0x0a, 0x0b, 0x72, 0x65, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x43, 0x6f,
	0x64, 0x65, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0b, 0x72, 0x65, 0x70, 0x6f, 0x6e, 0x73,
	0x65, 0x43, 0x6f, 0x64, 0x65, 0x32, 0x7f, 0x0a, 0x08, 0x54, 0x6f, 0x64, 0x6f, 0x6c, 0x69, 0x73,
	0x74, 0x12, 0x37, 0x0a, 0x07, 0x61, 0x64, 0x64, 0x54, 0x6f, 0x64, 0x6f, 0x12, 0x15, 0x2e, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2e, 0x54, 0x6f, 0x64, 0x6f, 0x52, 0x65, 0x71, 0x75,
	0x65, 0x73, 0x74, 0x1a, 0x13, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2e, 0x54,
	0x6f, 0x64, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x22, 0x00, 0x12, 0x3a, 0x0a, 0x0a, 0x52, 0x65,
	0x6d, 0x6f, 0x76, 0x65, 0x54, 0x6f, 0x64, 0x6f, 0x12, 0x15, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x62, 0x75, 0x66, 0x2e, 0x54, 0x6f, 0x64, 0x6f, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x1a,
	0x13, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2e, 0x54, 0x6f, 0x64, 0x6f, 0x52,
	0x65, 0x70, 0x6c, 0x79, 0x22, 0x00, 0x42, 0x67, 0x0a, 0x1b, 0x69, 0x6f, 0x2e, 0x67, 0x72, 0x70,
	0x63, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x73, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f,
	0x77, 0x6f, 0x72, 0x6c, 0x64, 0x42, 0x0f, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x57, 0x6f, 0x72, 0x6c,
	0x64, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x50, 0x01, 0x5a, 0x35, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65,
	0x2e, 0x67, 0x6f, 0x6c, 0x61, 0x6e, 0x67, 0x2e, 0x6f, 0x72, 0x67, 0x2f, 0x67, 0x72, 0x70, 0x63,
	0x2f, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x73, 0x2f, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77,
	0x6f, 0x72, 0x6c, 0x64, 0x2f, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x62,
	0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_protobuf_todolist_proto_rawDescOnce sync.Once
	file_protobuf_todolist_proto_rawDescData = file_protobuf_todolist_proto_rawDesc
)

func file_protobuf_todolist_proto_rawDescGZIP() []byte {
	file_protobuf_todolist_proto_rawDescOnce.Do(func() {
		file_protobuf_todolist_proto_rawDescData = protoimpl.X.CompressGZIP(file_protobuf_todolist_proto_rawDescData)
	})
	return file_protobuf_todolist_proto_rawDescData
}

var file_protobuf_todolist_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_protobuf_todolist_proto_goTypes = []interface{}{
	(*TodoRequest)(nil), // 0: protobuf.TodoRequest
	(*TodoReply)(nil),   // 1: protobuf.TodoReply
}
var file_protobuf_todolist_proto_depIdxs = []int32{
	0, // 0: protobuf.Todolist.addTodo:input_type -> protobuf.TodoRequest
	0, // 1: protobuf.Todolist.RemoveTodo:input_type -> protobuf.TodoRequest
	1, // 2: protobuf.Todolist.addTodo:output_type -> protobuf.TodoReply
	1, // 3: protobuf.Todolist.RemoveTodo:output_type -> protobuf.TodoReply
	2, // [2:4] is the sub-list for method output_type
	0, // [0:2] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_protobuf_todolist_proto_init() }
func file_protobuf_todolist_proto_init() {
	if File_protobuf_todolist_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_protobuf_todolist_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TodoRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_protobuf_todolist_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TodoReply); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_protobuf_todolist_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_protobuf_todolist_proto_goTypes,
		DependencyIndexes: file_protobuf_todolist_proto_depIdxs,
		MessageInfos:      file_protobuf_todolist_proto_msgTypes,
	}.Build()
	File_protobuf_todolist_proto = out.File
	file_protobuf_todolist_proto_rawDesc = nil
	file_protobuf_todolist_proto_goTypes = nil
	file_protobuf_todolist_proto_depIdxs = nil
}

// Reference imports to suppress errors if they are not otherwise used.
var _ context.Context
var _ grpc.ClientConnInterface

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
const _ = grpc.SupportPackageIsVersion6

// TodolistClient is the client API for Todolist service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://godoc.org/google.golang.org/grpc#ClientConn.NewStream.
type TodolistClient interface {
	// Sends a greeting
	AddTodo(ctx context.Context, in *TodoRequest, opts ...grpc.CallOption) (*TodoReply, error)
	// Sends another greeting
	RemoveTodo(ctx context.Context, in *TodoRequest, opts ...grpc.CallOption) (*TodoReply, error)
}

type todolistClient struct {
	cc grpc.ClientConnInterface
}

func NewTodolistClient(cc grpc.ClientConnInterface) TodolistClient {
	return &todolistClient{cc}
}

func (c *todolistClient) AddTodo(ctx context.Context, in *TodoRequest, opts ...grpc.CallOption) (*TodoReply, error) {
	out := new(TodoReply)
	err := c.cc.Invoke(ctx, "/protobuf.Todolist/addTodo", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *todolistClient) RemoveTodo(ctx context.Context, in *TodoRequest, opts ...grpc.CallOption) (*TodoReply, error) {
	out := new(TodoReply)
	err := c.cc.Invoke(ctx, "/protobuf.Todolist/RemoveTodo", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// TodolistServer is the server API for Todolist service.
type TodolistServer interface {
	// Sends a greeting
	AddTodo(context.Context, *TodoRequest) (*TodoReply, error)
	// Sends another greeting
	RemoveTodo(context.Context, *TodoRequest) (*TodoReply, error)
}

// UnimplementedTodolistServer can be embedded to have forward compatible implementations.
type UnimplementedTodolistServer struct {
}

func (*UnimplementedTodolistServer) AddTodo(context.Context, *TodoRequest) (*TodoReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method AddTodo not implemented")
}
func (*UnimplementedTodolistServer) RemoveTodo(context.Context, *TodoRequest) (*TodoReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method RemoveTodo not implemented")
}

func RegisterTodolistServer(s *grpc.Server, srv TodolistServer) {
	s.RegisterService(&_Todolist_serviceDesc, srv)
}

func _Todolist_AddTodo_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(TodoRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(TodolistServer).AddTodo(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/protobuf.Todolist/AddTodo",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(TodolistServer).AddTodo(ctx, req.(*TodoRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _Todolist_RemoveTodo_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(TodoRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(TodolistServer).RemoveTodo(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/protobuf.Todolist/RemoveTodo",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(TodolistServer).RemoveTodo(ctx, req.(*TodoRequest))
	}
	return interceptor(ctx, in, info, handler)
}

var _Todolist_serviceDesc = grpc.ServiceDesc{
	ServiceName: "protobuf.Todolist",
	HandlerType: (*TodolistServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "addTodo",
			Handler:    _Todolist_AddTodo_Handler,
		},
		{
			MethodName: "RemoveTodo",
			Handler:    _Todolist_RemoveTodo_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "protobuf/todolist.proto",
}
