CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP

SRC_DIR := src
BUILD_DIR := build

COMMON_SRCS := $(wildcard $(SRC_DIR)/common/*.cpp)
COMMON_OBJS := $(patsubst $(SRC_DIR)/common/%.cpp,$(BUILD_DIR)/common_%.o,$(COMMON_SRCS))

SERVER_SRCS := $(wildcard $(SRC_DIR)/server/*.cpp)
SERVER_OBJS := $(patsubst $(SRC_DIR)/server/%.cpp,$(BUILD_DIR)/server_%.o,$(SERVER_SRCS))

CLIENT_SRCS := $(wildcard $(SRC_DIR)/client/*.cpp)
CLIENT_OBJS := $(patsubst $(SRC_DIR)/client/%.cpp,$(BUILD_DIR)/client_%.o,$(CLIENT_SRCS))

SERVER_TARGET := server
CLIENT_TARGET := client

DEPS := $(COMMON_OBJS:.o=.d) $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(COMMON_OBJS) $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(CLIENT_TARGET): $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/common_%.o: $(SRC_DIR)/common/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/server_%.o: $(SRC_DIR)/server/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/client_%.o: $(SRC_DIR)/client/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR) $(SERVER_TARGET) $(CLIENT_TARGET)

.PHONY: all clean
