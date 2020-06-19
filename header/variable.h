//
// Created by Dark burster on 2020/6/19.
//

#ifndef OPENGL_VARIABLE_H
#define OPENGL_VARIABLE_H

#include <stdlib.h>
#include <glew.h>
#include <GLUT/glut.h>
// GLM is a C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL)
// specification.
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <FreeImage.h>
#include <iomanip>

GLfloat ramount = 2.5, slamount = 0.5, kneeamount = 0.25;
GLint maxlimit = 210, minlimit = 150;

int mouseoldx, mouseoldy; // For mouse motion
int windowWidth = 500, windowHeight = 500; //Width/Height of OpenGL window
GLdouble eyeloc = 2.0; // Where to look from; initially 0 -2, 2
GLfloat teapotlocX = 0; // ** NEW ** where the teapot is located
GLfloat teapotlocY = 0; // ** NEW ** where the teapot is located
GLfloat teapotlocZ = 0; // ** NEW ** where the teapot is located
GLfloat robotlocX = 0; // ** NEW ** where the teapot is located
GLfloat robotlocY = 0; // ** NEW ** where the teapot is located
GLfloat robotlocZ = 0; // ** NEW ** where the teapot is located
GLfloat rotamount = 0.0; // ** NEW ** amount to rotate teapot by
GLint animate = 0; // ** NEW ** whether to animate or not
GLuint vertexshader, fragmentshader, shaderprogram; // shaders
GLuint projectionPos, modelviewPos, colorPos; // Locations of uniform variables
// Uniform variables are used to communicate with your vertex or fragment shader from "outside".
glm::mat4 projection, modelview; // The mvp matrices themselves
glm::mat4 identity(1.0f); // An identity matrix used for making transformation matrices
GLfloat shoulderl = 210, elbowl = -5;
GLfloat shoulderr = 150, elbowr = -65;
GLfloat legl = 150, kneel = 30;
GLfloat legr = 210, kneer = 0;

GLfloat sign = 1;

GLubyte woodtexture[1024][2048][3]; // ** NEW ** texture (from grsites.com)
GLuint texNames[2]; // ** NEW ** texture buffer
GLuint istex;  // ** NEW ** blend parameter for texturing
GLint texturing = 0; // ** NEW ** to turn on/off texturing

GLubyte spheretexture[1024][2048][3]; // ** NEW ** texture heigth * width * channel
GLuint spheretexNames[1]; // ** NEW ** texture buffer

GLuint islight; // ** NEW ** for lighting
GLint lighting = 1; // ** NEW ** to turn on/off lighting

/* Variables to set uniform params for lighting fragment shader */
GLuint light0dirn;
GLuint light0color;
GLuint light1posn;
GLuint light1color;
GLuint light2posn;
GLuint light2color;
GLuint ambient;
GLuint diffuse;
GLuint specular;
GLuint shininess;

#endif //OPENGL_VARIABLE_H
