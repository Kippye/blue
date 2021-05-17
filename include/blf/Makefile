# Compiler variables.
GXX=g++
WINDOWSGXX=g++
CXXFLAGS=-no-pie -ldl -g
WINDOWSCXXFLAGS=-no-pie -lgdi32 -static -static-libgcc -static-libstdc++ -O3 -s -Os
INCLUDES=./include
LIBS=./libs
RM=rm -rf

# Location of the output executable.
OUT=./bin/BLF

# SRC folder is where the .cpp and .c files are located, OBJFOLDER is where the compiled .o files are.
OBJFOLDER=./build/objects
SRCFOLDER=./src
CFGFOLDER=./build/cfg

# Recursive wildcard function.
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILDFILE = $(CFGFOLDER)/build.cfg
BUILDNUMBER = $(shell cat $(BUILDFILE))

ADDITIONALOBJS=
MINGWADDITIONAL=
SUBDIRS = $(call rwildcard,$(SRCFOLDER),*.cpp)
SRCDIRS = $(shell find $(SRCFOLDER) -type d)
OBJDIRS = $(OBJFOLDER) $(subst $(SRCFOLDER),$(OBJFOLDER),$(SRCDIRS))
OBJECTNAMES := $(subst $(SRCFOLDER),$(OBJFOLDER),$(SUBDIRS:%.cpp=%.o))
MINGWOBJECTS := $(subst $(SRCFOLDER),$(OBJFOLDER),$(SUBDIRS:%.cpp=%.mingw64.o))

# Colours
yellow=\033[1;33m
no_color=\033[0m

all: $(CFGFOLDER) $(BUILDFILE) $(OBJDIRS) $(ADDITIONALOBJS) $(OBJECTNAMES) $(OUT)

$(BUILDFILE):
	@echo -e "$(no_color)Creating a new $(yellow)Build Configuration $(no_color)file..."
	@echo "0" > $(BUILDFILE)

$(OBJFOLDER):
	@echo -e "$(no_color)Making the $(yellow)'$(OBJFOLDER)' $(no_color)directory... $(no_color)"
	@mkdir -p $@

$(CFGFOLDER):
	@echo -e "$(no_color)Making the $(yellow)'$(CFGFOLDER)' $(no_color)directory... $(no_color)"
	@mkdir -p $@

$(OBJFOLDER)/%: $(SRCFOLDER)/%
	@echo -e "$(no_color)Creating $(yellow)$@ $(no_color)from the source tree... $(no_color)"
	@mkdir -p $@

$(OBJFOLDER)/%.o : $(SRCFOLDER)/%.cpp
	@echo -e "$(no_color)Compiling $(yellow)$(notdir $@)$(no_color)..."
	@$(GXX)  -std=c++17 -c $< -L $(LIBS) -I $(INCLUDES) $(CXXFLAGS) -DBUILD=$(BUILDNUMBER) -o $@

$(OBJFOLDER)/%.o: $(SRCFOLDER)/%.c
	@echo -e "$(no_color)Compiling $(yellow)$(notdir $@)$(no_color)..."
	@$(GXX)  -std=c++17 -c $< -L $(LIBS) -I $(INCLUDES) $(CXXFLAGS) -o $@

$(OBJFOLDER)/%.mingw64.o : $(SRCFOLDER)/%.cpp
	@echo -e "$(no_color)Compiling $(yellow)$(notdir $@)$(no_color)..."
	$(WINDOWSGXX)  -std=c++17 -c $< -L $(LIBS)/mingw64 -I $(INCLUDES) $(WINDOWSCXXFLAGS) -DBUILD=$(BUILDNUMBER) -o $@

$(OBJFOLDER)/%.mingw64.o: $(SRCFOLDER)/%.c
	@echo -e "$(no_color)Compiling $(yellow)$(notdir $@)$(no_color)..."
	@$(WINDOWSGXX)  -std=c++17 -c $< -L $(LIBS)/mingw64  -I $(INCLUDES) $(WINDOWSCXXFLAGS) -o $@

$(OUT): $(OBJECTNAMES)
	@echo -e "$(no_color)Linking $(yellow)$(notdir $(OUT))...$(no_color)"
	@$(GXX) -std=c++17 $(ADDITIONALOBJS) $(OBJECTNAMES) -L $(LIBS) -I $(INCLUDES) $(CXXFLAGS) -o $(OUT)
	@echo -e "$(no_color)Incrementing the $(yellow)Build Configuration Number...$(no_color)"
	@echo $(shell expr $(BUILDNUMBER) + 1) > $(BUILDFILE)
	@echo -e "$(no_color)Launching $(yellow)$(notdir $(OUT))...$(no_color)"
	@$(OUT)

windows: $(MINGWOBJECTS) $(MINGWADDITIONAL) $(BUILDFILE)
		@echo -e "$(no_color)Linking $(yellow)$(notdir $(OUT))...$(no_color)"
		@$(WINDOWSGXX) -std=c++17 $(MINGWADDITIONAL) $(MINGWOBJECTS) -L $(LIBS)/mingw64 -I $(INCLUDES) $(WINDOWSCXXFLAGS) -o $(OUT).exe
		@echo -e "$(no_color)Incrementing the $(yellow)Build Configuration Number...$(no_color)"
		@echo $(shell expr $(BUILDNUMBER) + 1) > $(BUILDFILE)
		@echo -e "$(no_color)Launching $(yellow)$(notdir $(OUT))...$(no_color)"
		@$(OUT).exe 

clean: $(OBJFOLDER)
	@echo -e "$(no_color)Removing $(yellow)'$(OBJFOLDER)'$(no_color)..."
	@$(RM) $(OBJFOLDER)

run:
	@$(OUT)

runwindows:
	@$(OUT).exe