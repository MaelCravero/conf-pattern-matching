CXXFLAGS = -Wall -Wextra -pedantic -std=c++20
CFLAGS = -Wall -Wextra -pedantic -std=c99

OUT = \
      cpp-basics/hello \
      cpp-basics/subtypes \
      cpp-basics/static-vs-dynamic \
      cpp-basics/template-instanciation.ll \
      cpp-matching/the-answer \
      cpp-matching/the-answer_c \
      cpp-matching/the-answer_c.s \
      cpp-matching/product-and-sum \
      cpp-matching/static-or-dynamic \
      cpp-matching/template-lambda-visit \
      cpp-matching/first-matcher \
      cpp-matching/match-tree \
      cpp-matching-hacks/match-tree-step1 \
      cpp-matching-hacks/match-tree-step2 \
      cpp-matching-hacks/match-tree-step3 \
      cpp-matching-hacks/match-tree-step4 \
      cpp-matching-hacks/match-tree-step5 \
      cpp-matching-hacks/match-tree-step6 \
      cpp-matching-hacks/match-tree-upcast \
      cpp-matching-hacks/match-tree-upcast-ref \
      cpp-matching-hacks/minmax \
      cpp-matching-hacks/warnings \
      match_tree \
      match_tree_ref \
      minmax_warnings \
      warnings

all: $(OUT)

%.ll: %.cc
	clang++ -emit-llvm -S $^ -o $@

%.s: %.c
	$(CC) -S $^ -o $@

.PHONY: clean

clean:
	$(RM) $(OUT)
