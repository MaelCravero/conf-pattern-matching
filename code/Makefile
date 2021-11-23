CXXFLAGS = -Wall -Wextra -pedantic -std=c++20

OUT = \
      minmax_warnings \
      warnings \
      templates-in-a-nutshell.ll

%.ll: %.cc
	clang++ -emit-llvm -S $^

.PHONY: clean

clean:
	$(RM) $(OUT)
