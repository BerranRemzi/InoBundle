FILES =..\source\Arduino.cpp ..\source\Emulator.cpp ..\..\..\src\main.cpp ..\..\..\src\games\Snake.cpp ..\..\..\src\InoBundle.cpp
TARGET=InoBundle.exe
OBJS  =Arduino.o Emulator.o main.o Snake.o InoBundle.o

MINGW =$(MINGW_PATH)
LIBS  = -L"$(MINGW)\lib" -L ..\lib -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
CFLAGS= -I"$(MINGW)\include" -I ..\include
CPP   =g++.exe

all: build

build: compile
	@$(CPP) -Wall -o $(TARGET) $(OBJS) $(LIBS)

compile: $(FILES)
	@$(CPP) -Wall -c $(FILES) $(CFLAGS)
