CC = gcc
CFLAGS = -g -Iinclude
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows

SRC = src
EXE = exe
INC = include
App = game
# all .c files
C_files = $(wildcard $(SRC)/*.c)

# convert src/main.c → exe/main.o
O_files = $(patsubst $(SRC)/%.c,$(EXE)/%.o,$(C_files))

# final program
main: $(O_files)
	$(CC) $(O_files) -o $(App) $(LDFLAGS)

# compile rule
$(EXE)/%.o: $(SRC)/%.c
	@mkdir -p $(EXE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE) $(App)