programmeP: p.c
	gcc -g -o p p.c

.PHONY: clean

clean: 
	@rm -f p
