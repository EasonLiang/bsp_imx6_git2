EXTRA_DIST = Doxygen.in

all: stamp-doxygen

stamp-doxygen:
if HAVE_DOXYGEN
	doxygen 
	touch stamp-doxygen
endif

clean: clean-local
clean-local:
	-rm -Rf html
	-rm -f stamp-doxygen