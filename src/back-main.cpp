
#include <stdlib.h>
#include <glew.h>
#include <glut/glut.h>
// GLM is a C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL)
// specification.
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <FreeImage.h>
#include <iomanip>

float ramount = 2.5;

int mouseoldx, mouseoldy; // For mouse motion
int windowWidth = 500, windowHeight = 500; //Width/Height of OpenGL window
GLdouble eyeloc = 2.0; // Where to look from; initially 0 -2, 2
GLfloat teapotloc = 0; // ** NEW ** where the teapot is located
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

#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QMainWindow>
#include <QMessageBox>

QDialog qDialog;

#include "header/shaders.h"
#include "header/Bmp.h"
#include "header/geometry3.h"

/* New helper transformation function to transform vector by modelview */
void transformvec(const GLfloat input[4], GLfloat output[4]) {
    glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
    glm::vec4 outputvec = modelview * inputvec;
    output[0] = outputvec[0];
    output[1] = outputvec[1];
    output[2] = outputvec[2];
    output[3] = outputvec[3];

}

// Treat this as a destructor function. Delete any dynamically allocated memory here
void deleteBuffers() {
    glDeleteVertexArrays(numobjects + ncolors, VAOs);
    glDeleteVertexArrays(1, &teapotVAO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(numperobj * numobjects + ncolors, buffers);
    glDeleteBuffers(4, spherebuffers);
    glDeleteBuffers(3, cubebuffers);
    glDeleteBuffers(3, cylinderbuffers);
}

void display(void) {
    // clear all pixels

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw white polygon (square) of unit length centered at the origin
    // Note that vertices must generally go counterclockwise
    // Change from the first program, in that I just made it white.
    // The old OpenGL code of using glBegin... glEnd no longer appears.
    // The new version uses vertex buffer objects from init.

    glUniform1i(islight, 1); // Turn off lighting (except on teapot, later)
    glUniform1i(istex, texturing);

    // Draw the floor
    // Start with no modifications made to the model matrix
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    glUniform3f(colorPos, 1.0f, 1.0f, 1.0f); // The floor is white
    drawtexture(FLOOR, texNames[0]); // Texturing floor
    glUniform1i(istex, 0); // Other items aren't textured

    // Now draw several cubes with different transforms, colors
    // We now maintain a stack for the modelview matrices. Changes made to the stack after pushing
    // are discarded once it is popped.
    pushMatrix(modelview);

    // 2nd pillar
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.4, -0.4, 0.1));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[1]); // green
        //drawcolor(CUBE, 1);
        drawcylinder();
        popMatrix(modelview);
    }
    // 3rd pillar
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.4, 0.4, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[2]); // blue
        //drawcolor(CUBE, 2);
        drawcylinder();
        popMatrix(modelview);
    }
    // 4th pillar
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(-0.4, 0.4, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[3]); // yellow
        //drawcolor(CUBE, 3);
        drawcylinder();
        popMatrix(modelview);
    }
    // light source 3 position
    GLfloat light2[4];
    const GLfloat light_specular2[] = {0, 1.0, 0, 1}; // green
    const GLfloat light_position2[] = {0.5, 0.0, 0.0, 1};

    pushMatrix(modelview);
    modelview = modelview * glm::translate(identity, glm::vec3(teapotloc, 0.0, 0.0));
    modelview = modelview * glm::translate(identity, glm::vec3(-0.4, -0.4, 0.0));
    transformvec(light_position2, light2);
    glUniform4fv(light2posn, 1, light2);
    glUniform4fv(light2color, 1, light_specular2);
    popMatrix(modelview);

    // Draw a teapot

    /* New for Demo; add lighting effects */
    {
        const GLfloat one[] = {1, 1, 1, 1}; //rgba
        const GLfloat medium[] = {0.5, 0.5, 0.5, 1}; //rgba
        const GLfloat small[] = {0.2f, 0.2f, 0.2f, 1}; //rgba
        const GLfloat high[] = {500};
        const GLfloat low[] = {0.5};

        const GLfloat zero[] = {0.0, 0.0, 0.0, 1.0}; //rgba
        const GLfloat light_specular[] = {1, 0.5, 0, 1};  // red
        const GLfloat light_direction[] = {0.5, 0, 0, 0}; // Dir light 0 in w
        const GLfloat light_specular1[] = {0, 0.5, 1, 1}; // blue
        const GLfloat light_position1[] = {0, -0.5, 0, 1};


        GLfloat light0[4], light1[4];

        // Set Light and Material properties for the teapot
        // Lights are transformed by current modelview matrix.
        // The shader can't do this globally.
        // So we need to do so manually.
        transformvec(light_direction, light0);
        transformvec(light_position1, light1);


        glUniform3fv(light0dirn, 1, light0);
        glUniform4fv(light0color, 1, light_specular);
        glUniform4fv(light1posn, 1, light1);
        glUniform4fv(light1color, 1, light_specular1);

        // glUniform4fv(light1color, 1, zero) ;

        glUniform4fv(ambient, 1, small);
        glUniform4fv(diffuse, 1, medium);
        glUniform4fv(specular, 1, one);
        glUniform1fv(shininess, 1, high);


        // Enable and Disable everything around the teapot
        // Generally, we would also need to define normals etc.
        // In the old OpenGL code, GLUT defines normals for us. The glut teapot can't
        // be drawn in modern OpenGL, so we need to load a 3D model for it. The normals
        // are defined in the 3D model file.
        glUniform1i(islight, lighting); // turn on lighting only for teapot.

    }
    //  ** NEW ** Put a teapot in the middle that animates
    glUniform3f(colorPos, 0.0f, 1.0f, 1.0f);
    //  ** NEW ** Put a teapot in the middle that animates
    pushMatrix(modelview);
    modelview = modelview * glm::translate(identity, glm::vec3(teapotloc, 0.0, 0.0));

    //  The following two transforms set up and center the teapot
    //  Remember that transforms right-multiply the modelview matrix (top of the stack)
    modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1));
    modelview =
            modelview * glm::rotate(glm::mat4(1.0f), rotamount * glm::pi<float>() / 180.0f, glm::vec3(0.0, 0.0, 1.0));
    modelview = modelview * glm::rotate(identity, glm::pi<float>() / 2.0f, glm::vec3(1.0, 0.0, 0.0));
    float size = 0.235f; // Teapot size
    modelview = modelview * glm::scale(identity, glm::vec3(size, size, size));
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    drawteapot();
    popMatrix(modelview);


    //// robot
    pushMatrix(modelview);
    modelview = modelview * glm::translate(identity, glm::vec3(teapotloc, 0.0, 0.0));
    modelview = modelview * glm::translate(identity, glm::vec3(-0.4, -0.4, 0.0));
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    //glUniform3fv(colorPos, 1, _cubecol[0]); // red
    //drawcolor(CUBE, 0);

    //================================================
    ///* robot head
    {
        //const GLfloat low[] = { 0.1 };

        //glUniform1fv(shininess, 1, low);
        glUniform1i(istex, texturing);

        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 1.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

        //drawsphere();
        drawspheretexture(spheretexNames[0]);
        popMatrix(modelview);
        glUniform1i(istex, 0); // Other items aren't textured
    }

    // 1st pillar
