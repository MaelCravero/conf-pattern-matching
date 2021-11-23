slides:
	$(MAKE) -C slides
	cp slides/slides.pdf .

clean:
	$(MAKE) -C slides clean
	$(MAKE) -C code clean

.PHONY: slides clean
