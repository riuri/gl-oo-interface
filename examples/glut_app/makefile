ifndef GLUT_APP
GLUT_APP=GLUT_APP

ifndef CLEANFOLDER
CLEANFOLDER=GLUT_APP
endif

include ../../build/makefile-header
R ?= ../..

# Add object files that this example needs.
GLUT_APP_OBJECTS=main.o my_model.o

# Add any libraries on which this example depends.
GLUT_APP_LIBS=gloo_shader gloo_tools gloo_obj gloo_glut

# Add header files for this example.
GLUT_APP_HEADERS=my_model.h

# Link example with libraries.
GLUT_APP_LINK=$(addprefix -l, $(GLUT_APP_LIBS)) $(IMAGE_LIBS) $(STANDARD_LIBS)

GLUT_APP_OBJECTS_FILENAMES=$(addprefix $(R)/examples/glut_app/, $(GLUT_APP_OBJECTS))
GLUT_APP_HEADER_FILENAMES =$(addprefix $(R)/examples/glut_app/, $(GLUT_APP_HEADERS))
GLUT_APP_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(GLUT_APP_LIBS))
GLUT_APP_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(GLUT_APP_LIBS))

include $(GLUT_APP_LIB_MAKEFILES)

all: $(R)/examples/glut_app/glut_app

CURRENT_DIR = $(shell pwd)
$(R)/examples/glut_app/glut_app: $(GLUT_APP_OBJECTS_FILENAMES)
	$(CXXLD) $(LDFLAGS) $(GLUT_APP_OBJECTS) $(GLUT_APP_LINK) -o $@

$(GLUT_APP_OBJECTS_FILENAMES): %.o: %.cpp $(GLUT_APP_LIB_FILENAMES) $(GLUT_APP_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(GLUI_INCLUDE) $< -o $@ -I../../dependencies/glm

ifeq ($(CLEANFOLDER), SIMULATOR)
clean: cleaninteractiveDeformableSimulator
endif

deepclean: cleanGLUT_APP

cleanGLUT_APP:
	$(RM) $(GLUT_APP_OBJECTS_FILENAMES) $(R)/examples/glut_app/glut_app

endif
