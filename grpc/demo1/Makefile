cur_dir = $(shell pwd)
pkg_path = $(shell dirname $(shell find /usr/local -name "grpc.pc"))

all:gen server.run client.run


server.run : server.cpp msg.grpc.pb.cc msg.pb.cc
	g++ $^ -L/usr/local/lib `pkg-config --with-path=$(pkg_path) --cflags --libs grpc++ protobuf` -lpthread -lgrpc++_reflection -ldl -o $@

client.run : client.cpp msg.grpc.pb.cc msg.pb.cc
	g++ $^ -L/usr/local/lib `pkg-config --with-path=$(pkg_path) --cflags --libs grpc++ protobuf` -lpthread -lgrpc++_reflection -ldl -o $@


gen:
	# 生成 `msg.grpc.pb` 服务类 
	protoc -I $(cur_dir) --grpc_out=$(cur_dir) --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` msg.proto
	# 生成 `msg.pb` 消息类
	protoc -I $(cur_dir) --cpp_out=$(cur_dir) msg.proto


clean:
	@rm -f $(cur_dir)/*.o
	@rm -f $(cur_dir)/*.run
	@rm -f $(cur_dir)/*.pb.h
	@rm -f $(cur_dir)/*.pb.cc

