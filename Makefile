CFLAGS = -Wall
CPPFLAGS = -Wall

all: bst avl rbt test-bst test-avl test-rbt

# Create the necessary folders
obj obj/bst obj/avl obj/rbt bin:
	mkdir -p $@

# Object compilation
obj/%.o: src/%.cpp | obj/bst
	g++ $(CPPFLAGS) -c $< -o $@
obj/avl/%.o: src/avl/%.cpp | obj/avl
	g++ $(CPPFLAGS) -c $< -o $@
obj/rbt/%.o: src/rbt/%.cpp | obj/rbt
	g++ $(CPPFLAGS) -c $< -o $@
obj/%.o: src/%.cpp | obj
	g++ $(CPPFLAGS) -c $< -o $@
obj/tinyhttp.o: tinyhttp/http.cpp
	g++ $(CPPFLAGS) -c $< -o $@
obj/unity.o: Unity/src/unity.c
	gcc $(CFLAGS) -c $< -o $@

# Link the CLI executables
bst: obj/bst/bst.o obj/bst/main_bst.o obj/data.o obj/cli.o obj/tree_utils.o obj/tinyhttp.o
	g++ $^ -o $@
avl: obj/avl/avl.o obj/avl/main_avl.o obj/data.o obj/cli.o obj/tree_utils.o obj/tinyhttp.o
	g++ $^ -o $@
rbt: obj/rbt/rbt.o obj/rbt/main_rbt.o obj/data.o obj/cli.o obj/tree_utils.o obj/tinyhttp.o
	g++ $^ -o $@

# Testing executables
test-bst: obj/bst/bst.o obj/bst/test_bst.o obj/unity.o obj/tree_utils.o | bin
	g++ $^ -o bin/$@
	./bin/$@
test-avl: obj/avl/avl.o obj/avl/test_avl.o obj/unity.o obj/tree_utils.o | bin
	g++ $^ -o bin/$@
	./bin/$@
test-rbt: obj/rbt/rbt.o obj/rbt/test_rbt.o obj/unity.o obj/tree_utils.o | bin
	g++ $^ -o bin/$@
	./bin/$@
test-treeutils: obj/test_tree_utils.o obj/unity.o obj/tree_utils.o | bin
	g++ $^ -o bin/$@
	./bin/$@

# Clean up directory
clean:
	rm -fr bin/* obj/* *.o bst avl rbt test-*

.PHONY: clean
