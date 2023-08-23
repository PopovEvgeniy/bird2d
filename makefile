flags=-g0 -O0 -lX11 -lGL

library:
	@g++ -c bird2d.cpp $(flags) -o bird2d.o
	@ar -r ./demo/bird2d.a bird2d.o
	@rm *.o
examples:
	@g++ tilemap.cpp ./demo/bird2d.a $(flags) -o ./demo/tilemap
	@g++ demo.cpp ./demo/bird2d.a $(flags) -o ./demo/demo
	@chmod ugo+rx ./demo/tilemap
	@chmod ugo+rx ./demo/demo
clean:
	@rm ./demo/bird2d.a
	@rm ./demo/tilemap
	@rm ./demo/demo
all: library examples