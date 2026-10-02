build/libsds.a: build/dynamic_array.o build/object_manager.o
	ar rcs build/libsds.a build/dynamic_array.o build/object_manager.o

build/dynamic_array.o: src/dynamic_array.c include/dynamic_array.h include/macros.h
	mkdir -p build
	gcc -Iinclude -c src/dynamic_array.c -o build/dynamic_array.o -Wno-free-nonheap-object

build/object_manager.o: src/object_manager.c include/object_manager.h include/macros.h
	mkdir -p build
	gcc -Iinclude -c src/object_manager.c -o build/object_manager.o -Wno-free-nonheap-object

clean:
	rm -r build