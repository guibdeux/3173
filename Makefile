all: fork.c rl.c
	gcc -g -o fork fork.c
	gcc -g -o rl rl.c

clean:
	@rm -rf fork rl &> /dev/null