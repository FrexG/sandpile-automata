CC := clang
CFLAGS := -Wall -Wextra

LIBS := `pkg-config --libs --cflags raylib`


SRC  := sandpile.c
OBJ  := sandpile.o
MAIN := sandpile

$(MAIN):$(OBJ)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ) $(LIBS)

$(OBJ):$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $(SRC) $(LIBS)

clean:
	rm *.o && rm sandpile 
