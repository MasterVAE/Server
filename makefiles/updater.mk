SOURCES_UPDATER = code/updater/updater.cpp code/updater/updater_request.cpp code/server/server_manager.cpp
OBJECTS_UPDATER := $(addprefix $(OBJ_DIR)/, $(SOURCES_UPDATER:.cpp=.o))
TARGET_UPDATER = $(TARGET_DIR)/updater.out

LIBS = -lcurl

HEADERS_UPDATER = $(wildcard $(SOURCE_DIR)/code/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_UPDATER): $(OBJECTS_UPDATER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "LINKED UPDATER"

upd: $(TARGET_UPDATER)	
	@./$(TARGET_UPDATER) asdkasjd &

upd_b: $(TARGET_UPDATER)

$(OBJECTS_UPDATER): $(HEADERS_UPDATER)