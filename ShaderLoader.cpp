//the damn Shader Loader Program
//By Richard Davis: Computer Engineer Clarkson university Spring 2014


//Shaders are this mini programs that run on the GPU instead of CPU
//But we need to get them there
//So we need a loader function
//So lets make one

//First include Angel's Crazy Library because why not at this point?
#include "Angel.h" // His file

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cstdio>
#include <iostream>

namespace Angel { //Honestly this scares me, last time it segfaulted

//We load our shader files as an array of characters AKA a string
//So lets make that string variable!
static char * //A static variable inside a function keeps its value between invocations //Honestly: Shouldn't there be a ; HERE?
readMeShaderFile(const char* shaderFileName){//The magic happens here
	//We are going to read that file and copy it into a wonderful string that gets returned to the program
	//Input:shaderFileName = the .glsl file on your hardrive that has your shader code in it... goes here
	//Output: buffy = the string that holds all that glsl code
	//Local Vars
	FILE* readInFile; //This is the stuff in file that gets made into our string
	long size; //This is the size of (num chars) that file -and hence in our char array/string //being long means its at least 32 bits--so we can have big files w/o issues
	char* buffy; //This is our string of stuff in that file//Angel used buf, I'm way too tired right now.
	//Start coding
	readInFile = fopen(shaderFileName, "r");//open as read only, last thing we want to do is change anything //be sure to close this later
	if (readInFile== NULL ) { //If the file be empty
		return NULL;//return empty
		std::cerr << "WARNING: The file you gave me " << shaderFileName << " was empty. WTF?";//and yell at the programmer
	}//Enough yelling

	fseek(readInFile, 0L, SEEK_END);//No binary header=No offset, Find the end of that File!
	size = ftell(readInFile);//get the size of that file
	
	fseek(readInFile,0L,SEEK_SET);//having read to the end, lets read it again more carefully-- this takes us back to the beginning of that file
	buffy = new char[size + 1];//allocate memory for the array of chars (named buffy -- I'm gonna regret this) which is the same size as the (file + 1)
	fread(buffy, 1, size, readInFile);//read an array of size (size) consisting of 1 byte chars from the readInFile and store it into a block of memory which is of at least size (size) of file (buffy!)
	buffy[size] = ' ';//make the very end of the buffer into a space (so we know when it ends?)
	fclose(readInFile);//WE ARE DONE with that physical file! CLOSE THE FILE before anything unexpected/ stupid happens

	return buffy;//Return a buffer that includes everything in that was in that file
}//End ze readMeShaderFile function

//Having the shader code in our buffer, we can use that to create GLSL program oobjects to invoke!

//Function #2
GLuint InitShader(const char* vShaderFileName, const char* fShaderFileName){
	//Given the names of our vertex and fragment shader files containing precious GLSL code on our hard drive, generate a GLSL program object!
        //Input:vShaderFileName = the .glsl file on your hardrive that has your vertex shader code in it... goes here
	//fShaderFileName = the .glsl file on your hardrive that has your fragment shader code in it... goes here
        //Output: GLuint program //the program object containing all of aour shaders!
        //Local Vars
	GLuint program; //the program object which contains our shaders!
	const int numShaders = 2;//number of shaders in our array of shaders, typically 2, a vertex + a fragment shader

	//Structure of a Shader
	struct Shader {//Our typical shader consists of: //NOTE: does not contain OpenGL shader objects
		const char*	filename;//the name of it's GLSL file
		GLenum  	type;//Shader's type-- Usually this is either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		GLchar*		source;//This is what is in the GLSL file, default to NULL before loading
	}//End Shader Structure
	shaders[numShaders] = {//Since we usually have 2 shaders, create an array of ...well... 2 shaders
		{ vShaderFileName, GL_VERTEX_SHADER, NULL }, //The Vertex Shader
		{ fShaderFileName, GL_FRAGMENT_SHADER, NULL  }//The Fragment Shader
	};//End array of shaders
	
	program = glCreateProgram ();//YES!creates an empty program object
	
	//increment through our array of shaders and load their actual values
	for ( int i = 0; i < numShaders; ++i) {//I learned something NEW! ++i != i++ apparently //EG i=0, x=i++ -> i=1, x=1 (post value of i) vs x=++i -> i=1, x=0 (pre value of i_) 
		Shader& s = shaders[i];//create a pointer to the ith shader in the array? -- named s for ease of typing
		s.source = readMeShaderFile(s.filename); //Swizzling is nice
		if ( s.source == NULL ) {//if the shader file is empty
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit( EXIT_FAILURE );//Yell at the programmer and kill the process in FAILURE
		}//end if statement
	        GLint shaderID;//this is the identifier handle number for a OpenGL shader object--there is one for each of our shaders [in this case 2: vertex and fragment]

		shaderID = glCreateShader(s.type);//Create an empty shader object of appropriate type specified in the Shader array of the for loop counter and return a nonzero ID#
		glShaderSource(shaderID, 1, (const GLchar**) &s.source, NULL);//replace the source code in a the empty shader object handled by shaderID with the string of GLSL code in s.source
		glCompileShader( shaderID );//compiles the source code strings that have been stored in the shader object specified by shaderID

//Note: Compilation of a shader can fail for a number of reasons as specified by the OpenGL ES 
//Shading Language Specification. Whether or not the compilation was successful, information about the 
//compilation can be obtained from the shader object's information log by calling glGetShaderInfoLog

		GLint compiled;//varaible to hold result of if the compilation suceessful
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compiled );//sets compiled to GL_TRUE if last compile of shaderID successful
		if ( !compiled ) {//if compilation fails
			std::cerr << s.filename << " failed to compile:"<<std::endl;//yell at the programmer
			GLint logSize;//variable for logsize
			glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &logSize );//sets value of logsize
			char* logMsg = new char[logSize];//creates a dynamic string big enough to hold the compilation log's message
			glGetShaderInfoLog( shaderID, logSize, NULL, logMsg );//return the information log for the shader object referred to by shaderID, allow a buffer of LogSize char in length and store it in logMsg //the length of the string in the log isn't required so that parameter is NULL
			std::cerr << logMsg << std::endl;//Yell at the programmer whatever error was in the shader's compilation log when it failed
			delete [] logMsg;//Clean up after yourself! Remove dynamic objects when finished with them!
			exit( EXIT_FAILURE );//having yelled at the programmer, kill the process in FAILURE
		}//end if loop

