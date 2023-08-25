flags=-g0 -O0 -lX11 -lGL
game_engine=./demo/bird2d.a
main_demo=./demo/demo
tilemap_demo=./demo/tilemap

all: engine examples
engine:
	@g++ -c bird2d.cpp $(flags) -o bird2d.o
	@ar -r $(game_engine) bird2d.o
	@rm *.o
	@echo "The game engine successfully compiled"
examples:
	@g++ tilemap.cpp $(game_engine) $(flags) -o $(tilemap_demo)
	@g++ demo.cpp $(game_engine) $(flags) -o $(main_demo)
	@chmod ugo+rx $(main_demo)
	@chmod ugo+rx $(tilemap_demo)
	@echo "Examples successfully compiled"
clean:
	@rm $(game_engine)
	@rm $(tilemap_demo)
	@rm $(main_demo)
	@echo "Clean successfully completed"