
sockets: srv_chat.c cli.c srv_echo.c srv_poll.c
	@gcc -o srv_chat srv_chat.c
	@gcc -o cli cli.c
	@gcc -o srv_echo srv_echo.c
	@gcc -o srv_poll srv_poll.c

.PHONY: clean

clean:
	@rm -rf srv_poll srv_echo cli srv_chat
