TARGET=boxcreate boxopen boxclose
LDFLAGS=-lcryptsetup -luuid
all:$(TARGET)

boxcreate: voltool.o  dirutil.o sm3.o boxcreate.o
	$(CC) -o $(@) $(^) $(LDFLAGS)

boxopen: voltool.o dirutil.o sm3.o boxopen.o
	$(CC)  -o $(@) $(^)  $(LDFLAGS)

boxclose: voltool.o dirutil.o sm3.o boxclose.o
	$(CC)  -o $(@) $(^)  $(LDFLAGS)



clean:
	rm -rf $(TARGET) *.o

.phony:  all clean






