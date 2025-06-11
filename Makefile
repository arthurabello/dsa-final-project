#───────────── Compiler / Flags ─────────────
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -MMD -MP
INC_DIRS  := -Isrc               # header search path

#───────────── Layout ─────────────
OBJ_DIR   := makemagic/obj
BIN_DIR   := makemagic/bin
COMMON_SRC:= src/data.cpp src/tree_utils.cpp

#───────────── BST target ─────────────
BST_SRC   := $(COMMON_SRC) \
             src/bst/bst.cpp \
             src/bst/main_bst.cpp
BST_OBJ   := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(BST_SRC))
BST_BIN   := $(BIN_DIR)/bst

#───────────── AVL target ─────────────
AVL_SRC   := $(COMMON_SRC) \
             src/avl/avl.cpp \
             src/avl/main_avl.cpp
AVL_OBJ   := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(AVL_SRC))
AVL_BIN   := $(BIN_DIR)/avl

#───────────── Dependency file list ─────────
DEPS      := $(BST_OBJ:.o=.d) \
             $(AVL_OBJ:.o=.d)

#───────────── House-keeping ────────────────
.SUFFIXES:                         # disable built-ins

#───────────── Default target ───────────────
all: $(BST_BIN)

#───────────── Pattern rule (*.cpp → *.o) ───
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

#───────────── Link rules ───────────────────
$(BST_BIN): $(BST_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(AVL_BIN): $(AVL_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#───────────── Convenience targets ──────────
build-bst: $(BST_BIN)

build-avl: $(AVL_BIN)

#───────────── Clean target ─────────────────
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

#───────────── Short aliases ─────────────
aliases: $(BST_BIN) $(AVL_BIN)
	ln -sf $(BST_BIN) bst
	ln -sf $(AVL_BIN) avl

#───────────── Phony declarations ─────────
.PHONY: all clean build-bst build-avl aliases

#───────────── Auto-dependencies ────────────
-include $(DEPS)
