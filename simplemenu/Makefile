EXE = simplemenu
TARGET ?= pc

# Exclude system drivers and internal unsqfs (using Paul's instead)
SRCS = $(shell find src -name "*.c" ! -name "system_logic_*.c" ! -name "control_*.c" ! -name "unsqfs.c")

ifeq ($(TARGET), pc)
    SRCS += src/logic/system_logic_od.c src/logic/control_od.c
    CFLAGS += $(shell pkg-config --cflags sdl SDL_image SDL_ttf SDL_mixer) -DPC
    LIBS    = $(shell pkg-config --libs sdl SDL_image SDL_ttf SDL_mixer) -lpng -ljpeg -lz -lm -lpthread
else
    LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lpng -ljpeg -lz -lm -pthread
    ifeq ($(TARGET), miyoo)
        SRCS += src/logic/system_logic_bittboy.c src/logic/control_bittboy.c
        CFLAGS += -DMIYOO
    else ifeq ($(TARGET), retrofw)
        SRCS += src/logic/system_logic_rfw.c src/logic/control_rfw.c
        CFLAGS += -DRETROFW
    else ifeq ($(TARGET), rg350)
        SRCS += src/logic/system_logic_od.c src/logic/control_od.c
        CFLAGS += -DRG350
        LIBS += -lshake
    endif
endif

SRCS += $(EXTRA_SRCS)
OBJS = $(SRCS:.c=.o)
CC  = $(CROSS_COMPILE)gcc
#CFLAGS += -Wall -Wextra -O2 -std=gnu99
CFLAGS += -w -O2 -std=gnu99

ifneq ($(TARGET), pc)
    ifneq ($(SYSROOT),)
        CFLAGS  += --sysroot=$(SYSROOT)
        LDFLAGS += --sysroot=$(SYSROOT)
    endif
endif

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete
	rm -f $(EXE)
