flags=-g0 -O0 -lX11 -lGL -lvlc
game_engine=./demo/bird2d.a
game_engine_shared=./bird2d.so
game_engine_object=./bird2d.o
main_demo=./demo/demo
tilemap_demo=./demo/tilemap
isometric_demo=./demo/isometric
parallax_demo=./demo/parallax
camera_demo=./demo/camera

all: engine examples
engine-shared:
	@$(CXX) -c -fPIC bird2d.cpp $(flags) -o $(game_engine_object)
	@$(CXX) -shared -o $(game_engine_shared) $(game_engine_object)
	@rm *.o
	@echo "The game engine was successfully compiled"
engine:
	@$(CXX) -c bird2d.cpp $(flags) -o $(game_engine_object)
	@$(AR) -r $(game_engine) $(game_engine_object)
	@rm *.o
	@echo "The game engine was successfully compiled"
examples:
	@$(CXX) tilemap.cpp $(game_engine) $(flags) -o $(tilemap_demo)
	@$(CXX) demo.cpp $(game_engine) $(flags) -o $(main_demo)
	@$(CXX) isometric.cpp $(game_engine) $(flags) -o $(isometric_demo)
	@$(CXX) parallax.cpp $(game_engine) $(flags) -o $(parallax_demo)
	@$(CXX) camera.cpp $(game_engine) $(flags) -o $(camera_demo)
	@chmod ugo+rx $(main_demo)
	@chmod ugo+rx $(tilemap_demo)
	@chmod ugo+rx $(isometric_demo)
	@chmod ugo+rx $(parallax_demo)
	@chmod ugo+rx $(camera_demo)
	@echo "The examples have been successfully compiled"
clean:
	@rm $(game_engine)
	@rm $(tilemap_demo)
	@rm $(main_demo)
	@rm $(isometric_demo)
	@rm $(parallax_demo)
	@rm $(camera_demo)
	@echo "Cleaning was successfully completed"