
# List of modules to use for compilation
MODULES=vector llist

# Set object paths based on MODULES
STATIC_OBJECTS=$(patsubst %,obj/static/%.o,$(MODULES))
SHARED_OBJECTS=$(patsubst %,obj/shared/%.o,$(MODULES))
BINS=$(patsubst %,bin/%test,$(MODULES))

# Set VPATH based on MODULES
EMPTY :=
SPACE := $(EMPTY) $(EMPTY)
VPATH=$(subst $(SPACE),:,$(patsubst %,src/%,$(MODULES)))

bins: $(BINS)

bin/%test: obj/bin/%.o obj/bin/%test.o
	@mkdir -p bin
	gcc -g -Wall -o $@ $^

cleanbins:
	rm -rf bin/* obj/bin/*

lib: lib/libcdatastructures.a lib/libcdatastructures.so

lib/libcdatastructures.a: $(STATIC_OBJECTS)
	@mkdir -p lib
	ar rcs $@ $^

lib/libcdatastructures.so: $(SHARED_OBJECTS)
	@mkdir -p lib
	gcc -shared -o $@ $^

obj/static/%.o: %.c
	@mkdir -p obj/static
	gcc -o $@ -c $<

obj/bin/%.o: %.c
	@mkdir -p obj/bin
	gcc -g -Wall -o $@ -c $<

obj/shared/%.o: %.c
	@mkdir -p obj/shared
	gcc -fPIC -o $@ -c $<

cleanlib:
	rm -rf lib/* obj/shared/* obj/static/*

clean: cleanbins cleanlib

