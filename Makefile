CXX=g++-4.9
CXXOPTIMIZE= -O0
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE
SERVERCLASSES= server.cpp http-request.cpp http-response.cpp
CLIENTCLASSES= client.cpp http-request.cpp http-response.cpp

all: web-server web-client

web-server: $(SERVERCLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

web-client: $(CLIENTCLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
