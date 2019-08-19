# Software Name
PROGRAM = simplemenu.elf
#PLATFORM = BITTBOY
PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
CC = /opt/bittboy-toolchain/bin/arm-miyoo-linux-musleabi-gcc
else ifeq ($(PROFILE), RS97)
CC = /opt/rs97-toolchain/bin/mipsel-linux-gcc
else
CC = gcc
endif

# You can use Ofast too but it can be more prone to bugs, careful.
CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie
CFLAGS +=  -I.

LIBS += -lasound -lSDL_mixer -lmikmod -lvorbisidec -logg -lSDL -lm -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz -lstdc++
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

ifeq ($(DEBUG), YES)
CFLAGS +=  -DDEBUG -g3
else
LDFLAGS	+=  -s
endif

CFILES =  simplemenu.c string_utils.c graphics.c screen.c control.c logic.c system_logic.c config.c
SFILES = 

OFILES = $(SFILES:.S=.o) $(CFILES:.c=.o)

$(PROGRAM):	$(OFILES)
			$(CC) $(CFLAGS) $(OFILES) -o $@ $(LDFLAGS)

all: $(PROGRAM)

%.o: %.c
	 $(CC) $(CFLAGS) -c $< -o $@

clean:
	 -rm -f $(OFILES) $(MAPFILE) $(PROGRAM)