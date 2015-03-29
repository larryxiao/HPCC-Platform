CC = g++

INCLUDES = -I../system/jlib -I../system/include -I/usr/lib/x86_64-linux-gnu/

CFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = 

LIBRARY = /opt/HPCCSystems/lib/libjlib.so /usr/lib/x86_64-linux-gnu/libboost_program_options.so

all: 
	$(CC) $(LDFLAGS) $(CFLAGS) deploy_manager.cpp -o deploy_manager $(LIBRARY) 

env:
	
help: env
	./deploy_manager

server: env
	./deploy_manager --mode 1

client: env
	./deploy_manager --mode 0 --host 127.0.0.1

