# CC = gcc

# CFLAGS = -g -Iinclude 
# LDFLAGS = -lmingw32 -lSDL2main -lSDL2

# SRC = src
# EXE = exe
# APP = game

# # find all .c files (Windows safe)
# C_FILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)

# # convert to object files
# O_FILES = $(C_FILES:$(SRC)/%.c=$(EXE)/%.o)

# main: $(O_FILES)
# 	$(CC) $(O_FILES) -o $(APP) $(LDFLAGS)

# # compile rule (IMPORTANT FIX)
# $(EXE)/%.o: $(SRC)/%.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -rf $(EXE) $(APP)



CC = gcc

CFLAGS = -g -Iinclude 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net



SRC = src
EXE = exe
APP = game

# find all .c files (Windows safe)
C_FILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)

# convert to object files
O_FILES = $(C_FILES:$(SRC)/%.c=$(EXE)/%.o)

main: $(O_FILES)
	$(CC) $(O_FILES) -o $(APP) $(LDFLAGS)

# compile rule (IMPORTANT FIX)
$(EXE)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE) $(APP)