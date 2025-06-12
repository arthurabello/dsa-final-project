#───────────── Compiler / Flags ─────────────
CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -O2 -MMD -MP
INC_DIRS   := -Isrc -IUnity/src          # project headers + Unity

#───────────── Layout ───────────────────────
OBJ_DIR    := makemagic/obj
BIN_DIR    := makemagic/bin
COMMON_SRC := src/data.cpp src/tree_utils.cpp 

#───────────── BST target ───────────────────
BST_SRC    := $(COMMON_SRC) \
              src/bst/bst.cpp \
              src/bst/main_bst.cpp \
              src/cli.cpp \
              tinyhttp/http.cpp
BST_OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(filter src/%.cpp,$(BST_SRC))) \
           $(OBJ_DIR)/tinyhttp/http.o
BST_BIN    := bst                              # final executable lives at repo root

#───────────── AVL target ───────────────────
AVL_SRC    := $(COMMON_SRC) \
              src/avl/avl.cpp \
              src/avl/main_avl.cpp \
              src/cli.cpp \
              tinyhttp/http.cpp
AVL_OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(filter src/%.cpp,$(AVL_SRC))) \
           $(OBJ_DIR)/tinyhttp/http.o
AVL_BIN    := avl

#───────────── Unit-test targets ────────────
UNITY_SRC           := Unity/src/unity.c

BST_TEST_SRC        := $(COMMON_SRC) src/bst/bst.cpp src/bst/test_bst.cpp $(UNITY_SRC)
AVL_TEST_SRC        := $(COMMON_SRC) src/avl/avl.cpp src/avl/test_avl.cpp $(UNITY_SRC)
TREEUTILS_TEST_SRC  := $(COMMON_SRC) src/tree_utils/test_tree_utils.cpp $(UNITY_SRC)

BST_TEST_BIN        := $(BIN_DIR)/bst_tests
AVL_TEST_BIN        := $(BIN_DIR)/avl_tests
TREEUTILS_TEST_BIN  := $(BIN_DIR)/treeutils_tests

#───────────── Dependency files ─────────────
DEPS := $(BST_OBJ:.o=.d) $(AVL_OBJ:.o=.d)

#───────────── House-keeping ────────────────
.SUFFIXES:                              

#───────────── Default target ───────────────
all: $(BST_BIN)

#───────────── Pattern rule (*.cpp → *.o) ───
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

$(OBJ_DIR)/tinyhttp/%.o: tinyhttp/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

#───────────── Link rules ───────────────────
$(BST_BIN): $(BST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(AVL_BIN): $(AVL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

#───────────── Unit-test build + run ────────
$(BIN_DIR):
	@mkdir -p $@

#— BST tests —
test-bst: $(BST_TEST_BIN)
	@echo "🔍  Running BST test-suite…"
	@$(BST_TEST_BIN)

$(BST_TEST_BIN): $(BST_TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) \
	       -x c $(UNITY_SRC) -x c++ $(filter-out $(UNITY_SRC),$^) \
	       -o $@

#— AVL tests —
test-avl: $(AVL_TEST_BIN)
	@echo "🔍  Running AVL test-suite…"
	@$(AVL_TEST_BIN)

$(AVL_TEST_BIN): $(AVL_TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) \
	       -x c $(UNITY_SRC) -x c++ $(filter-out $(UNITY_SRC),$^) \
	       -o $@

#— Tree-Utils tests —
test-treeutils: $(TREEUTILS_TEST_BIN)
	@echo "🔍  Running Tree-Utils test-suite…"
	@$(TREEUTILS_TEST_BIN)

$(TREEUTILS_TEST_BIN): $(TREEUTILS_TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) \
	       -x c $(UNITY_SRC) -x c++ $(filter-out $(UNITY_SRC),$^) \
	       -o $@

#───────────── Convenience targets ──────────
build-bst: $(BST_BIN)
build-avl: $(AVL_BIN)

aliases: $(BST_BIN) $(AVL_BIN)
	ln -sf $(BST_BIN) bst
	ln -sf $(AVL_BIN) avl

#───────────── Clean target ─────────────────
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) bst avl

#───────────── Phony declarations ───────────
.PHONY: all clean build-bst build-avl aliases \
        test-bst test-avl test-treeutils

#───────────── Auto-dependencies ────────────
-include $(DEPS)
