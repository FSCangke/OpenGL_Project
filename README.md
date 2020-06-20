# OpenGL_Project
This is the demo of 3d real time animation, develop on MacOS.
## 1. Functions
Available commands:  
press 'h' to print this message again.  
press Esc to quit.  
press 'o' to save a screenshot to "../resource/screenshot.png\".  
press 'i' to move teapot and robot to initial position.  
press 'I' to move teapot and robot to initial position, and reset all variables.  
press 'p' to start/stop animation.  
press 't' to turn texturing on/off.  
press 'a', 'b', 'c', 'd' to set teapot location, robot movement, robot location and robot movement limit. 
press 's' to turn shading on/off.  
## 2. Code
Here is code.

+ Display
```
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
    modelview = modelview * glm::translate(identity, glm::vec3(teapotlocX, teapotlocY, teapotlocZ));
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
    modelview = modelview * glm::translate(identity, glm::vec3(teapotlocX, teapotlocY, teapotlocZ));

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

    // robot
    pushMatrix(modelview);
    modelview = modelview * glm::translate(identity, glm::vec3(robotlocX, robotlocY, robotlocZ));
    modelview = modelview * glm::translate(identity, glm::vec3(-0.4, -0.4, 0.0));
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

    // robot head
    {
        glUniform1i(istex, texturing);

        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 1.0));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);

        drawspheretexture(spheretexNames[0]);
        popMatrix(modelview);
        glUniform1i(istex, 0); // Other items aren't textured
    }

    // robot body
    {
        pushMatrix(modelview);
        modelview = modelview * glm::translate(identity, glm::vec3(0.0, 0.0, 0.6));
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]);
        glUniform3fv(colorPos, 1, _cubecol[0]); // red
        drawcylinder();
        popMatrix(modelview);
    }

    // robot right arm
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
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
    }
    // robot left arm
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
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
    }

    // robot right leg
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
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
    }

    // robot left leg
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
        drawcylinder();
        popMatrix(modelview);
        popMatrix(modelview);
    }

    popMatrix(modelview);

    popMatrix(modelview);

    // don't wait!
    // start processing buffered OpenGL routines

    glutSwapBuffers();
    glFlush();
}
```
+ Keyboard
```
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':
            printHelp();
            break;
        case 'o':
            saveScreenshot();
            break;
        case 'i':
            initloc();
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
        case 'I':
            initall();
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
        case 'a':
            setteapotloc();
            break;
        case 'b':
            setrobotmove();
            break;
        case 'c':
            setrobotloc();
            break;
        case 'd':
            setrobotlimit();
            break;
        default:
            break;
    }
}
```
+ anime
```
void anime(int argc, char **argv) {

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
}
```