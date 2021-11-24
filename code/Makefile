CXXFLAGS = -Wall -Wextra -pedantic -std=c++20

OUT = \
      match_tree \
      match_tree_ref \
      minmax_warnings \
      warnings \
      templates-in-a-nutshell.ll

all: $(OUT)

%.ll: %.cc
	clang++ -emit-llvm -S $^

.PHONY: clean

clean:
	$(RM) $(OUT)
