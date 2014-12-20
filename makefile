C=clang++                                                                                
FLAGS= -std=c++11 -o OPVM
all: opvm
opvm: main.cpp
	$(C) $(FLAGS) main.cpp Lexer.cpp Parser.cpp
