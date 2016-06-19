ifndef GLOO_TOOLS
GLOO_TOOLS=GLOO_TOOLS

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_TOOLS
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_TOOLS_OBJECTS=transform.o

# the libraries this library depends on
GLOO_TOOLS_LIBS=

# the headers in this library
GLOO_TOOLS_HEADERS=transform.h

GLOO_TOOLS_LINK=$(addprefix -l, $(GLOO_TOOLS_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_TOOLS_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_tools/, $(GLOO_TOOLS_OBJECTS))
GLOO_TOOLS_HEADER_FILENAMES=$(addprefix $(L)/gloo_tools/, $(GLOO_TOOLS_HEADERS))
GLOO_TOOLS_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_TOOLS_LIBS))
GLOO_TOOLS_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_TOOLS_LIBS))

include $(GLOO_TOOLS_MAKEFILES)

all: $(L)/gloo_tools/libgloo_tools.a

$(L)/gloo_tools/libgloo_tools.a: $(GLOO_TOOLS_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_tools/*.h $(L)/include/gloo

$(GLOO_TOOLS_FILENAMES): %.o: %.cpp $(GLOO_TOOLS_FILENAMES) $(GLOO_TOOLS_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

# ifeq ($(CLEANFOLDER), GLOO_TOOLS)
# cleanGLOO_TOOLS: cleanGLOO_TOOLS
# endif

deepclean: cleanGLOO_TOOLS

cleanGLOO_TOOLS:
	$(RM) $(GLOO_TOOLS_OBJECTS_FILENAMES) $(GLOO_TOOLS_FILENAMES) $(L)/gloo_tools/libgloo_tools.a

endif