# Software Name
PROGRAM = simplemenu.elf
#PLATFORM = BITTBOY
PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
CC = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
else ifeq ($(PROFILE), RS97)
CC = /opt/rs97-toolchain/bin/mipsel-linux-gcc
CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
else
CC = gcc
CFLAGS = -Ofast -fdata-sections -ffunction-sections -fPIC -flto -Wall
endif

# You can use Ofast too but it can be more prone to bugs, careful.
CFLAGS +=  -I.

LIBS += -lSDL -lasound -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz
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