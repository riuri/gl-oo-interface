ifndef GLOO_GLUT
GLOO_GLUT=GLOO_GLUT

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_GLUT
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_GLUT_OBJECTS=glut_view_controller.o

# the libraries this library depends on
GLOO_GLUT_LIBS=

# the headers in this library
GLOO_GLUT_HEADERS=glut_view_controller.h model_base.h

GLOO_GLUT_LINK=$(addprefix -l, $(GLOO_GLUT_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_GLUT_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_glut/, $(GLOO_GLUT_OBJECTS))
GLOO_GLUT_HEADER_FILENAMES=$(addprefix $(L)/gloo_glut/, $(GLOO_GLUT_HEADERS))
GLOO_GLUT_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_GLUT_LIBS))
GLOO_GLUT_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_GLUT_LIBS))

include $(GLOO_GLUT_MAKEFILES)

all: $(L)/gloo_glut/libgloo_glut.a

$(L)/gloo_glut/libgloo_glut.a: $(GLOO_GLUT_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_glut/*.h $(L)/include/gloo

$(GLOO_GLUT_FILENAMES): %.o: %.cpp $(GLOO_GLUT_FILENAMES) $(GLOO_GLUT_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

# ifeq ($(CLEANFOLDER), GLOO_GLUT)
# cleanGLOO_GLUT: cleanGLOO_GLUT
# endif

deepclean: cleanGLOO_GLUT

cleanGLOO_GLUT:
	$(RM) $(GLOO_GLUT_OBJECTS_FILENAMES) $(L)/gloo_glut/libgloo_glut.a

endif