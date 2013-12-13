CC=gcc
CSRC=$(shell find . -name '*.c')
OBJS=$(CSRC:.c=.o)

CFLAGS=-m32 -O2 -Wall -Werror -fno-strict-aliasing
DEPEND=$(CC) -MM -I.

x86mm: $(OBJS)
	gcc -o x86mm $(CFLAGS) $(OBJS)

clean:
	-rm x86mm *.o *.d

%.d:%.c Makefile
	@$(SHELL) -ec '$(DEPEND) $< | sed '\''s/\($*\.o\)[ :]*/\1 $@ : /g'\'' > $@'

-include $(CSRC:.c=.d)
