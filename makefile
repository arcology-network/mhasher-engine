.PHONY : clean test

CPPFLAGS= -std=c++17 -fPIC -Wall -Wextra -O3
LDFLAGS= -std=c++17 -shared

SOURCES = mhasher.external.cpp binaryset.external.cpp string.engine.cpp
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libmhasher.so

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) test

test:
	g++ $(CPPFLAGS) mhasher.external.cpp mhasher_external_test.cpp main.cpp -ltbb -lcryptopp -o test

$(TARGET) : $(OBJECTS)
	g++ $(CPPFLAGS) $(OBJECTS) -o $@ $(LDFLAGS) -lboost_system -lcryptopp -ltbb

