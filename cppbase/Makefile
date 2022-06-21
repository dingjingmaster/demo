cflag = -w 

CUR_DIR = $(shell pwd)

src_p = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo2-*-p.cpp)))
src2 = $(filter-out $(src_p), $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo2-*.cpp))))
src = $(filter-out $(src2) $(src_p), $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo-*.cpp))))

target2 = $(strip $(patsubst %.cpp, %.run, $(src2)))
target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target) $(target2)

demo2-%.run:demo2-%.cpp $(CUR_DIR)/demo2-%-p.cpp
	g++ -o $@ $^ $(cflag) 

demo-%.run:demo-%.cpp
	g++ -o $@ $< $(cflag) 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

