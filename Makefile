CC = gcc
CFLAGS = -Wall -g
LIBS = -lorcon2
INCLUDES=
TARGET = presp
SRCS = main.c
OBJS = $(SRCS:.c=.o)

.PHONY: depend clean

all: $(TARGET)
	@echo Compiling something!

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# hello
# DO NOT DELETE

main.o: /usr/include/stdio.h /usr/include/features.h
main.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/getopt.h /usr/include/string.h /usr/include/xlocale.h
main.o: /usr/include/pthread.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/sched.h /usr/include/time.h /usr/include/bits/sched.h
main.o: /usr/include/signal.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
main.o: /usr/include/stdlib.h /usr/include/sys/types.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
main.o: /usr/include/alloca.h /usr/include/sys/time.h
