ifndef GLOO_MESH
GLOO_MESH=GLOO_MESH

ifndef CLEANFOLDER
CLEANFOLDER=GLOO_MESH
endif

include ../../build/makefile-header
R ?= ../..

# the object files to be compiled for this library
GLOO_MESH_OBJECTS=group.o

# the libraries this library depends on
GLOO_MESH_LIBS=

# the headers in this library
GLOO_MESH_HEADERS=group.h

GLOO_MESH_LINK=$(addprefix -l, $(GLOO_MESH_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLOO_MESH_OBJECTS_FILENAMES=$(addprefix $(L)/gloo_mesh/, $(GLOO_MESH_OBJECTS))
GLOO_MESH_HEADER_FILENAMES=$(addprefix $(L)/gloo_mesh/, $(GLOO_MESH_HEADERS))
GLOO_MESH_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLOO_MESH_LIBS))
GLOO_MESH_FILENAMES=$(call GET_LIB_FILENAMES, $(GLOO_MESH_LIBS))

include $(GLOO_MESH_MAKEFILES)

all: $(L)/gloo_mesh/libgloo_mesh.a

$(L)/gloo_mesh/libgloo_mesh.a: $(GLOO_MESH_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/gloo_mesh/*.h $(L)/include/gloo

$(GLOO_MESH_OBJECTS_FILENAMES): %.o: %.cpp $(GLOO_MESH_FILENAMES) $(GLOO_MESH_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), GLOO_MESH)
cleanGLOO_MESH: cleanGLOO_MESH
endif

deepclean: cleanGLOO_MESH

cleanGLOO_MESH:
	$(RM) $(GLOO_MESH_FILENAMES) $(L)/gloo_mesh/libgloo_mesh.a

endif