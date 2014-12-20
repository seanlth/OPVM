C=clang++                                                                                
FLAGS= -std=c++11 -o OPVM
all: opvm
spartans: main.cpp 
	$(C) $(FLAGS) main.cpp Lexer.cpp Parser.cpp
