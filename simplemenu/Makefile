# Software Name
TARGET = simplemenu.dge
#PLATFORM = BITTBOY
#PLATFORM = RG-300
#PLATFORM = RG-350
PLATFORM = UNIV
#PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
	CC = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	LINKER = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
	LIBS += -lSDL -lasound -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz
else ifeq ($(PLATFORM), RG-300)
	CC = /opt/rs97-toolchain/bin/mipsel-linux-gcc
	LINKER = /opt/rs97-toolchain/bin/mipsel-linux-gcc
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
	LIBS += -lSDL -lSDL_sound -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz -lbz2
else ifeq ($(PLATFORM), RG-350)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	LINKER = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	CFLAGS = -DUSE_GZIP -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -std=gnu99
	LIBS += -lSDL -lSDL_sound -lSDL_image -lpng -ljpeg -lSDL_ttf -lshake -lpthread -lfreetype -lz -lbz2
else ifeq ($(PLATFORM), RFW2)
	CC = /opt/rfw2-toolchain/usr/bin/mipsel-linux-gcc
	LINKER = /opt/rfw2-toolchain/usr/bin/mipsel-linux-gcc
	CFLAGS = -DUSE_GZIP -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -std=gnu99
	LIBS += -lSDL -lSDL_sound -lSDL_image -lpng -ljpeg -lSDL_ttf -lpthread -lfreetype -lz -lbz2		
else
	CC = gcc
	LINKER   = gcc
	CFLAGS = -DUSE_GZIP -DTARGET_PC -Ofast -fdata-sections -ffunction-sections -fPIC -flto -Wall
	LIBS += -lSDL -lasound -lSDL_image -lpng -ljpeg -lSDL_ttf -lpthread -lfreetype -lz	
endif

# You can use Ofast too but it can be more prone to bugs, careful.
CFLAGS +=  -I.
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

ifeq ($(DEBUG), YES)
	CFLAGS +=  -DDEBUG -g3
else
	LDFLAGS	+=  -s -lm
endif

SRCDIR   = src/logic
OBJDIR   = src/obj
BINDIR   = output
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f
	
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"