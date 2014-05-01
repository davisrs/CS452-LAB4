#ifndef CUBE_H
#define CUBE_H

//class Cube {

//public:
	//Cube (float size)

	//Cube::Cube (float size){//constructor
	
GLfloat setCubeSize(float size){//create a size*size*size cube centered at 0,0,0
	GLfloat vertexarray[]={
		0.0f,	0.0f,	0.0f, //0 bbl (Key:bottom/top.back/front.left/right)
		size,	0.0f,	0.0f, //1 bbr
		0.0f,	size,	0.0f, //2 tbr Top Left 0,1
		size,	size,	0.0f, //3 tbl Top Right 1,1
		0.0f,	0.0f,	size, //4 bfl
		size,	0.0f,	size, //5 bfr
		0.0f,	size,	size, //6 tfeft 0,0
		size,	size,	size, //7 tfr Bot Right 1,0
		
	};
}
	                       //r g b alpha //color array
	GLfloat colorarray[]={	
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};
	
	GLubyte elems[]={
			0,	6,	4,
			0,	2,	6,
			0,	3,	2,
			0,	1,	3,
			2,	7,	6,
			2,	3,	7,
			4,	6,	7,
			4,	7,	5,
			0,	4,	5,
			0,	5,	1,
			1,	5,	7,
			1,	7,	3
	};
#endif //cube_H
