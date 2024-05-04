#-----------------------------------------------------------------------------------
# Grupo SO-028
# Sofia Santos  fc59804
# Joao Dias     fc59854
# Nuno Graxinha fc59855
#-----------------------------------------------------------------------------------
# directories; we don't HAVE to have this, but it's standard procedure
OBJ_dir = obj
INCLUDE_dir = include
SRC_dir = src
BIN_dir = bin

#-----------------------------------------------------------------------------------
# I want to make this dynamic instead of having to rely on actual files....
OBJECTOS = configuration.o main.o hospsignal.o hosptime.o log.o synchronization.o doctor.o memory.o patient.o receptionist.o process.o

configuration.o =
main.o =
hospsignal.o =
hosptime.o =
log.o =
synchronization.o =
doctor.o = 
memory.o = 
patient.o = 
process.o =

#-----------------------------------------------------------------------------------
# Will change the name of the output file!
FILENAME = hOSpital
#-----------------------------------------------------------------------------------
CC = gcc # Compiler; doesn't HAVE to be like this, but it's standard procedure
CFLAGS = -w	# flags; always good to know what we want for the compiler, Wall shows all warnings
#-----------------------------------------------------------------------------------
out: $(OBJECTOS)
	@$(CC)$(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/$(FILENAME)
	@echo "Compilation successful. Have a good day!"

%.o: $(SRC_dir)/%.c $($@)
	@$(CC) $(CFLAGS) -c $^ -o $(OBJ_dir)/$@
#-----------------------------------------------------------------------------------
# Not actually working, please fix
clean:
	@rm -rf *.o $(FILENAME)
	@rm -rf $(FILENAME)
	@echo "Deleted successfully!"
#-----------------------------------------------------------------------------------