//You are to create a cube.

//-The cube should have two lights surrounding it, on opposing sides from one another.
//    -For example, the lights could be on the left and right, top and bottom, or back and front.
//-There must be two different kinds of lighting.
//    -Examples are: specular, diffuse, ambient, phong, etc.
//-The camera should be adjusted via keyboard controls
//    -W,A,S, and D should move the camera up, down, left, and right respectively.

#include "ShaderLoader.cpp"
#include "cube.h"
#include "Angel.h"
#include <cstdlib>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


using namespace std;

void rotate(GLuint locate);

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;

GLuint vaoID, vboID;//vao and vbo names
GLuint eboID;
GLuint shaderProgramID;

GLuint positionID, colorID;
GLuint indexBufferID; 

void init(){
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, 600, 600);
  //Create a vertex array object
  glGenVertexArrays(1, &vaoID);//generates 1 vertex array objects for vaoID
  glBindVertexArray(vaoID);//binds VAO
  
  glGenBuffers(1, &vboID);//generates 1 vertex buffer objects that are associated with vboID to hold the color and vertex data
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  //create space in the buffer but don't load anything into it yet
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray)+sizeof(colorarray),NULL,GL_STATIC_DRAW);
  //Load vetex data into the VAO
  int offsetd = 0;
  glBufferSubData(GL_ARRAY_BUFFER, offsetd, sizeof(vertexarray), vertexarray);
  offsetd += sizeof(vertexarray);
  // Load the colors right after that
  glBufferSubData(GL_ARRAY_BUFFER, offsetd, sizeof(colorarray), colorarray);
  offsetd += sizeof(colorarray);
  
  //Index buffer (ebo)
  glGenBuffers(1,&eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);
  
  //make me a shader and use it (glUseProgram is called internal to InitShader)
  shaderProgramID = InitShader("vertexshader.glsl", "fragmentshader.glsl");//Load shaders and use the resulting shader program
  
  //specify the layout of the vertex data
  offsetd = 0;
  positionID = glGetAttribLocation(shaderProgramID, "in_position");
  glEnableVertexAttribArray(positionID);
  glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offsetd));//defines an array of vertex attribute data
  offsetd += sizeof(vertexarray);
  
  colorID = glGetAttribLocation(shaderProgramID, "in_color");
  glEnableVertexAttribArray(colorID);
  glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offsetd));
  offsetd += sizeof(colorarray);
}

void drawscene(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glUseProgram(shaderProgramID);
  glm::mat4 trans;
  trans=glm::translate(trans,cubeTran);//translate the cube
  trans=glm::rotate(trans,pit,glm::vec3(1,0,0));//rotate the cube around the x axis
  trans=glm::rotate(trans,yaw,glm::vec3(0,1,0));//rotate the cube arround the y axis
  trans=glm::scale(trans,glm::vec3(scalar));//scaling the cube
  GLint tempLoc = glGetUniformLocation(shaderProgramID,"modelMatrix");//Matrix that handle the transformations
  glUniformMatrix4fv(tempLoc,1,GL_FALSE,&trans[0][0]);
  glDrawElements(GL_TRIANGLES,sizeof(elems),GL_UNSIGNED_BYTE,NULL);
  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}

void keypress(unsigned char key, int x, int y){
switch( key ) {
case 27://escape
fprintf(stderr, "Gave up already?\n");
exit( EXIT_SUCCESS );
break;
case 'w':
cubeTran.y+=2;
printf("\nUp\n");
break;
case 's':
cubeTran.y-=2;
printf("\nDown\n");
break;
case 'a':
cubeTran.x-=2;
printf("\nLeft\n");
break;
case 'd':
cubeTran.x+=2;
printf("\nRight\n");
break;
case 'e':
scalar+=.1f;
printf("\nScaleUp\n");
break;
case 'q':
scalar-=.1f;
printf("\nScaleDown\n");
break;
case 'i':
pit+=.1;
printf("\nIncPitch\n");
break;
case 'k':
pit-=.1;
printf("\nDecPitch\n");
break;
case 'j':
yaw+=.1;
printf("\nIncYaw\n");
break;
case 'l':
yaw-=.1;
printf("\nDecYaw\n");
break;
}
}

int main(int argc,char ** argv){

  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Lab 4: Lit cube");//creates the window with the specified name
  
  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){//initialize glew, if it fails...
fprintf(stderr, "Unable to initalize GLEW");//Yell at programmer GLEW not working
exit(EXIT_FAILURE);//kill process, exit in FAILURE if GLEW doesn't work'
  }
  
  glutInitContextVersion(4, 3);//specifies the version of opengl
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);//specifies what profile your using
  
  setCubeSize(1.0f);//set the vertices in cube's vertice array to 1.0

  init();//Call init function

  //returns what version of opengl and glsl your computer can use
  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version=glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);//displays callback draws the shapes
  glutKeyboardFunc(keypress);//get keyboard inputs
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