// This function builds a new matrix. It doesn't actually modify the passed in matrix.
// Consequently, we need to assign this result to modelview.
    //pushMatrix(modelview);
    //modelview = modelview * glm::translate(identity, glm::vec3(teapotloc, 0.0, 0.0));
    //modelview = modelview * glm::translate(identity, glm::vec3(-0.4, -0.4, 0.0));
    //glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    //glUniform3fv(colorPos, 1, _cubecol[0]); // red
    //drawcolor(CUBE, 0);

    //================================================
    /* robot head
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 1.0)) ;
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        drawsphere();
        popMatrix(modelview);
    }
    */
    ///* robot body
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.6));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[0]); // red
        //drawcolor(CUBE, 0);
        //solidCube(1.0);
        drawcylinder();
        popMatrix(modelview);
    }
    //*/


    ///* robot right arm
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, -0.2, 0.8));

        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) shoulderr, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.2, 0.2, 0.52));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[1]); // green
        //drawcolor(CUBE, 2);
        drawcylinder();
        popMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.14)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) elbowr, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, -0.02));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.2, 0.2, 0.52));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[3]); // white
        //drawcolor(CUBE, 1);
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
        //*/
    }
    ///* robot left arm
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.15, 0.8));

        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) shoulderl, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.2, 0.2, 0.52));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[1]); // green
        //drawcolor(CUBE, 2);
        drawcylinder();
        popMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.14)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) elbowl, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.2, 0.2, 0.52));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[2]); // blue
        //drawcolor(CUBE, 1);
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
        //*/

        //===================================================
    }

    ///* robot right leg
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0, -0.06, 0.55));

        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) legr, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.4, 0.4, 1.1));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[1]); // green
        //drawcolor(CUBE, 2);
        drawcylinder();
        popMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.3)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) kneer, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, -0.02));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.4, 0.4, 1.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[3]); // white
        //drawcolor(CUBE, 1);
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
        //*/

        //===================================================
    }
    ///* robot left leg
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.06, 0.55));

        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.1)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) legl, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, 0.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.4, 0.4, 1.1));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[1]); // green
        //drawcolor(CUBE, 2);
        drawcylinder();
        popMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.3)) *
                    glm::rotate(identity, glm::pi<float>() / 180.0f * (GLfloat) kneel, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::translate(identity, glm::vec3(0.0, 0.0, -0.02));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        pushMatrix(modelview);
        modelview = modelview * glm::scale(identity, glm::vec3(0.4, 0.4, 1.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[2]); // blue
        //drawcolor(CUBE, 1);
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
        //*/




        //===================================================
    }

    popMatrix(modelview);

    popMatrix(modelview);

    // Does order of drawing matter?
    // What happens if I draw the ground after the pillars?
    // I will show this in class.

    // glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    // drawobject(FLOOR) ;

    // don't wait!
    // start processing buffered OpenGL routines

    glutSwapBuffers();
    glFlush();
}

