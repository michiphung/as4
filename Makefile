CC = g++

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iexample_00/glut-3.7.6-bin
	LDFLAGS = -lglut -lGL
endif
	
RM = /bin/rm -f 
all: main 
main: as4.o IKSolver.o Algebra3.o Joint.o 
	$(CC) $(CFLAGS) -o as4 as4.o IKSolver.o Algebra3.o Joint.o $(LDFLAGS)
as4.o: as4.cpp Algebra3.h
	$(CC) $(CFLAGS) -c as4.cpp -o as4.o
Algebra3.o: Algebra3.cpp Algebra3.h
	$(CC) $(CFLAGS) -c Algebra3.cpp
IKSolver.o: IKSolver.cpp IKSolver.h
	$(CC) $(CFLAGS) -c IKSolver.cpp
Joint.o: Joint.cpp Joint.h
	$(CC) $(CFLAGS) -c Joint.cpp
clean: 
	$(RM) *.o as4
 

