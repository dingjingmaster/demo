cur_dir = $(shell pwd)

src = $(strip $(subst $(cur_dir), ., $(wildcard $(cur_dir)/*.cc)))

target = $(strip $(patsubst %.cc, %.run, $(src)))

# 自己编译安装
lib_dir = $(shell dirname $(shell find /usr/lib -name "libprotobuf.a"))
inc_dir = $(shell find $(shell dirname $(lib_dir))/include -name "protobuf")

flags1 = -I/usr/local/include -L$(lib_dir) -lprotobuf -Wl,-rpath,$(lib_dir)
flags2 = `pkg-config --cflags --libs protobuf`

all: $(target)


%.run : %.cc %.pb.cc
	g++ -o $@ $^ $(flags2)


%.pb.cc : %.proto
	/usr/bin/protoc --cpp_out=$(cur_dir) -I$(cur_dir) $<


clean:
	rm -f *.run
	rm -f *.pb.h
	rm -f *.pb.cc 
