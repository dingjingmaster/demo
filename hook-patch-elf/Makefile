all = hook-deepin-editor

$(all):
	@echo ""

hook-deepin-editor:hook-deepin-editor.so

hook-deepin-editor.so:hook-deepin-editor.c
	@g++ -fPIC -shared -L. -lhook-func -o $@ $^
	@echo "CXX -o $@"

clean:
	@rm -f *.o
	@rm -f *.so

.PHONY:all clean