// ** NEW ** in this assignment, is an animation of a teapot
// Hitting p will pause this animation; see keyboard callback

void animation(void) {
    teapotloc = teapotloc + 0.0025;
    rotamount = rotamount + ramount;
    if (teapotloc > 1.0) teapotloc = -0.5;
    if (rotamount > 360.0) rotamount = 0.0;
    if (shoulderr > 210) {
        sign = -1;
    } else if (shoulderr < 150) {
        sign = 1;
    }
    shoulderr = shoulderr + sign * 0.5;
    elbowr = elbowr + sign * 0.5;
    shoulderl = shoulderl - sign * 0.5;
    elbowl = elbowl - sign * 0.5;
    legr = legr - sign * 0.5;
    kneer = kneer + sign * 0.25;
    legl = legl + sign * 0.5;
    kneel = kneel - sign * 0.25;

    glutPostRedisplay();
}

void moveTeapot() {
    rotamount = 45.0;
    teapotloc = -0.05;
}

// Defines a Mouse callback to zoom in and out
// This is done by modifying gluLookAt
// The actual motion is in mousedrag
// mouse simply sets state for mousedrag
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            // Do Nothing ;
        } else if (state == GLUT_DOWN) {
            mouseoldx = x;
            mouseoldy = y; // so we can move wrt x , y
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { // Reset gluLookAt
        eyeloc = 2.0;
        modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
        // Send the updated matrix to the shader
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glutPostRedisplay();
    }
}

void mousedrag(int x, int y) {
    int yloc = y - mouseoldy;    // We will use the y coord to zoom in/out
    eyeloc += 0.005 * yloc;         // Where do we look from
    if (eyeloc < 0) eyeloc = 0.0;
    mouseoldy = y;

    /* Set the eye location */
    modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
    // Send the updated matrix to the shader
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
    glutPostRedisplay();
}

void printHelp() {
    std::cout << "\nAvailable commands:\n"
              << "press 'h' to print this message again.\n"
              << "press Esc to quit.\n"
              << "press 'o' to save a screenshot to \"./screenshot.png\".\n"
              << "press 'i' to move teapot into position for Assign0 screenshot.\n"
              << "press 'p' to start/stop teapot animation.\n"
              << "press 't' to turn texturing on/off.\n"
              << "press 's' to turn shading on/off.\n";
}

