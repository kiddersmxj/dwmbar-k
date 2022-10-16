CC = g++

BUILD_DIR ?= build
SRC_DIRS ?= src
MOD_DIRS ?= modules

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
MODS := $(shell find $(MOD_DIRS) -name *.cpp -or -name *.c -or -name *.s)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -Wall

TARGET_EXEC ?= dwmbar-k
TARGET_MODS ?= $(addprefix bin/,$(subst .cpp,,$(MODS)))

#test: all
#	@echo "$(SRCS)"
#	@echo "$(MODS)"
#	@echo "$(TARGET_MODS)"

all: bin/$(TARGET_EXEC) $(TARGET_MODS)

bin/$(TARGET_EXEC): $(OBJS)
	@echo "Linking..."
	@echo " $(CC) $(OBJS) -o $@ $(LDFLAGS)"; $(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	@echo " $(CC) $(CPPFLAGS) -c $< -o $@"; $(CC) $(CPPFLAGS) -c $< -o $@

bin/modules/%-k: $(MODS)
	$(MKDIR_P) $(dir $@)
	@echo " $(CC) $(CPPFLAGS) $< -o $@ $(LDFLAGS)"; $(CC) $(CPPFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

