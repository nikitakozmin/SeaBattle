CC = g++
CFLAGS = -std=c++20#-Wall -Wextra
EXE = cw.exe

all: $(EXE)

$(EXE): main.o ship.o field.o ship_manager.o ability_manager.o double_damage.o scanner.o bombardment.o local_exceptions.o game.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.cpp
	$(CC) $(CFLAGS) -c $^

ship.o: ship.cpp ship.hpp
	$(CC) $(CFLAGS) -c $^

field.o: field.cpp field.hpp
	$(CC) $(CFLAGS) -c $^

ship_manager.o: ship_manager.cpp ship_manager.hpp
	$(CC) $(CFLAGS) -c $^

ability_manager.o: ability_manager.cpp ability_manager.hpp ability.hpp
	$(CC) $(CFLAGS) -c $^

double_damage.o: double_damage.cpp double_damage.hpp ability.hpp
	$(CC) $(CFLAGS) -c $^

scanner.o: scanner.cpp scanner.hpp ability.hpp
	$(CC) $(CFLAGS) -c $^

bombardment.o: bombardment.cpp bombardment.hpp ability.hpp
	$(CC) $(CFLAGS) -c $^

local_exceptions.o: local_exceptions.cpp local_exceptions.hpp
	$(CC) $(CFLAGS) -c $^

game.o: game.cpp game.hpp
	$(CC) $(CFLAGS) -c $^

clear:
ifeq ($(EXE),cw.exe)
	del *.o
	del *.gch
else
	rm *.o
	rm *.gch
endif
