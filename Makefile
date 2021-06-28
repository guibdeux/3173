all: fork.c rl.c
	gcc -g -o fork fork.c
	gcc -g -o rl rl.c
	gcc -g -o ami ami.c


clean:
	@rm -rf fork ami rl &> /dev/null