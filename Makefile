programmeP: p.c
	gcc -g -o p p.c

.PHONY: clean./p

clean: rm -f p