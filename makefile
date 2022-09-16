## set the directories for obj and source files
OBJDIR=obj
SRCDIR=source
## create the obj directory if necessary
MKDIR_P = mkdir -p
## add all files to one variable
OBJ=$(objects)
## specifies the compiler, g++ in this case
CPP=g++
## compiler flags -c
## -c for compilation
## -g for warnings
## -std=c++17 for CPP17 standard
## -Wall for all warnings
## -Werror handle all warnings as errors
CFLAGS=-c -g -std=c++17 -Wall -Werror
## linker flags for SDL2
LDFLAGS=-lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf

## target all for the framework
## test executes the builded framework
all: directory sdl_framework test

directory:
	$(MKDIR_P) $(OBJDIR)

## for variable for all file categories
objects = $(generalpurpose) $(states) $(textures)

## general purpose files
generalpurpose = $(OBJDIR)/Game.o $(OBJDIR)/InputManager.o $(OBJDIR)/Logfile.o $(OBJDIR)/main.o \
				$(OBJDIR)/Net.o $(OBJDIR)/SoundManager.o $(OBJDIR)/SystemTime.o $(OBJDIR)/Timer.o

## states files connected
states = $(OBJDIR)/GameState.o $(OBJDIR)/GameStateMachine.o $(OBJDIR)/GameOverState.o \
		$(OBJDIR)/MenuButton.o $(OBJDIR)/MenuState.o $(OBJDIR)/PauseState.o $(OBJDIR)/PlayState.o \
		$(OBJDIR)/SettingState.o

## texture files
textures = $(OBJDIR)/Texture.o $(OBJDIR)/TextureManager.o $(OBJDIR)/Vector2D.o

## target for framework
## will be executed for $(OBJ) what means all files added to this variable
sdl_framework: $(OBJ)
	## run gcc compiler for all files and link them with sdl to the framework
	$(CPP) $(OBJ) $(LDFLAGS) -o sdlframework

## here's where the magic happens
## $(OBJDIR) is the placeholder for the directory for object-files
## % is wildcard
## $< is the dependency (source file)
## $@ is the target (object file)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) $< -o $@

## executes the framework if build is successful
test: sdl_framework
	./sdlframework

## cleans the $(OBJDIR) specified directory
## and deletes the executable file 
clean:
	rm $(OBJDIR)/*.o sdlframework