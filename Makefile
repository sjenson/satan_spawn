make:
	em++ -O3 -s ALLOW_MEMORY_GROWTH=1 --bind fungal_growth.cpp -o fungus.html --std=c++14 -I/usr/local/include/eigen3 -I~/dev/emsdk/emscripten/1.38.6 --emrun

run:
	emrun --browser chrome index.html
