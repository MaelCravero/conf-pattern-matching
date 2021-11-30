HEADER = header.tex
META = 00_metadata.md
SRC = \
	  01_introduction.md \
	  02_cpp_basics.md \
	  03_pattern_matching.md \
	  04_back_to_cpp.md \
	  05_hacking_cpp.md \
	  06_conclusion.md

OUT = slides.pdf

slides: $(OUT)

$(OUT): $(META) $(SRC) $(HEADER)
	pandoc -t beamer $(META) $(SRC) -o $(OUT) --include-in-header $(HEADER)

0%: 0%.md
	pandoc -t beamer $(META) $^ -o $(OUT) --include-in-header $(HEADER)

clean:
	$(RM) $(OUT)

.PHONY: clean slides
