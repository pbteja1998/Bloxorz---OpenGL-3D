all: sample2D

sample2D: Sample_GL3_3D.cpp glad.c
	g++ -g -o sample2D Sample_GL3_3D.cpp glad.c -lSOIL -lGL -lglfw -ldl 

clean:
	rm sample2D
