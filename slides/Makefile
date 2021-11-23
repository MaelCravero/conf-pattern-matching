HEADER = header.tex
IN = slides.md
OUT = slides.pdf

slides: $(OUT)

$(OUT): $(IN) $(HEADER)
	pandoc -t beamer $(IN) -o $(OUT) --include-in-header $(HEADER)

clean:
	$(RM) $(OUT)

.PHONY: clean slides
