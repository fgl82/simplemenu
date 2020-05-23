# Software Name
TARGET = simplemenu
#PLATFORM = BITTBOY
#PLATFORM = RG-300
#PLATFORM = RG-350
#PLATFORM = NPG
PLATFORM = PC

# Compiler
ifeq ($(PLATFORM), BITTBOY)
	CC = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	LINKER = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
	CFLAGS = -DTARGET_BITTBOY -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall
	LIBS += -lSDL -lasound -lSDL_image -lpng -ljpeg -lSDL_ttf -lfreetype -lz
else ifeq ($(PLATFORM), RG-300)
	CC = /opt/new-toolchain/usr/bin/mipsel-linux-gcc
	LINKER = /opt/new-toolchain/usr/bin/mipsel-linux-gcc
	CFLAGS = -DTARGET_RG300 -DUSE_GZIP -ofast -fdata-sections -ffunction-sections -fno-PIC -Wall -Wextra
	LIBS += -lSDL -lSDL_sound -lSDL_image -lSDL_ttf -lz 
else ifeq ($(PLATFORM), RG-350)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	LINKER = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	CFLAGS = -DTARGET_RG350 -DUSE_GZIP -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -std=gnu99
	LIBS += -lSDL -lSDL_sound -lSDL_image -lSDL_ttf -lshake -lpthread -lz -lbz2
else ifeq ($(PLATFORM), NPG)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	LINKER = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	CFLAGS = -DTARGET_NPG -DUSE_GZIP -Ofast -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -std=gnu99
	LIBS += -lSDL -lSDL_sound -lSDL_image -lSDL_ttf -lpthread -lz -lbz2	
else
	TARGET = simplemenu-x86
	CC = gcc-9
	LINKER   = gcc-9
	CFLAGS = -DTARGET_PC -DUSE_GZIP -Ofast -fdata-sections -ffunction-sections -fPIC -flto -Wall -Wextra -std=gnu99
	LIBS += -lSDL -lasound -lSDL_image -lSDL_ttf -lpthread -lz 	
endif

# You can use Ofast too but it can be more prone to bugs, careful.
CFLAGS +=  -I.
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

#DEBUG=YES
ifeq ($(DEBUG), YES)
	CFLAGS +=  -DDEBUG -g3
else
	LDFLAGS	+=  -s -lm
endif

SRCDIR   = src/logic
OBJDIR   = src/obj
BINDIR   = output
SOURCES  := $(wildcard $(SRCDIR)/*.c)

ifeq ($(PLATFORM), BITTBOY)
	SOURCES := $(filter-out src/logic/control_rg350.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/control_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg350.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg300.c, $(SOURCES))	
else ifeq ($(PLATFORM), RG-300)
	SOURCES := $(filter-out src/logic/control_rg350.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/control_bittboy.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg350.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_bittboy.c, $(SOURCES))		
else ifeq ($(PLATFORM), RG-350)  
	SOURCES := $(filter-out src/logic/control_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/control_bittboy.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_bittboy.c, $(SOURCES))
else ifeq ($(PLATFORM), NPG)  
	SOURCES := $(filter-out src/logic/control_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/control_bittboy.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_bittboy.c, $(SOURCES))	
else
	SOURCES := $(filter-out src/logic/control_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/control_bittboy.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_rg300.c, $(SOURCES))
	SOURCES := $(filter-out src/logic/system_logic_bittboy.c, $(SOURCES))	
endif 

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