#
# C++ Makefile template
#


BIN_NAME     = test
# yes / no
IS_LIBRARY   = no

SRC_DIR      = .
CPP_FILES    = $(shell ls $(SRC_DIR)/*.cpp)
H_FILES      = 
OBJECT_FILES = $(foreach CPP_FILE, $(CPP_FILES), $(patsubst %.cpp,%.o,$(CPP_FILE)))
DEP_FILES    = $(foreach CPP_FILE, $(CPP_FILES), $(patsubst %.cpp,%.d,$(CPP_FILE)))

CXXFLAGS     = -ggdb3 -O0 -Wall -I..
LDFLAGS      = -L.. -lcppapp -rdynamic

ECHO         = $(shell which echo)


build: $(BIN_NAME)


-include $(DEP_FILES)


clean:
	@echo "========= CLEANING ========="
	rm -f $(OBJECT_FILES) $(BIN_NAME)
	@echo


rebuild:
	@$(MAKE) clean
	@$(MAKE) build


deps: $(DEP_FILES)


clean-deps:
	rm -f $(DEP_FILES)


$(BIN_NAME): $(OBJECT_FILES)
ifeq ($(IS_LIBRARY),yes)
	@echo "========= LINKING LIBRARY $@ ========="
	$(AR) -r $@ $^
else
	@echo "========= LINKING EXECUTABLE $@ ========="
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
endif
	@echo


.PHONY: all build clean rebuild deps clean-deps


%.d: %.cpp $(H_FILES)
	@$(ECHO) "Generating \"$@\"..."
	@$(ECHO) -n "$(SRC_DIR)/" > $@
	@$(CXX) $(CXXFLAGS) -MM $< >> $@


