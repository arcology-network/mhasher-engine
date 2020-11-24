#######################
# Makefile
#######################

# compile and lib parameter
CC      := g++
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I. -I/usr/include -I/usr/loca/include
CFLAGS  := 
CXXFLAGS:= 

# link parameter
LIB := libmtrie.so

#g++ -std=c++17 -O3 -c -fPIC -Wall -I/usr/include/  ./*.cpp
#g++ -shared -O3 -o libmtrie.so  ./*.o -l:/libtbb.so -l:libcryptopp.so

#link
$(LIB):mtrie.external.o
	$(CC) -shared -O3 -o $(LIB)  ./*.o -l:/libtbb.so -l:libcryptopp.so


#compile
mtrie.external.o:mtrie.external.cpp
	$(CC) -std=c++17 -O3 -c -fPIC -Wall -I/usr/include/ ./*.cpp
	

# clean
clean:
	rm -fr *.o *.gch 