cflag = -w

heads = 											\
	-I/usr/include/glib-2.0/						\
	-I.

demo1_src = $(wildcard dbus_name/*.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

demo2_src = $(wildcard dbus_recv_send/*.c)
demo2_obj = $(patsubst %.c, %.o, $(demo2_src))

demo3_src = $(wildcard dbus_service/*.c)
demo3_obj = $(patsubst %.c, %.o, $(demo3_src))

demo4_src = $(wildcard gdbus-*.c)
demo4_obj = $(patsubst %.c, %.run, $(demo4_src))


gdbus_target = $(patsubst %.c, %.run, $(wildcard gdbus_*.c))


gdbus_data = data/gdbus_service1.xml.data			\

gobject_target =									\
	dbus_service/share_service.run					\
	dbus_service/start_service.run					\
	dbus_name/dbus_name.run 

	#dbus_recv_send/listen.run\
	dbus_recv_send/send.run\

target = $(gdbus_data)								\
		 $(gdbus_target)							\
		 $(gobject_target)							\
		 $(demo4_obj)

all:$(target)

gdbus_%.run:gdbus_%.c data/*.c
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs gio-2.0 gio-unix-2.0`

gdbus-%.run:gdbus-%.c 
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs gio-2.0 gio-unix-2.0`

dbus_service/share_service.run:dbus_service/share_service.o
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1`

dbus_service/start_service.run:dbus_service/start_service.o
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1`

dbus_recv_send/listen.run:dbus_recv_send/listen.o
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1 glib-2.0 dbus-glib-1`

dbus_name/dbus_name.run: $(demo1_obj)
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1`

%.o:%.c
	cc -o $@ -c $< $(cflag) $(heads) `pkg-config --cflags --libs dbus-1 glib-2.0 dbus-glib-1`

data/gdbus_service1.xml.data:data/gdbus_service1.xml
	@gdbus-codegen									\
		--interface-prefix es.aleksander			\
		--generate-c-code data/alarm				\
		$<

.PHONY:clean all target

clean:
	@rm -fr *.o
	@rm -fr *.run
	@rm -fr */*.o
	@rm -fr */*.run
