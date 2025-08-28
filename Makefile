CC := g++
LDFLAGS := -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET := super_mario

all: assets $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/defines.hpp
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@

assets:
	tar -xzf assets.tar.gz

clean:
	-rm -rf $(OBJ_DIR) $(TARGET)
