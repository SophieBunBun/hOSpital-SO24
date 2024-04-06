#-----------------------------------------------------------------------------------
# directories; we don't HAVE to have this, but it's standard procedure
OBJ_dir = obj
INCLUDE_dir = include
SRC_dir = src
BIN_dir = bin

#-----------------------------------------------------------------------------------
# I want to make this dynamic instead of having to rely on actual files....
OBJECTOS = main.o doctor.o memory.o

main.o =
doctor.o = 
memory.o = 
#-----------------------------------------------------------------------------------
# Will change the name of the output file!
FILENAME = out
#-----------------------------------------------------------------------------------
CC = gcc # Compiler; doesn't HAVE to be like this, but it's standard procedure
CFLAGS = -Wall	# flags; always good to know what we want for the compiler, Wall shows all warnings
#-----------------------------------------------------------------------------------
out: $(OBJECTOS)
	 $(CC)$(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/$(FILENAME)

%.o: $(SRC_dir)/%.c $($@)
	 $(CC) $(CFLAGS) -c $^ -o $(OBJ_dir)/$@
#-----------------------------------------------------------------------------------
# Not actually working, please fix
clean:
	rm -rf *.o $(FILENAME)
	rm -rf $(FILENAME)
#-----------------------------------------------------------------------------------