ifndef GLOO_RENDERING
GLOO_RENDERING=GLOO_RENDERING

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_RENDERING
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_RENDERING_OBJECTS=renderer.o light.o

# the libraries this library depends on
GLOO_RENDERING_LIBS=gloo_shader gloo_tools

# the headers in this library
GLOO_RENDERING_HEADERS=renderer.h light.h

GLOO_RENDERING_LINK=$(addprefix -l, $(GLOO_RENDERING_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_RENDERING_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_rendering/, $(GLOO_RENDERING_OBJECTS))
GLOO_RENDERING_HEADER_FILENAMES=$(addprefix $(L)/gloo_rendering/, $(GLOO_RENDERING_HEADERS))
GLOO_RENDERING_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_RENDERING_LIBS))
GLOO_RENDERING_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_RENDERING_LIBS))

include $(GLOO_RENDERING_MAKEFILES)

all: $(L)/gloo_rendering/libgloo_rendering.a

$(L)/gloo_rendering/libgloo_rendering.a: $(GLOO_RENDERING_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_rendering/*.h $(L)/include/gloo

$(GLOO_RENDERING_OBJECTS_FILENAMES): %.o: %.cpp $(GLOO_RENDERING_FILENAMES) $(GLOO_RENDERING_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(GLM_INCLUDE) $< -o $@ -I../../dependencies/glm

deepclean: cleanGLOO_RENDERING

cleanGLOO_RENDERING:
	$(RM) $(GLOO_RENDERING_OBJECTS_FILENAMES) $(L)/gloo_rendering/libgloo_rendering.a

endif

# $(L)/gloo_rendering/libgloo_rendering.a: transform.o
# 	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_rendering/*.h $(L)/include/gloo

# transform.o: transform.cpp ../../dependencies/glm
# 	 	$(CXX) $(CXXFLAGS) -c transform.cpp 

# main: main.o ../directory1/libmylib.a
#     $(CC) main.o $(LDFLAGS) -lmylib -o main

# main.o: main.c ../directory1/myheader.h
#     $(CC) $(CFLAGS) -c main.c

# ifeq ($(CLEANFOLDER), GLOO_RENDERING)
# cleanGLOO_RENDERING: cleanGLOO_RENDERING
# endif