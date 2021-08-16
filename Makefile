programmes: p.c pCopy.c
	gcc -g -o p p.c
	gcc -g -o pCopy pCopy.c

.PHONY: clean

clean: 
	@rm -f p pCopy
