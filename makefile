CC = g++

BUILD_DIR ?= build
SRC_DIRS ?= src
MOD_DIRS ?= modules
LIB_DIRS ?= lib

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
MODS := $(shell find $(MOD_DIRS) -name *.cpp -or -name *.c -or -name *.s)
MODOBJS := $(MODS:%=$(BUILD_DIR)$(MOD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LDFLAGS := $(shell find $(LIB_DIRS) -name *.o)

CPPFLAGS ?= -Wall

TARGET_EXEC ?= dwmbar-k
TARGET_MODS ?= $(addprefix bin/,$(subst .cpp,,$(MODS)))

test: all
	@echo "$(SRCS)"
	@echo "$(MODS)"
	@echo "$(TARGET_MODS)"
	@echo "$(INC_DIRS)"

all: $(TARGET_MODS) bin/$(TARGET_EXEC)

bin/$(TARGET_EXEC): $(OBJS)
	@echo "Linking..."
	@echo " $(CC) $^ -o $@ $(LDFLAGS)"; $(CC) $^ -o $@ $(LDFLAGS)

bin/modules/%-k: $(MODS)
	$(MKDIR_P) $(dir $@)
	@echo " $(CC) ${subst bin/,,$@.cpp} -o $@ $(LDFLAGS)"; $(CC) ${subst bin/,,$@.cpp} -o $@ $(LDFLAGS)
	#@echo " $(CC) $(LDFLAGS) $< -o $@"; $(CC) $(LDFLAGS) $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Making Objects..."
	$(MKDIR_P) $(dir $@)
	@echo " $(CC) $(CPPFLAGS) -c $< -o $@"; $(CC) $(CPPFLAGS) -c $< -o $@

#$(BUILD_DIR)/$(MOD_DIR)%.cpp.o: $(MODS)
#	@echo " $@"
#	@echo " $(CC) $(CPPFLAGS) -c $< -o $@"; $(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

