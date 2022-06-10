all:
	@cd gobject/ && make all
	@cd gtk/ && make all
	@cd ipc/ && make all
	@cd glib/ && make all
	@cd gio/ && make all
	@cd dbus/ && make all
	@cd cairo/ && make all
	@cd pam/ && make all
	@cd sl/ && make all

.PHONY:clean 

clean:
	@cd gobject/ && make clean
	@cd gtk/ && make clean
	@cd ipc/ && make clean
	@cd glib/ && make clean
	@cd gio/ && make clean
	@cd dbus/ && make clean
	@cd cairo/ && make clean
	@cd pam/ && make clean
	@cd sl/ && make clean
