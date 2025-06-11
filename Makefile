#───────────── Compiler / Flags ─────────────
CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -O2 -MMD -MP
INC_DIRS   := -Isrc -IUnity/src    

#───────────── Layout ─────────────
OBJ_DIR   := makemagic/obj
BIN_DIR   := makemagic/bin
COMMON_SRC:= src/data.cpp src/tree_utils.cpp src/cli.cpp

#──────────── BST target ───────────────────
BST_SRC    := $(COMMON_SRC) \
              src/bst/bst.cpp \
              src/bst/main_bst.cpp
BST_OBJ    := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(BST_SRC))
BST_BIN    := $(BIN_DIR)/bst

#───────────── AVL target ───────────────────
AVL_SRC    := $(COMMON_SRC) \
              src/avl/avl.cpp \
              src/avl/main_avl.cpp
AVL_OBJ    := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(AVL_SRC))
AVL_BIN    := $(BIN_DIR)/avl

#───────────── Unit-test targets ────────────
UNITY_SRC      := Unity/src/unity.c

BST_TEST_SRC   := $(COMMON_SRC) src/bst/bst.cpp src/bst/test_bst.cpp $(UNITY_SRC)
AVL_TEST_SRC   := $(COMMON_SRC) src/avl/avl.cpp src/avl/test_avl.cpp $(UNITY_SRC)

BST_TEST_BIN   := $(BIN_DIR)/bst_tests
AVL_TEST_BIN   := $(BIN_DIR)/avl_tests

#───────────── Dependency file list ─────────
DEPS       := $(BST_OBJ:.o=.d) \
              $(AVL_OBJ:.o=.d)

#───────────── House-keeping ────────────────
.SUFFIXES:                              

#───────────── Default target ───────────────
all: $(BST_BIN) aliases

#───────────── Pattern rule (*.cpp → *.o) ───
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

#──────────── Link rules ───────────────────
$(BST_BIN): $(BST_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(AVL_BIN): $(AVL_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#───────────── Unit-test build + run ────────
$(BIN_DIR):
	@mkdir -p $@

test-bst: $(BST_TEST_BIN)
	@echo "🔍  Running BST test-suite…"
	@$(BST_TEST_BIN)

$(BST_TEST_BIN): $(BST_TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) \
	       -x c $(UNITY_SRC) -x c++ $(filter-out $(UNITY_SRC),$^) \
	       -o $@

test-avl: $(AVL_TEST_BIN)
	@echo "🔍  Running AVL test-suite…"
	@$(AVL_TEST_BIN)

$(AVL_TEST_BIN): $(AVL_TEST_SRC) | $(BIN_DIR)
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
.PHONY: all clean build-bst build-avl aliases test-bst test-avl

#───────────── Auto-dependencies ────────────
-include $(DEPS)
