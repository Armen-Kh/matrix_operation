SOURCES = $(wildcard *.cpp)
TARGETS = $(subst .cpp,.exe, $(SOURCES)) 

START : $(TARGETS)

%.exe : %.cpp
	g++ -o $@ $<

clean :
	rm -fv *.exe
