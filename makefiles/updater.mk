SOURCES_UPDATER_SERVER = code/updater/updater.cpp code/updater/updater_request.cpp code/server/server_manager.cpp
SOURCES_UPDATER_CLIENT = code/updater_client/updater_client.cpp code/server/server_manager.cpp
OBJECTS_UPDATER_SERVER := $(addprefix $(OBJ_DIR)/, $(SOURCES_UPDATER_SERVER:.cpp=.o))
OBJECTS_UPDATER_CLIENT := $(addprefix $(OBJ_DIR)/, $(SOURCES_UPDATER_CLIENT:.cpp=.o))
TARGET_UPDATER_SERVER = $(TARGET_DIR)/updater_server.out
TARGET_UPDATER_CLIENT = $(TARGET_DIR)/updater_client.out

LIBS = -lcurl

HEADERS_UPDATER = $(wildcard $(SOURCE_DIR)/code/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_UPDATER_SERVER): $(OBJECTS_UPDATER_SERVER) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "LINKED UPDATER SERVER"

$(TARGET_UPDATER_CLIENT): $(OBJECTS_UPDATER_CLIENT) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "LINKED UPDATER CLIENT"

upd_srv: $(TARGET_UPDATER_SERVER)	
	@./$(TARGET_UPDATER_SERVER) asdkasjd &

upd_srv_b: $(TARGET_UPDATER_SERVER)

upd_clt: $(TARGET_UPDATER_CLIENT)	
	@./$(TARGET_UPDATER_CLIENT)

upd_clt_b: $(TARGET_UPDATER_CLIENT)

kill: $(TARGET_UPDATER_CLIENT)	
	@./$(TARGET_UPDATER_CLIENT) kill

$(OBJECTS_UPDATER_SERVER): $(HEADERS_UPDATER)
$(OBJECTS_UPDATER_CLIENT): $(HEADERS_UPDATER)