		delete [] s.source;// Clean up after yourself! Remove dynamic objects when finished with them!
		glAttachShader( program, shaderID );//FINALLY//attaches a shader object to a program object
	}//end for loop

	//link and error check -- very similiar to above except with a programID instead of shaderID
	glLinkProgram(program);//links the program object specified by programID, attaching the shader objects to create a GPU executable

	GLint	linked;//holds the status of the link operation (GL_TRUE if linked w/o errors)
	glGetProgramiv( program, GL_LINK_STATUS, &linked );//querry link status and assign result to linked
	if ( !linked ){//If linking failed
		std::cerr << "Shader program failed to link" << std::endl;//yell at the programmer
		GLint logSize;//holds the size (# of chars) of log
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logSize );//returns # of chars in the info log for program, including null terminator, allows us to calculate the size of the buffer required to store the info log
		char* logMsg = new char[logSize];//creates a dynamic string large enough to hold the linker's error message if it failed
		glGetProgramInfoLog ( program, logSize, NULL, logMsg );//return the information log for the program object referred to by program, allow a buffer of LogSize char in length and store it in logMsg //the length of the string in the log isn't required so that parameter is NULL
		std::cerr << logMsg << std::endl;//Yell at the programmer whatever error was in the shader's compilation log when it failed
		delete [] logMsg;//Clean up after yourself! Remove dynamic objects when finished with them!

		exit( EXIT_FAILURE );//having yelled at the programmer, kill the process in FAILURE
	}//end if loop
	
	//Use program object--Finally!
	glUseProgram(program);//Installs the program object as part of the current rendering state!
	
	//For matrix math	
	glm::mat4 view;
	view = glm::lookAt(//position and direction of camera
		glm::vec3(0.0f, 0.0f, 50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	GLint tempLoc = glGetUniformLocation(program, "viewMatrix");//Matrix that handles the camera movement
	glUniformMatrix4fv(tempLoc, 1, GL_FALSE, &view[0][0]);
  
	glm::mat4 mainProjMatrix;
	mainProjMatrix = glm::perspective(57.0,1.0,.1,500.0);//Matrix that handle the orthographic or perspective viewing
	tempLoc = glGetUniformLocation(program, "Matrix");
	glUniformMatrix4fv(tempLoc, 1, GL_FALSE, &mainProjMatrix[0][0]);
	
	return program;//returns programID to main function
}//End the InitShaderFunction


}//Close the angel namespace block

