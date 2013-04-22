CXX = g++
DEBUGFLAGS = -c -g -Wall

QUANTA_CFLAGS=-I${SDIR}/QUANTA/include
QUANTA_LDFLAGS=-L${SDIR}/lib -lquanta
GLEW_LIB= -lGLEW

CFLAGS = -D_REENTRANT $(DEBUGFLAGS) -D_XOPEN_SOURCE=500 -D_GNU_SOURCE
CFLAGS += -I./include -I./common -I./network 
CFLAGS += `Wand-config --cflags --cppflags` -fno-stack-protector
LIBS = -lpthread -lGL -lGLU -lglut -lm -lMagickWand -lMagickCore $(QUANTA_LDFLAGS) $(GLEW_LIB)
# -L../../lib -lsail

#all: server#client server
#all: client server
#client:
#	$(CC) $(DEBUGFLAGS) -o client client.cpp $(CFLAGS) $(LIBS)
default: viewer_server viewer_client
viewer_server: viewer_server.o server.o funcs.o
	$(CXX) viewer_server.o server.o funcs.o -o viewer_server $(LIBS)
viewer_client: viewer_client.o client.o funcs.o client_scene.o image_loader.o
	$(CXX) viewer_client.o client.o funcs.o client_scene.o image_loader.o -o viewer_client $(LIBS)
viewer_server.o: viewer_server.cpp
	$(CXX) $(DEBUGFLAGS) -c viewer_server.cpp $(CFLAGS) $(LIBS)
server.o:
	$(CXX) $(DEBUGFLAGS) -c server.cpp $(CFLAGS) $(LIBS)
viewer_client.o:
	$(CXX) $(DEBUGFLAGS) -c viewer_client.cpp $(CFLAGS) $(LIBS)
funcs.o:
	$(CXX) $(DEBUGFLAGS) -c funcs.cpp $(CFLAGS) $(LIBS)
client.o:
	$(CXX) $(DEBUGFLAGS) -c client.cpp $(CFLAGS) $(LIBS)
client_scene.o:
	$(CXX) $(DEBUGFLAGS) -c client_scene.cpp $(CFLAGS) $(LIBS)
image_loader.o:
	$(CXX) $(DEBUGFLAGS) -c image_loader.cpp $(CFLAGS) $(LIBS)
clean:
	rm -f *.o viewer_server viewer_client
