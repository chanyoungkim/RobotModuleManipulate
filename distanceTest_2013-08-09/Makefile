CC=/arm-linux-4.1.1/bin/arm-iwmmxt-linux-gnueabi-gcc -lm -pthread

CFLAGS += -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux  \
		-I./ -DDEBUG_BUILD

SOURCES = camera.c testDistance.c overlay2.c moduls/stereo.c

OBJECTS = ${SOURCES:.c=.o}

LFLAGS += -lpthread

OUT = distanceTest2

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJECTS) $(LIBS)

clean: 
	rm -f line_tracer *.o *.?~ moduls/*.o moduls/*.?~ 
