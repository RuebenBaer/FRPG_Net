CXX = g++
CPPFLAGS =  \
	-Wall \
	-Werror \
	-Wpedantic \
	-pipe \
	-mthreads \
	-O3 \
	-std=gnu++11

INCLUDES =  \
	-I$(WXINCLUDE) \
	-I$(WXINCLUDESETUP)

DEFS =  \
	-D__WXMSW__ \
	-DwxUSE_UNICODE

LIBS = \
	-lkernel32 \
	-luser32 \
	-lgdi32 \
	-lcomdlg32 \
	-lwinspool \
	-lwinmm \
	-lshell32 \
	-lshlwapi \
	-lcomctl32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lrpcrt4 \
	-ladvapi32 \
	-lversion \
	-lws2_32 \
	-lwininet \
	-loleacc \
	-luxtheme

include wxLib.lst

OBJDIR = obj
SRCDIR = .
BINDIR = bin
BIN = $(BINDIR)\GUI_Net.exe
OBJ = \
	$(OBJDIR)\GUI_NetApp.o \
	$(OBJDIR)\GUI_NetMain.o \
	$(OBJDIR)\Dbl_Eingabe.o \
	$(OBJDIR)\NetzBerechnung.o \
	$(OBJDIR)\Kamera_3D.o \
	$(OBJDIR)\Vektor\Vektor.o \
	$(OBJDIR)\Matrix\Matrix.o \
	$(OBJDIR)\resource.res

.PHONEY: clean all

all: $(OBJ)
	$(CXX) $(CPPFLAGS) $(DEFS) -L$(WXLIBPATH) -o $(BIN) $(OBJ) -s $(LIBS)

$(OBJDIR)\GUI_NetApp.o: $(SRCDIR)\GUI_NetApp.cpp $(SRCDIR)\GUI_NetApp.h
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\GUI_NetMain.o: $(SRCDIR)\GUI_NetMain.cpp $(SRCDIR)\GUI_NetMain.h
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Dbl_Eingabe.o: $(SRCDIR)\Dbl_Eingabe.cpp $(SRCDIR)\Dbl_Eingabe.h
	if not exist $(OBJDIR)\Dbl_Eingabe mkdir $(OBJDIR)\Dbl_Eingabe
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\NetzBerechnung.o: $(SRCDIR)\NetzBerechnung.cpp $(SRCDIR)\NetzBerechnung.h
	if not exist $(OBJDIR)\aru_events mkdir $(OBJDIR)\aru_events
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Kamera_3D.o: $(SRCDIR)\Kamera_3D.cpp $(SRCDIR)\Kamera_3D.h
	if not exist $(OBJDIR)\Kamera mkdir $(OBJDIR)\Kamera
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Vektor\Vektor.o: $(SRCDIR)\Vektor\Vektor.cpp $(SRCDIR)\Vektor\Vektor.h
	if not exist $(OBJDIR)\Vektor mkdir $(OBJDIR)\Vektor
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Matrix\Matrix.o: $(SRCDIR)\Matrix\Matrix.cpp $(SRCDIR)\Matrix\Matrix.h
	if not exist $(OBJDIR)\Matrix mkdir $(OBJDIR)\Matrix
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\resource.res: $(SRCDIR)\resource.rc
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	windres.exe $(INCLUDES) -J rc -O coff -i $(SRCDIR)\resource.rc -o $(OBJDIR)\resource.res

clean:
	del /s /q $(OBJDIR)
	del /s /q $(BIN)