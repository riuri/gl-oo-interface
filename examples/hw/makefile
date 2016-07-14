ifndef HW
HW=HW

ifndef CLEANFOLDER
CLEANFOLDER=HW
endif

include ../../build/makefile-header
R ?= ../..

# Add object files that this example needs.
HW_OBJECTS=hw.o

# Add any libraries on which this example depends.
HW_LIBS=gloo_shader gloo_tools gloo_obj gloo_glut

# Add header files for this example.
HW_HEADERS=

# Link example with libraries.
HW_LINK=$(addprefix -l, $(HW_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

HW_OBJECTS_FILENAMES=$(addprefix $(R)/examples/hw/, $(HW_OBJECTS))
HW_HEADER_FILENAMES =$(addprefix $(R)/examples/hw/, $(HW_HEADERS))
HW_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(HW_LIBS))
HW_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(HW_LIBS))

include $(HW_LIB_MAKEFILES)

all: $(R)/examples/hw/hw

CURRENT_DIR = $(shell pwd)
$(R)/examples/hw/hw: $(HW_OBJECTS_FILENAMES)
	$(CXXLD) $(LDFLAGS) $(HW_OBJECTS) $(HW_LINK) -o $@

$(HW_OBJECTS_FILENAMES): %.o: %.cpp $(HW_LIB_FILENAMES) $(HW_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(GLUI_INCLUDE) $< -o $@ -I../../dependencies/glm

ifeq ($(CLEANFOLDER), SIMULATOR)
clean: cleaninteractiveDeformableSimulator
endif

deepclean: cleanHW

cleanHW:
	$(RM) $(HW_OBJECTS_FILENAMES) $(R)/examples/hw/hw

endif
