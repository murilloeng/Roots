#compiler
CXX = g++
WARS = -Wall
INCS = -I .. -I ../external/cpp/inc
CXXFLAGS = -std=c++20 -fPIC -pipe -fopenmp -MT $@ -MMD -MP -MF $(subst .o,.d, $@) $(DEFS) $(INCS) $(WARS)

#mode
ifneq ($(m), r)
	mode = debug
	CXXFLAGS += -ggdb3
else
	mode = release
	CXXFLAGS += -Ofast
endif

#ouput
out = dist/$(mode)/roots.out
canvas = ../Canvas/dist/$(mode)/libcanvas.so

#sources
src := $(sort $(shell find -path './src/*.cpp'))

#objects
obj = $(sort $(subst ./src/, build/$(mode)/, $(addsuffix .o, $(basename $(src)))))

#dependencies
dep = $(subst .o,.d, $(obj))

#rules
all : $(out)

run : $(out)
	./$(out)

debug : $(out)
	gdb ./$(out)

canvas : 
	@cd ../Canvas && make -f Makefile m=$m

$(out) : canvas $(obj)
	@mkdir -p $(dir $@)
	@g++ -o $(out) $(obj) $(canvas) -l GLEW -l glut -l freetype
	@echo 'executable - $(mode): $@'

build/$(mode)/%.o : src/%.cpp build/$(mode)/%.d
	@echo 'compiling($(mode)): $<'
	@mkdir -p $(dir $@) && rm -rf $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(dep) : ;

clean :
	@rm -rf dist/$(mode)
	@rm -rf build/$(mode)
	@echo 'clean($(mode)): complete!'

print-% :
	@echo $* = $($*)

.PHONY : all clean print-%