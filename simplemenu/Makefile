# Software Name
TARGET = simplemenu.elf
PLATFORM = BITTBOY
#PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
	CC = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	LINKER = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
else ifeq ($(PROFILE), RS97)
	CC = /opt/rs97-toolchain/bin/mipsel-linux-gcc
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
else
	CC = gcc
	LINKER   = gcc
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -fPIC -flto -Wall
endif

# You can use Ofast too but it can be more prone to bugs, careful.
CFLAGS +=  -I.
LIBS += -lSDL -lasound -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz
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