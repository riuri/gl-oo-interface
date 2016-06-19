ifndef GLOO_SHADER
GLOO_SHADER=GLOO_SHADER

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_SHADER
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_SHADER_OBJECTS=shader_program.o

# the libraries this library depends on
GLOO_SHADER_LIBS=

# the headers in this library
GLOO_SHADER_HEADERS=shader_program.h

GLOO_SHADER_LINK=$(addprefix -l, $(GLOO_SHADER_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_SHADER_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_shader/, $(GLOO_SHADER_OBJECTS))
GLOO_SHADER_HEADER_FILENAMES=$(addprefix $(L)/gloo_shader/, $(GLOO_SHADER_HEADERS))
GLOO_SHADER_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_SHADER_LIBS))
GLOO_SHADER_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_SHADER_LIBS))

include $(GLOO_SHADER_MAKEFILES)

all: $(L)/gloo_shader/libgloo_shader.a

$(L)/gloo_shader/libgloo_shader.a: $(GLOO_SHADER_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_shader/*.h $(L)/include/gloo

$(GLOO_SHADER_FILENAMES): %.o: %.cpp $(GLOO_SHADER_FILENAMES) $(GLOO_SHADER_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

# ifeq ($(CLEANFOLDER), GLOO_SHADER)
# cleanGLOO_SHADER: cleanGLOO_SHADER
# endif

deepclean: cleanGLOO_SHADER

cleanGLOO_SHADER:
	$(RM) $(GLOO_SHADER_FILENAMES) $(L)/gloo_shader/libgloo_shader.a

endif