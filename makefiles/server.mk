SOURCES_SERVER = code/main_server/server_main.cpp code/server/server_manager.cpp code/main_server/server_request.cpp
OBJECTS_SERVER := $(addprefix $(OBJ_DIR)/, $(SOURCES_SERVER:.cpp=.o))
TARGET_SERVER = $(TARGET_DIR)/server.out

LIBS = -lcurl

HEADERS_SERVER = $(wildcard $(SOURCE_DIR)/code/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_SERVER): $(OBJECTS_SERVER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "LINKED SERVER"

srv: $(TARGET_SERVER)	
	@./$(TARGET_SERVER) adjasnhf &

srv_b: $(TARGET_SERVER)

$(OBJECTS_SERVER): $(HEADERS_SERVER)