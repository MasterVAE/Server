SOURCES_CLIENT = code/client/client_main.cpp
OBJECTS_CLIENT := $(addprefix $(OBJ_DIR)/, $(SOURCES_CLIENT:.cpp=.o))
TARGET_CLIENT = $(TARGET_DIR)/client.out

LIBS = -lcurl

HEADERS_CLIENT = $(wildcard $(SOURCE_DIR)/code/client/*.h) $(wildcard $(SOURCE_DIR)/*.h)

$(TARGET_CLIENT): $(OBJECTS_CLIENT) | $(TARGET_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@echo "LINKED CLIENT"

clt: $(TARGET_CLIENT)	
	@./$(TARGET_CLIENT)

clt_b: $(TARGET_CLIENT)

$(OBJECTS_CLIENT): $(HEADERS_CLIENT)