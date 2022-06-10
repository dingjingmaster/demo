cflag =  

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target) tip exec

%.run:%.c
	cc -o $@ `pkg-config --cflags --libs glib-2.0 gio-2.0 polkit-gobject-1 polkit-agent-1` -finput-charset=utf8 $< $(cflag)

.PHONY:clean all install tip exec

tip:
	@echo "===> 请注意，这个例子需要安装之后(sudo make install)执行'test-polkit-demo.run'测试效果 <=="

exec:
	@chmod a+x $(CUR_DIR)/*.run

install:
	cp $(CUR_DIR)/test-polkit-demo.run /usr/bin/
	cp $(CUR_DIR)/org.demo.pkexec.policy /usr/share/polkit-1/actions/

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt
	@rm -f /usr/bin/test-polkit-demo.run
	@rm -f /usr/share/polkit-1/actions/org.demo.pkexec.policy

