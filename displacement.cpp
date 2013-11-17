/* Bucky Frost
* Paul Gentemann
* CS 381
* File Name : splinepatch.cpp
* Last Modified : Mon 11 Nov 2013 02:41:22 PM AKST
* Description : A pair of spline plains, patched together, that will
* make a wave upon user input.
*/

// OpenGL/GLUT includes - DO THESE FIRST
#include <cstdlib> // Do this before GL/GLUT includes
using std::exit;
#ifndef __APPLE__
# include <GL/glew.h>
# include <GL/glut.h> // Includes OpenGL headers as well
#else
# include <GLEW/glew.h>
# include <GLUT/glut.h> // Apple puts glut.h in a different place
#endif
#ifdef _MSC_VER // Tell MS-Visual Studio about GLEW lib
# pragma comment(lib, "glew32.lib")
#endif

#include "includes/glslprog.h" //glsl functionality

#include <string>
using std::string;
#include <iostream>
using std::cerr; using std::endl;

// Global Variables
const int ESCKEY = 27;		 // ASCII value of Escape
const int startwinsize = 600;	 // Start window width & height (pixels)
int winw = 1, winh = 1;		 // Window width, height (pixels)

// Shader Related Variables
bool shaderbool1;
string vshader1fname; // Filename for vertex shader source
string gshader1fname; // Filename for geometry shader source
string fshader1fname; // Filename for fragment shader source
GLhandleARB prog1; // GLSL Program Object
GLfloat shaderfloat1;

// myDisplay
// The GLUT display function
void myDisplay()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //bg colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLhandleARB theprog; // CURRENTLY-used program object or 0 if none
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Activate shaders
    theprog = prog1;

    glEnable(GL_DEPTH_TEST); // Set up 3D
    glLoadIdentity(); // Start with camera.

    // Position light source 0 & draw ball there
    glPushMatrix();
        glTranslated(-1., 1., 2.); // Starting left, up, behind camera
        GLfloat origin[] = { 0.f, 0.f, 0.f, 1.f };
        glLightfv(GL_LIGHT0, GL_POSITION, origin);
        glUseProgramObjectARB(0);
        glColor3d(1., 1., 1.); // white ball
        glutSolidSphere(.5, 20, 15); // obj for light source
    glPopMatrix();

    glUseProgramObjectARB(theprog);

    //Send info to shader
    if (theprog)
    {
        GLint loc; // Location for shader vars
        loc = glGetUniformLocationARB(theprog, "myb1");
        if (loc != -1)
        {
            glUniform1i(loc, shaderbool1);
        }
        loc = glGetUniformLocationARB(theprog, "myf1");
        if (loc != -1)
        {
            glUniform1f(loc, shaderfloat1);
        }
    }

    // Draw Objects
    glBegin(GL_TRIANGLES);
       glVertex3d(0.,0.,0.);
       glVertex3d(1.,0.,0.);
       glVertex3d(0.,1.,0.);
    glEnd();

    glutSwapBuffers();
}


// myIdle
// The GLUT idle function
void myIdle()
{
    glutPostRedisplay();
    // Print OpenGL errors, if there are any (for debugging)
    static int error_count = 0;
    if (GLenum err = glGetError())
    {
        ++error_count;
        cerr << "OpenGL ERROR " << error_count << ": "
             << gluErrorString(err) << endl;
    }
}

// myKeyboard
// The GLUT keyboard function
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ESCKEY: // Esc: quit
        exit(0);
        break;
    }
}

// myReshape
// The GLUT reshape function
void myReshape(int w, int h)
{
    // Set viewport & save window dimensions in globals
    glViewport(0, 0, w, h);
    winw = w;
    winh = h;
    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60., double(w)/h, 0.01, 100.);
    glMatrixMode(GL_MODELVIEW); // Always go back to model/view mode
}

// init
// Initialize GL states & global data
void init()
{
    glMatrixMode(GL_MODELVIEW);
    
    // Shaders
    prog1 = makeProgramObjectFromFiles(vshader1fname, gshader1fname, fshader1fname);
}


// The main
int main(int argc, char ** argv)
{
    // Initilization of OpenGL/GLUT
    glutInit(&argc, argv);
    // Set shader source filenames. Done here, as opposed to init() so we can
    // use command line arguments.
    getShaderFilenames(vshader1fname, gshader1fname, fshader1fname, argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Creating the view window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 - Displacement Goodness!");

    // Init GLEW & check status - exit on failure
    if (glewInit() != GLEW_OK)
    {
        cerr << "glewInit failed" << endl;
        exit(1);
    }

    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);

    // Start GLUT event handling loop
    glutMainLoop();

    return 0;
}
