

SRC:=$(wildcard *.c)
OBJ:=$(patsubst %.c,%.o,$(SRC))
DST:=$(patsubst %.c,%,$(SRC))
CC:=gcc

.PHONY: all clean
	
all:$(DST)

$(DST):%:%.o
	$(CC) $<  -o $@
	
$(OBJ):%.o:%.c
	$(CC) $< -c -o $@
	

clean:
	@rm -rf $(DST)
	@rm -rf $(OBJ)

