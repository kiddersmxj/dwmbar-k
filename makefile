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

all: $(TARGET_MODS) bin/$(TARGET_EXEC)

bin/$(TARGET_EXEC): $(OBJS)
	@echo "Linking..."
	@echo "$(CC) $^ -o $@ $(LDFLAGS)"; $(CC) $^ -o $@ $(LDFLAGS)

bin/modules/%-k: $(MODS)
	$(MKDIR_P) $(dir $@)
	@echo "$(CC) ${subst bin/,,$@.cpp} -o $@ -lasound $(LDFLAGS)"; $(CC) ${subst bin/,,$@.cpp} -o $@ -lasound $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Making Objects..."
	$(MKDIR_P) $(dir $@)
	@echo "$(CC) $(CPPFLAGS) -c $< -o $@"; $(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r bin/$(MOD_DIRS)

-include $(DEPS)

MKDIR_P ?= mkdir -p

