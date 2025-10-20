SOURCES_SERVER = code/server/server_main.cpp code/server/server_manager.cpp code/server/server_request.cpp
OBJECTS_SERVER := $(addprefix $(OBJ_DIR)/, $(SOURCES_SERVER:.cpp=.o))
TARGET_SERVER = $(TARGET_DIR)/server.out

HEADERS_SERVER = $(wildcard $(SOURCE_DIR)/code/server/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_SERVER): $(OBJECTS_SERVER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED SERVER"

srv: $(TARGET_SERVER)	
	@./$(TARGET_SERVER)

srv_b: $(TARGET_SERVER)

$(OBJECTS_SERVER): $(HEADERS_SERVER)