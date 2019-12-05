LIBRARIES = -I"./lib/lua" -L"./lib/lua" -llua -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
build:
	g++ -w -std=c++14 ./src/*.cpp -o game ${LIBRARIES}
clean:
	rm ./game

run:
	./game
