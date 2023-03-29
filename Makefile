include SOURCES

#compiling & linking

OBJ_TMP = $(subst .cpp,.o,       $(SRC))
OBJ     = $(subst src,src/build, $(OBJ_TMP)) # src/main.cpp -> src/main.o -> src/build/main.o

#compiling
                                             
link : $(OBJ) 
	$(CC) $(IFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME) 

src/build/%.o : src/%.cpp	
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@	

clear :
	rm   -f ./src/build/*.o 