void saveScreenshot() {
    int pix = windowWidth * windowHeight;
    BYTE *pixels = new BYTE[3 * pix];
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, windowWidth * 3, 24, 0xFF0000,
                                                 0x00FF00, 0x0000FF, false);

    std::cout << "Saving screenshot: screenshot.png\n";

    FreeImage_Save(FIF_PNG, img, "screenshot.png", 0);
    delete pixels;
}

// Defines what to do when various keys are pressed
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':
            printHelp();
            break;
        case 'o':
            saveScreenshot();
            break;
        case 'i':
            moveTeapot();
            eyeloc = 2.0f;
            // Immediately update the modelview matrix
            modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
            // Send the updated matrix to the shader
            glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
            texturing = 0;
            lighting = 1;
            animate = 0;
            glutIdleFunc(NULL);
            glutPostRedisplay();
            break;
        case 27:  // Escape to quit
            exit(0);
            break;
        case 'p': // ** NEW ** to pause/restart animation
            animate = !animate;
            if (animate) glutIdleFunc(animation);
            else glutIdleFunc(NULL);
            break;
        case 't': // ** NEW ** to turn on/off texturing ;
            texturing = !texturing;
            glutPostRedisplay();
            break;
        case 'l': // ** NEW ** to turn on/off shading (always smooth) ;
            lighting = !lighting;
            glutPostRedisplay();
            break;
        case 's':
            //shoulder = (shoulder + 5) % 360;
            glutPostRedisplay();
            break;
        case 'S':
            //shoulder = (shoulder - 5) % 360;
            glutPostRedisplay();
            break;
        case 'e':
            //elbow = (elbow + 5) % 360;
            glutPostRedisplay();
            break;
        case 'E':
            //elbow = (elbow - 5) % 360;
            glutPostRedisplay();
            break;
        case 'a':
            QMessageBox::information(NULL, "title", "comment");
            printf("ramount: %.2f\n", ramount);
            printf("input ramount\n");
            scanf("%f", &ramount);
            printf("ramount: %.2f\n", ramount);
            break;
        default:
            break;
    }
}

/* Reshapes the window appropriately */
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // Think about the rationale for this choice for glm::perspective
    // What would happen if you changed near and far planes?
    projection = glm::perspective(30.0f / 180.0f * glm::pi<float>(), (GLfloat) w / (GLfloat) h, 1.0f, 10.0f);
    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
}

void checkOpenGLVersion() {
    const char *version_p = (const char *) glGetString(GL_VERSION);
    float version = 0.0f;

    if (version_p != NULL)
        version = atof(version_p);

    if (version < 3.1f) {
        std::cout << std::endl << "*****************************************" << std::endl;

        if (version_p != NULL) {
            std::cout << "WARNING: Your OpenGL version is not supported." << std::endl;
            std::cout << "We detected version " << std::fixed << std::setprecision(1) << version;
            std::cout << ", but at least version 3.1 is required." << std::endl << std::endl;
        } else {
            std::cout << "WARNING: Your OpenGL version could not be detected." << std::endl << std::endl;
        }

        std::cout << "Please update your graphics drivers BEFORE posting on the forum. If this" << std::endl
                  << "doesn't work, ensure your GPU supports OpenGL 3.1 or greater." << std::endl;

        std::cout << "If you receive a 0xC0000005: Access Violation error, this is likely the reason." << std::endl;

        std::cout << std::endl;

        std::cout << "Additional OpenGL Info:" << std::endl;
        std::cout << "(Please include with support requests)" << std::endl;
        std::cout << "GL_VERSION: ";
        std::cout << glGetString(GL_VERSION) << std::endl;
        std::cout << "GL_VENDOR: ";
        std::cout << glGetString(GL_VENDOR) << std::endl;
        std::cout << "GL_RENDERER: ";
        std::cout << glGetString(GL_RENDERER) << std::endl;

        std::cout << std::endl << "*****************************************" << std::endl;
        std::cout << std::endl << "Select terminal and press <ENTER> to continue." << std::endl;
        std::cin.get();
        std::cout << "Select OpenGL window to use commands below." << std::endl;
    }
}

