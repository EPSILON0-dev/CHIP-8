APP        = chip8

CC         = g++
LINKER     = g++ -o
RM         = rm -f
MKDIR      = mkdir -p
RMDIR      = rmdir

SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin
LOGDIR	   = log
TESTDIR    = test

CFLAGS    :=-O2 -Wall -Wextra
LFLAGS    :=-lsfml-graphics -lsfml-system -lsfml-window
OBJ       := $(OBJDIR)/interpreter.o $(OBJDIR)/main.o $(OBJDIR)/screen.o $(OBJDIR)/util.o
DIRS      := $(OBJDIR) $(BINDIR) $(LOGDIR)

.PHONEY: all
all: directories chip8

chip8: directories $(BINDIR)/$(APP)

.PHONEY: directories
directories: $(DIRS)

$(LOGDIR):
	@$(MKDIR) $(LOGDIR)

$(OBJDIR):
	@$(MKDIR) $(OBJDIR)

$(BINDIR):
	@$(MKDIR) $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/$(APP): $(OBJ)
	$(LINKER) $@ $^ $(CFLAGS) $(LFLAGS)

.PHONEY: clean
clean:
	$(RM) $(OBJDIR)/*.o

.PHONEY: realclean
realclean: clean
	@$(RM) $(BINDIR)/* $(TESTDIR)/*.s19 $(LOGDIR)/*
	@$(RMDIR) $(OBJDIR)
	@$(RMDIR) $(BINDIR)
	@$(RMDIR) $(LOGDIR)
