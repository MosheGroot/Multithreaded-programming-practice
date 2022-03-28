CC			:= g++
CFLAGS	:= -std=c++17 -Wall -Wextra -Werror -g

all:		client sender

client	: Client.cpp Settings.hpp
	$(CC) $(CFALGS) Client.cpp -o $@

sender	: Sender.cpp Settings.hpp
	$(CC) $(CFALGS) Sender.cpp -o $@

clean:
	rm -rf client sender

re:	clean all
	