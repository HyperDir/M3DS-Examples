.SUFFIXES:

C_VERSION		    := 23
CXX_VERSION		    := 26

APP_TITLE           := Serialisation
APP_DESCRIPTION     := An M3DS Serialisation Example
APP_AUTHOR          := HyperDir

APP_VER_MAJOR       := 1
APP_VER_MINOR       := 0
APP_VER_PATCH       := 0

#---------------------------------------------------------------------------------
# Environment Setup
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO")
endif

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# 3DS IP Address/Hostname
IP3DS               := Nintendo

BUILD_DIR           := build
SOURCES_DIR         := source
INCLUDE_DIR         := include
EMBED_DIR           := $(BUILD_DIR)

OUTPUT_DIR          := output
ROMFS_DIR           := romfs
GFX_DIR             := gfx

ICON                := icon.png

#---------------------------------------------------------------------------------
# Flags
#---------------------------------------------------------------------------------
ARCH_FLAGS          := -march=armv6k -mtune=mpcore -mfloat-abi=hard

WARNINGS            := -Wall -Werror -Wextra -Wconversion -Wpedantic -Wno-psabi

MAKEFLAGS           := -j14 --silent
COMMON_FLAGS        := $(WARNINGS) -O2 -mword-relocations -fomit-frame-pointer -ffast-math $(ARCH_FLAGS) -D__3DS__
C_FLAGS             := $(COMMON_FLAGS) -std=c$(C_VERSION)
CXX_FLAGS           := $(COMMON_FLAGS) -std=c++$(CXX_VERSION) -fno-rtti -fno-exceptions

LD_FLAGS            := -specs=3dsx.specs $(ARCH_FLAGS) -z noexecstack

ifeq ($(OS),Windows_NT)
    DEVKITPRO       := $(shell echo "$(DEVKITPRO)" | sed -e 's|^/opt|C:|')
    DEVKITARM       := $(shell echo "$(DEVKITARM)" | sed -e 's|^/opt|C:|')
endif

PORTLIBS            := $(DEVKITPRO)/portlibs/3ds
CTRULIB             := $(DEVKITPRO)/libctru

LIBS                := -lm3ds -lcitro3d -lctru -lm
LIB_DIRS            := $(PORTLIBS) $(CTRULIB)

#---------------------------------------------------------------------------------
# Build Variable Setup
#---------------------------------------------------------------------------------
recurse = $(shell find $2 -type $1 -name '$3' 2> /dev/null)

C_FILES             := $(foreach dir,$(SOURCES_DIR),$(call recurse,f,$(dir),*.c))
CXX_FILES           := $(foreach dir,$(SOURCES_DIR),$(call recurse,f,$(dir),*.cpp))
PICA_FILES          := $(foreach dir,$(SOURCES_DIR), $(call recurse,f,$(dir),*.pica))

GFX_FILES           := $(foreach dir,$(GFX_DIR),$(call recurse,f,$(dir),*.*))

GFX_EXTENSIONS      := $(addprefix %.,PNG png JPG jpg JPEG jpeg BMP bmp)

T3X_FILES           := $(addsuffix .t3x, $(addprefix $(ROMFS_DIR)/, $(filter $(GFX_EXTENSIONS),$(GFX_FILES))))
ROMFS_FILES         := $(shell find $(ROMFS_DIR) -type f) $(T3X_FILES)

O_FILES             := $(patsubst $(SOURCES_DIR)/%,$(BUILD_DIR)/%,$(addsuffix .o, $(basename $(C_FILES) $(CXX_FILES))))
BIN_FILES           := $(patsubst $(SOURCES_DIR)/%,$(BUILD_DIR)/%,$(addsuffix .bin, $(basename $(basename $(PICA_FILES)))))

INCLUDE             := $(foreach dir,$(INCLUDE_DIR),-I./$(dir)) $(foreach dir,$(LIB_DIRS),-isystem $(dir)/include)
LIB_PATHS           := $(foreach dir,$(LIB_DIRS),-L$(dir)/lib)

EMBED               := $(foreach dir,$(EMBED_DIR),--embed-dir=./$(dir))

