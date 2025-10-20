SOURCES_ASSEMBLER = code/assembler/assembler.cpp code/assembler/assembler_manager.cpp code/assembler/assembler_func.cpp code/lib.cpp
OBJECTS_ASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_ASSEMBLER:.cpp=.o))
TARGET_ASSEMBLER = $(TARGET_DIR)/asm.out

HEADERS_ASSEMBLER = $(wildcard $(SOURCE_DIR)/code/assembler/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_ASSEMBLER): $(OBJECTS_ASSEMBLER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED ASSEMBLER"

asm: $(TARGET_ASSEMBLER)	
	@./$(TARGET_ASSEMBLER)

asm_b: $(TARGET_ASSEMBLER)

$(OBJECTS_ASSEMBLER): $(HEADERS_ASSEMBLER)