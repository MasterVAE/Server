SOURCES_PROCESSOR = code/processor/processor.cpp code/processor/processor_manager.cpp code/processor/processor_functions.cpp code/processor/stack.cpp code/lib.cpp
OBJECTS_PROCESSOR := $(addprefix $(OBJ_DIR)/, $(SOURCES_PROCESSOR:.cpp=.o))
TARGET_PROCESSOR = $(TARGET_DIR)/spu.out

HEADERS_PROCESSOR = $(wildcard $(SOURCE_DIR)/code/processor/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_PROCESSOR): $(OBJECTS_PROCESSOR) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED PROCESSOR"

spu: $(TARGET_PROCESSOR)
	@./$(TARGET_PROCESSOR)

spu_b: $(TARGET_PROCESSOR)

$(OBJECTS_PROCESSOR): $(HEADERS_PROCESSOR)