CC                  := $(DEVKITARM)/bin/arm-none-eabi-gcc
CXX                 := $(DEVKITARM)/bin/arm-none-eabi-g++
LD                  := $(CXX)

VERSION_FLAGS       := -DAPP_VERSION_MAJOR=$(APP_VER_MAJOR) -DAPP_VERSION_MINOR=$(APP_VER_MINOR) -DAPP_VERSION_PATCH=$(APP_VER_PATCH)

SPACE               := $() $()
OUTPUT_NAME         := $(subst $(SPACE),,$(APP_TITLE))
OUTPUT_FILE         := $(OUTPUT_DIR)/$(OUTPUT_NAME)

3DSLINK             := $(DEVKITPRO)/tools/bin/3dslink
3DSXTOOL            := $(DEVKITPRO)/tools/bin/3dsxtool
SMDHTOOL            := $(DEVKITPRO)/tools/bin/smdhtool
TEX3DS              := $(DEVKITPRO)/tools/bin/tex3ds
PICASSO             := $(DEVKITPRO)/tools/bin/picasso

ifeq ($(OS),Windows_NT)
    RUN := start
    else
        UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        RUN := open
    else
        RUN := xdg-open
    endif
endif


.PHONY: 3dsx all clean elf hblauncher run smdh
.PRECIOUS: $(T3X_FILES)

all: 3dsx

3dsx: $(OUTPUT_FILE).3dsx
smdh: $(OUTPUT_FILE).smdh
elf: $(OUTPUT_FILE).elf

hblauncher : $(OUTPUT_FILE).3dsx
	@$(3DSLINK) -a $(IP3DS) $(OUTPUT_FILE).3dsx

run: $(OUTPUT_FILE).3dsx
	$(RUN) $(OUTPUT_FILE).3dsx

clean:
	@echo Cleaning $(APP_TITLE)...
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR) $(ROMFS_DIR)/gfx

$(ROMFS_DIR)/%.t3x: % 
	$(info Processing $<)
	@mkdir -p $(dir $@)
	@$(TEX3DS) --atlas -f rgba8888 -z -i $< -o $@ > /dev/null 2>&1

$(BUILD_DIR)/%.bin: $(SOURCES_DIR)/%.v.pica
	$(info Compiling $^)
	@mkdir -p $(dir $@)
	@$(PICASSO) -o $@ $^

$(BUILD_DIR)/%.bin: $(SOURCES_DIR)/%.v.pica $(SOURCES_DIR)/%.g.pica
	$(info Compiling $^)
	@mkdir -p $(dir $@)
	@$(PICASSO) -o $@ $^

$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.c $(BIN_FILES)
	$(info Compiling $<)
	@mkdir -p $(dir $@)
	@$(CC) -MMD -MP -MF $(BUILD_DIR)/$*.d $(C_FLAGS) $(VERSION_FLAGS) $(INCLUDE) $(EMBED) -c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(BIN_FILES)
	$(info Compiling $<)
	@mkdir -p $(dir $@)
	@$(CXX) -MMD -MP -MF $(BUILD_DIR)/$*.d $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) $(EMBED) -c $< -o $@

$(OUTPUT_DIR)/%.elf: $(O_FILES)
	$(info Linking $(notdir $@))
	@mkdir -p $(dir $@)
	@$(LD) $(LD_FLAGS) $(O_FILES) $(LIB_PATHS) $(LIBS) -o $@

$(OUTPUT_DIR)/%.smdh: $(APP_INFO_FILE) 
	$(info Creating $(notdir $@))
	@mkdir -p $(dir $@)
	@$(SMDHTOOL) --create "$(APP_TITLE)" "$(APP_DESCRIPTION)" "$(APP_AUTHOR)" "$(ICON)" $@

$(OUTPUT_DIR)/%.3dsx: $(OUTPUT_DIR)/%.elf $(OUTPUT_DIR)/%.smdh $(ROMFS_FILES)
	@mkdir -p $(dir $@)
	@$(3DSXTOOL) $< $@ --smdh=$(word 2,$^) --romfs=$(ROMFS_DIR)
	$(info Built $(notdir $@))

-include $(O_FILES:.o=.d)
