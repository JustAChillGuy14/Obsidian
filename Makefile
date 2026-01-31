CC := g++
INCLUDE_DIR := headers
CFLAGS := -Wall -Wextra -Wpedantic -I$(INCLUDE_DIR) -MMD -MP

TARGET := main
OBJ_DIR := objects
SRC_DIR := src
SRC_DIRS := $(wildcard $(SRC_DIR)/*/)

SRC_FILES := $(wildcard $(foreach d,$(SRC_DIRS),$(d)*)) $(wildcard $(SRC_DIR)/*.cpp)
OBJECT_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(OBJECT_FILES:%.o=%.d)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean
