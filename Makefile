# Makefile

CXX = g++
CXXFLAGS = -g -O2 -std=c++11 -pthread -march=native
LDFLAGS = -L/usr/local/lib/ -lntl -lgmp -lm -lssl -lcrypto
TARGET = crypto_pk

# Add the new cpp files here
SOURCES = main.cpp rsa.cpp diffiehellman.cpp utility.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

all: check_deps $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

check_deps:
	@echo "Checking dependencies..."
	@case `uname` in \
		Linux*) ldconfig -p | grep libntl > /dev/null || (echo "NTL not found. Please install NTL." && exit 1); \
				ldconfig -p | grep libssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL." && exit 1) ;; \
		Darwin*)ls -l /usr/local/lib/ | egrep -i ntl > /dev/null || (echo "NTL not found. Please install NTL using Homebrew." && exit 1); \
				ls -l /usr/local/lib/ | egrep -i openssl > /dev/null || (echo "OpenSSL not found. Please install OpenSSL using Homebrew." && exit 1) ;; \
		*) echo "Unsupported operating system." && exit 1 ;; \
	esac
	@echo "All dependencies are installed."
