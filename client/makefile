# CC = gcc

# CFLAGS = -g -Iinclude -I..\..\shared\Include
# LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net


# SHARED = shared 
# SRC = src
# EXE = exe
# APP = client

# # find all .c files (Windows safe)
# C_FILES = $(wildcard $(SRC)/*.c) $(wildcard $(SHARED)/client/$(SRC)/*/*.c)

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

CFLAGS = -g -IInclude -I../shared/Include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net

SHARED = ../shared/src
SRC = src
EXE = exe
APP = client



CLIENT_C = $(wildcard $(SRC)/*.c)
SHARED_C = $(wildcard $(SHARED)/**/*.c)


CLIENT_O = $(CLIENT_C:$(SRC)/%.c=$(EXE)/%.o)
SHARED_O = $(SHARED_C:$(SHARED)/%.c=$(EXE)/shared/%.o)
# CLIENT_O = $(patsubst $(SRC)/%.c,$(EXE)/%.o,$(CLIENT_C))
# SHARED_O = $(patsubst $(SHARED)/%.c,$(EXE)/shared/%.o,$(SHARED_C))

O_FILES = $(CLIENT_O) $(SHARED_O)



main: $(O_FILES)
	$(CC) $(O_FILES) -o $(APP) $(LDFLAGS)



$(EXE)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@



$(EXE)/shared/%.o: $(SHARED)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm -rf $(EXE) $(APP)