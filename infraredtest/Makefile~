CC=/usr/local/arm-linux-4.1.1/bin/arm-iwmmxt-linux-gnueabi-gcc

CFLAGS += -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux  \
		-I. -DDEBUG_BUILD

SRC_COMMON = camera.c overlay2.c moduls/panorama.c moduls/stereo.c moduls/labeling.c

SRC_CLIENT = mission_client.c $(SRC_COMMON)
SRC_SERVER = mission_server.c $(SRC_COMMON)

OBJ_CLIENT = ${SRC_CLIENT:.c=.o}
OBJ_SERVER = ${SRC_SERVER:.c=.o}

LDFLAGS += -L. -lm -lpthread

BUILDDIR = bin/

OUT_CLIENT = mission_c
OUT_SERVER = mission_s

all: $(OUT_CLIENT) $(OUT_SERVER)

$(OUT_CLIENT): $(OBJ_CLIENT)
	mkdir -p $(BUILDDIR)
	$(CC) -o $(BUILDDIR)$(OUT_CLIENT) $(OBJ_CLIENT) $(LIBS) $(LDFLAGS) $(CFLAGS)

$(OUT_SERVER): $(OBJ_SERVER)
	mkdir -p $(BUILDDIR)
	$(CC) -o $(BUILDDIR)$(OUT_SERVER) $(OBJ_SERVER) $(LIBS) $(LDFLAGS) $(CFLAGS)

clean: 
	rm -f $(BUILDDIR)$(OUT_CLIENT) $(BUILDDIR)$(OUT_SERVER) *.o *.?~ moduls/*.o moduls/*.?~ 
