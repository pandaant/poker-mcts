# compiler
CXX = clang++ -g

INCLUDES=-isystem ../../dep/decimal_for_cpp/include \
		 -isystem ../../dep/rapidjson/include \
		 -I ./include/freedom \
		 -I ../libmcts/include \
		 -I ../libecalc/include \
		 -I ../libpoker/include \

# add paths of libraries the
# targets depend on.
#LIBRARIES = -L ./../lib/$(target) -lecalc \
			#-L ../../libpoker/lib/$(target) -lpoker \
			#-L ../../../dep/UnitTest++ -lUnitTest++ 
		 
CXXFLAGS=-m64 -static -ansi -std=c++11 \
		 -MMD -MP 

DOC_OUT = doc

ifeq ($(target),debug)
    CXXFLAGS +=-O0 -Weverything -Wno-c++98-compat #-Werror
else
    target = release
	CXXFLAGS +=-O3 -Wall -Wno-unused-function 
endif

# name ob linked lib
LIB_OUT = lib/$(target)/libfreedom.a

# gobble a files for compilation
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/$(target)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(OBJ_FILES:.o=.d)

$(LIB_OUT): $(OBJ_FILES)
	ar rcs $@ $^

obj/$(target)/%.o: src/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(LIBRARIES) -c -o $@ $<

all: $(LIB_OUT)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DEP_FILES)
	rm -f $(LIB_OUT)
	rm -f -r $(DOC_OUT)

doc:
	mkdir -p $(DOC_OUT)
	doxygen doxyfile

-include $(DEP_FILES)

.PHONY: clean all doc
