# Specify the source directory and get all C source files
SRC_DIR = source
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET = mysh

# Specify the object directory and generate object files
OBJ_DIR = bin
OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

# Default rule to build the program
all: $(TARGET)

# Rule to build the program
$(TARGET): $(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS)

# Rule to compile each source file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

# Clean rule to remove object files and executable
clean:
	rm -f $(OBJS) $(TARGET)