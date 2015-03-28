PKGCONFIG=pkg-config
OS=GNU/Linux
CXXFLAGS=-I../src -I incl -std=c++14 -O3 -pipe -g -pg -Wall -Wno-literal-suffix -Wno-unused-variable -pedantic-errors `$(PKGCONFIG) --cflags lua`
WINFLAGS=-m32 -Wl,-subsystem,windows -static-libgcc -static-libstdc++ -DWINDOWS
LINUXFLAGS=
CPPLIBS= `$(PKGCONFIG) --static --libs lua sdl` -L. -Wl,-rpath -Wl,./lib
WINLIBS=libpdcurses/*.o -lSDL -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic 
# SWAP the following two lines(Uncomment one, comment the other) to switch
# between console curses and pdcurses(which can support SDL/X11 output)
LINUXLIBS=-Bstatic -lcurses -lpanel -Bdynamic
#LINUXLIBS=-Bstatic -lpdcurses -Bdynamic
SRCPATH=src/
OBJPATH=obj/
SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst $(SRCPATH)%.cpp,$(OBJPATH)%.o,$(wildcard $(SRCPATH)*.cpp))
DEPS:=$(patsubst $(SRCPATH)%.cpp,$(OBJPATH)%.depend,$(wildcard $(SRCPATH)*.cpp))
APP=to-the-west
ifeq ($(OS), GNU/Linux)
OSFLAGS=$(LINUXFLAGS)
OSLIBS=$(LINUXLIBS)
else ifeq ($(OS), Windows)
OSFLAGS=$(WINFLAGS)
OSLIBS=$(WINLIBS)
CXX=i686-w64-mingw32-g++
APP := $(APP).exe
else
$(error Unknown OS selected for build output)
endif

$(OBJPATH)%.depend: $(SRCPATH)%.cpp
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $(OSFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)%.o: $(SRCPATH)%.cpp
	@echo -e "Building \e[1;35m$<\e[0m..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(OSFLAGS)

all: $(DEPS) $(SRCS) $(OBJS)
	@echo -e "\e[0;33mBuilding main application for \e[1;35m$(OS)\e[0;33m systems...\e[0m"
	@$(CXX) -o $(APP) $(OBJS) $(CXXFLAGS) $(OSFLAGS) $(CPPLIBS) $(OSLIBS)
	@echo -e "\e[0;32mBuild completed.\e[0m"

release:
	@echo -e "\e[0;31mCopying relevant files...\e[0m"
	@mkdir -p bin
	@cp -r data bin
	@cp $(APP) bin
	@echo -e "\e[0;32mSuccess! You can find the new files in the bin folder.\e[0m"

-include $(patsubst $(SRCPATH)%.cpp,$(OBJPATH)%.depend,$(wildcard $(SRCPATH)*.cpp))
$(shell mkdir -p obj)

.PHONY: clean
clean:
	@echo -e "\e[0;31mCleaning up...\e[0m"
	$(RM) $(OBJS)
	$(RM) $(OBJPATH)*.depend
	$(RM) $(OBJPATH)*.depend.*
	$(RM) $(APP)
	@echo -e "\e[0;32mCleanup completed.\e[0m"