void init(void) {
    //Warn students about OpenGL version before 0xC0000005 error
    checkOpenGLVersion();

    printHelp();

    /* select clearing color 	*/
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* initialize viewing values  */
    projection = glm::mat4(1.0f); // The identity matrix
    modelview = glm::lookAt(glm::vec3(0, -eyeloc, eyeloc), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));


    // Initialize the shaders

    vertexshader = initshaders(GL_VERTEX_SHADER, "../shaders/light.vert.glsl");
    fragmentshader = initshaders(GL_FRAGMENT_SHADER, "../shaders/light.frag.glsl");
    GLuint program = glCreateProgram();
    shaderprogram = initprogram(vertexshader, fragmentshader);
    GLint linked;
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &linked);

    // * NEW * Set up the shader parameter mappings properly for lighting.
    islight = glGetUniformLocation(shaderprogram, "islight");
    //  glGetUniformLocation - return the location of a uniform variable
    light0dirn = glGetUniformLocation(shaderprogram, "light0dirn");
    light0color = glGetUniformLocation(shaderprogram, "light0color");
    light1posn = glGetUniformLocation(shaderprogram, "light1posn");
    light1color = glGetUniformLocation(shaderprogram, "light1color");
    light2posn = glGetUniformLocation(shaderprogram, "light2posn");
    light2color = glGetUniformLocation(shaderprogram, "light2color");
    ambient = glGetUniformLocation(shaderprogram, "ambient");
    diffuse = glGetUniformLocation(shaderprogram, "diffuse");
    specular = glGetUniformLocation(shaderprogram, "specular");
    shininess = glGetUniformLocation(shaderprogram, "shininess");

    // Get the positions of other uniform variables
    projectionPos = glGetUniformLocation(shaderprogram, "projection");
    modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
    colorPos = glGetUniformLocation(shaderprogram, "color");

    // Now create the buffer objects to be used in the scene later
    glGenVertexArrays(numobjects + ncolors, VAOs);
    glGenVertexArrays(1, &teapotVAO);
    glGenVertexArrays(1, &sphereVAO);
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(numperobj * numobjects + ncolors + 1, buffers); // 1 extra buffer for the texcoords
    glGenBuffers(3, teapotbuffers);
    glGenBuffers(4, spherebuffers);
    glGenBuffers(3, cubebuffers);
    glGenBuffers(3, cylinderbuffers);
    // Initialize texture
    inittexture("../resource/earth2048.bmp", shaderprogram);
    // load BMP image
    initspheretexture("../resource/nezha.bmp", shaderprogram);
    //initspheretexture("resource/wood.ppm", shaderprogram);

    // Initialize objects
    initobject(FLOOR, (GLfloat *) floorverts, sizeof(floorverts), (GLfloat *) floorcol, sizeof(floorcol),
               (GLubyte *) floorinds, sizeof(floorinds), GL_TRIANGLES);
    initcubes(CUBE, (GLfloat *) cubeverts, sizeof(cubeverts), (GLubyte *) cubeinds, sizeof(cubeinds), GL_TRIANGLES);
    loadteapot();
    //initsphere();
    initcylinder();
    //initCube();

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // The default option
}


int main(int argc, char **argv) {

    FreeImage_Initialise(); // FreeImage is an Open Source library project for developers who would
    //like to support popular graphics image formats like PNG, BMP, JPEG, TIFF and others as needed
    //by today's multimedia applications.

    glutInit(&argc, argv); // GLUT (OpenGL Utility Toolkit, now freeglut) takes care of all the
    //system-specific chores required for creating windows, initializing OpenGL contexts, and
    //handling input events, to allow for trully portable OpenGL programs.

    // Requests the type of buffers (Single, RGB).
    // Think about what buffers you would need...

    // Request the depth if needed, later swith to double buffer

    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Simple Demo with Shaders");

    GLenum err = glewInit(); //The OpenGL Extension Wrangler Library (GLEW) is
    //a cross-platform open-source C/C++ extension loading library.
    //GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported
    //on the target platform.
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetString(err) << std::endl;
    }

    init(); // Always initialize first

    // Now, we define callbacks and functions for various tasks.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mousedrag);
    glutMainLoop(); // Start the main code
    FreeImage_DeInitialise();
    deleteBuffers();
    return 0;   /* ANSI C requires main to return int. */
}
