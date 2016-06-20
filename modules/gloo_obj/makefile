ifndef GLOO_OBJ
GLOO_OBJ=GLOO_OBJ

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_OBJ
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_OBJ_OBJECTS=test.o

# the libraries this library depends on
GLOO_OBJ_LIBS=

# the headers in this library
GLOO_OBJ_HEADERS=test.h

GLOO_OBJ_LINK=$(addprefix -l, $(GLOO_OBJ_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_OBJ_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_obj/, $(GLOO_OBJ_OBJECTS))
GLOO_OBJ_HEADER_FILENAMES=$(addprefix $(L)/gloo_obj/, $(GLOO_OBJ_HEADERS))
GLOO_OBJ_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_OBJ_LIBS))
GLOO_OBJ_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_OBJ_LIBS))

include $(GLOO_OBJ_MAKEFILES)

all: $(L)/gloo_obj/libgloo_obj.a

$(L)/gloo_obj/libgloo_obj.a: $(GLOO_OBJ_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_obj/*.h $(L)/include/gloo

$(GLOO_OBJ_OBJECTS_FILENAMES): %.o: %.cpp $(GLOO_OBJ_FILENAMES) $(GLOO_OBJ_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), GLOO_OBJ)
cleanGLOO_OBJ: cleanGLOO_OBJ
endif

deepclean: cleanGLOO_OBJ

cleanGLOO_OBJ:
	$(RM) $(GLOO_OBJ_FILENAMES) $(L)/gloo_obj/libgloo_obj.a

endif