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

#CFLAGS = -g -Iinclude 
CFLAGS = -g -Iclient/include -Ishared/include -Iinclude
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net



SRC = src
EXE = exe
APP = client/client.exe
#APP = game

# find all .c files (Windows safe)
#C_FILES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
C_FILES = $(wildcard client/src/*.c) $(wildcard client/src/Controll/*.c) $(wildcard client/src/Modell/*.c) $(wildcard client/src/View/*.c) $(wildcard src/*.c) $(wildcard src/Controll/*.c) $(wildcard src/Modell/*.c) $(wildcard src/View/*.c)


# convert to object files
#O_FILES = $(C_FILES:$(SRC)/%.c=$(EXE)/%.o)
O_FILES = $(C_FILES:%.c=$(EXE)/%.o)

main: $(O_FILES)
	$(CC) $(O_FILES) -o $(APP) $(LDFLAGS)

# compile rule (IMPORTANT FIX)
#$(EXE)/%.o: $(SRC)/%.c
$(EXE)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
#	rm -rf $(EXE) $(APP)
	rm -rf $(EXE) client/client.exe game game.exe
