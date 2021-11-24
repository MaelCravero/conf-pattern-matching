CXXFLAGS = -Wall -Wextra -pedantic -std=c++20

OUT = \
      cpp-basics/hello \
      cpp-basics/subtypes \
      cpp-basics/static-vs-dynamic \
      cpp-basics/template-instanciation.ll \
      match_tree \
      match_tree_ref \
      minmax_warnings \
      warnings

all: $(OUT)

%.ll: %.cc
	clang++ -emit-llvm -S $^ -o $@

.PHONY: clean

clean:
	$(RM) $(OUT)
