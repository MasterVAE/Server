SOURCES_UPDATER = code/updater/updater.cpp
OBJECTS_UPDATER := $(addprefix $(OBJ_DIR)/, $(SOURCES_UPDATER:.cpp=.o))
TARGET_UPDATER = $(TARGET_DIR)/updater.out

HEADERS_UPDATER = $(wildcard $(SOURCE_DIR)/code/updater/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_UPDATER): $(OBJECTS_UPDATER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED UPDATER"

upd: $(TARGET_UPDATER)	
	@./$(TARGET_UPDATER)

upd_b: $(TARGET_UPDATER)

$(OBJECTS_UPDATER): $(HEADERS_UPDATER)