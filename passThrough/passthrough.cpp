// bezierpatch.cpp
// VERSION 3
// Glenn G. Chappell
// 28 Oct 2013
//
// For CS 381 Fall 2013
// Draw Bezier Patch
//
// History:
// - v2: reversed front-facing determination for patch
// - v3: added spot direction

// OpenGL/GLUT includes - DO THESE FIRST
#include <cstdlib>       // Do this before GL/GLUT includes
using std::exit;
#ifndef __APPLE__
# include <GL/glew.h>
# include <GL/glut.h>    // Includes OpenGL headers as well
#else
# include <GLEW/glew.h>
# include <GLUT/glut.h>  // Apple puts glut.h in a different place
#endif
#ifdef _MSC_VER          // Tell MS-Visual Studio about GLEW lib
# pragma comment(lib, "glew32.lib")
#endif

/*// Other includes
#include "lib381/bitmapprinter.h"
                         // For class BitmapPrinter
#include "lib381/glslprog.h"
                         // For GLSL code-handling functions*/
#include "includes/glslprog.h"
#include "includes/bitmapprinter.h"
#include <cmath>
using std::cos;
using std::sin;
#include <iomanip>
using std::setprecision;
using std::fixed;
#include <sstream>
using std::ostringstream;
#include <string>
using std::string;
#include <iostream>
using std::cerr;
using std::endl;


// Global variables
// Keyboard
const int ESCKEY = 27;         // ASCII value of Escape

// Window/viewport
const int startwinsize = 600;  // Start window width & height (pixels)
int winw = 1, winh = 1;        // Window width, height (pixels)
                               //  (Initialize to avoid spurious errors)

// Objects
double savetime;               // Time of previous movement (sec)
double rotangle;               // Rotation angle for object (deg)
const double rotspeed = 40.;   // Rotation speed (deg/sec)
bool rotateflag;               // True if object rotating
double lightrotang;            // Angle for light source 0 move (deg)
const double lightrotspeed = 50.;
                               // Speed for above (deg/sec)
bool lightrotflag;             // True if light source is moving
int numsubdivs;                // Number of subdivisions for object
const int minsubdivs = 1;      //  Minumum of above
const int maxsubdivs = 50;     //  Maximum of above
int shade;                     // 0: shader, 1: no shader
                               //  2: wireframe-no shader
bool grayback;                 // False: black background; true: gray

// Shaders
string vshader1fname;          // Filename for vertex shader source
string gshader1fname;
string fshader1fname;          // Filename for fragment shader source
GLhandleARB prog1;             // GLSL Program Object

bool shaderbool1;              // Boolean to send to shaders
double shaderfloat1;           // Float to send to shaders


void drawBezierPatch(int subdivs)
{
    GLdouble cpts[48] = {
         1.5, -1.5, 0.,
         1.5, -0.5,-3.,
         1.5,  0.5,-2.,
         1.5,  1.5, 1.,
         0.5, -1.5, 0.,
         0.5, -0.5, 0.,
         0.5,  0.5, 0.,
         0.5,  1.5, 0.,
        -0.5, -1.5, 0.,
        -0.5, -0.5, 1.,
        -0.5,  0.5,-3.,
        -0.5,  1.5, 1.,
        -1.5, -1.5, 1.,
        -1.5, -0.5,-2.,
        -1.5,  0.5, 1.,
        -1.5,  1.5, 0.
    };

    glMap2d(GL_MAP2_VERTEX_3,
            0., 1.,
            3,
            4,
            0., 1.,
            3*4,
            4,
            cpts);
    glEnable(GL_MAP2_VERTEX_3);

    glMapGrid2d(subdivs, 0., 1., subdivs, 0., 1.);
    glEnable(GL_AUTO_NORMAL);

    glFrontFace(GL_CW);  // Normals are evidently backwards here :-(
    glEvalMesh2(GL_FILL, 0, subdivs, 0, subdivs);
    glFrontFace(GL_CCW);
}


// myDisplay
// The GLUT display function
void myDisplay()
{
    if (grayback)
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLhandleARB theprog;  // CURRENTLY-used program object or 0 if none

    // Shaders used? Wireframe?
  //  switch (shade)
   // {
    //    case 0:  // 0: filled polygons, use shaders, smooth
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            theprog = prog1;
     //       break;
      //  case 1:  // 1: filled polygons, no shader
      //      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      //      theprog = prog1;
      //      break;
      //  case 2:  // 2: wireframe, no shader
       //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //    theprog = prog1;
         //   break;
   // }

    // Set up 3-D
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glTranslated(0., 0., -9.);

    // Position light source 0 & draw ball there
    glPushMatrix();
    glTranslated(0.0, 0.0, 1.0);
    glRotated(lightrotang, 1.,0.,0.);
    glTranslated(-1., 1., 1.);
    GLfloat origin4[] = { 0.f, 0.f, 0.f, 1.f };
    glLightfv(GL_LIGHT0, GL_POSITION, origin4);
    GLfloat spotdir3[] = { 1.f, -1.f, -1.f };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir3);
    glUseProgramObjectARB(0);
    glColor3d(1., 1., 1.);
    glutSolidSphere(0.1, 20, 15);
    glPopMatrix();

    // Make program object (if any) active
    glUseProgramObjectARB(theprog);

    // Send values to shaders
    //if (theprog)
    //{
     //   GLint loc;  // Location for shader vars

    //    loc = glGetUniformLocationARB(theprog, "myb1");
    //    if (loc != -1)
    //    {
    //        glUniform1i(loc, shaderbool1);
    ///    }

   //     loc = glGetUniformLocationARB(theprog, "myf1");
     //   if (loc != -1)
       // {
       //     glUniform1f(loc, GLfloat(shaderfloat1));
       // }
   // }

    // Draw the appropriate object
    glRotated(rotangle, 1.,2.,1.);
    glColor3d(0.8, 0.4, 0.6);
    //drawBezierPatch(numsubdivs);
	glBegin(GL_TRIANGLES);
		glVertex3d(0,1,1);
		glVertex3d(0,-1,-1);
		glVertex3d(0,2,0);
	glEnd();



    // Reset polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    

    // Draw documentation
    glUseProgramObjectARB(0);     // No shaders
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0., double(winw), 0., double(winh));
    if (grayback)  // Draw text in a contrasting color
        glColor3d(0., 0., 0.);
    else
        glColor3d(0.7, 0.7, 0.7);
    BitmapPrinter p(20., winh-20., 20.);
    ostringstream os;
    os << numsubdivs;
    p.print("<- ->    Change # of subdivisions (" + os.str() + ")");
    string shstrs[3] = {
        "SHADER/no shader/wireframe",
        "shader/NO SHADER/wireframe",
        "shader/no shader/WIREFRAME"
    };
    p.print("W        " + shstrs[shade]);
    p.print("R        Toggle object rotation");
    p.print("L        Toggle light-source movement");
    p.print(string("Space    Toggle shader boolean (") +
        (shaderbool1 ? "true" : "false") + ")");
    ostringstream os2;
    os2 << fixed << setprecision(2) << shaderfloat1;
    p.print("[ ]      Change shader float (" + os2.str() + ")");
    p.print("B        Change background color");
    p.print("Esc      Quit");
    glPopMatrix();                // Restore prev projection
    glMatrixMode(GL_MODELVIEW);


    glutSwapBuffers();
}


// myIdle
// The GLUT idle function
void myIdle()
{
    // Compute elapsed time since last movement
    double currtime = glutGet(GLUT_ELAPSED_TIME) / 1000.;
    double elapsedtime = currtime - savetime;
    savetime = currtime;
    if (elapsedtime > 0.1)
        elapsedtime = 0.1;

    // Move objects

    // Rotate main object
    if (rotateflag)
    {
        rotangle += rotspeed * elapsedtime;
        glutPostRedisplay();
    }

    // Move light source
    if (lightrotflag)
    {
        lightrotang += lightrotspeed * elapsedtime;
        glutPostRedisplay();
    }

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
    case ESCKEY:  // Esc: quit
        exit(0);
        break;
    case 'w':     // W: cycle shader use & wireframe
        shade = (shade+1)%3;
        glutPostRedisplay();
        break;
    case 'W':
        shade = (shade+3-1)%3;
        glutPostRedisplay();
        break;
    case 'r':     // R: toggle object rotation
    case 'R':
        rotateflag = !rotateflag;
        break;
    case 'l':     // L: toggle light-source movement
    case 'L':
        lightrotflag = !lightrotflag;
        break;
    case 'b':     // 'B': Change background color
    case 'B':
        grayback = !grayback;
        glutPostRedisplay();
        break;
    case ' ':     // Space: toggle shader bool
        shaderbool1 = !shaderbool1;
        glutPostRedisplay();
        break;
    case '[':     // [: Decrease shader float
    case '{':
        shaderfloat1 -= 0.02;
        if (shaderfloat1 < 0.)
            shaderfloat1 = 0.;
        glutPostRedisplay();
        break;
    case ']':     // ]: Increase shader float
    case '}':
        shaderfloat1 += 0.02;
        if (shaderfloat1 > 1.)
            shaderfloat1 = 1.;
        glutPostRedisplay();
        break;
    }
}


// mySpecial
// The GLUT special function
void mySpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:  // ->: increase subdivisions
        ++numsubdivs;
        if (numsubdivs > maxsubdivs)
            numsubdivs = maxsubdivs;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:   // <-: decrease subdivisions
        --numsubdivs;
        if (numsubdivs < minsubdivs)
            numsubdivs = minsubdivs;
        glutPostRedisplay();
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
    gluPerspective(60., double(w)/h, 0.5, 100.);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
    // Objects
    savetime = glutGet(GLUT_ELAPSED_TIME)/1000.;
    numsubdivs = 20;
    shade = 0;
    rotangle = 0.;
    rotateflag = false;
    lightrotang = 0.;
    lightrotflag = false;
    grayback = true;

    shaderbool1 = true;
    shaderfloat1 = 1.;

    // OpenGL Stuff

    // Shaders
    prog1 = makeProgramObjectFromFiles(vshader1fname, gshader1fname, fshader1fname);
}


int main(int argc, char ** argv)
{
    // Initialize OpenGL/GLUT
    glutInit(&argc, argv);
    getShaderFilenames(vshader1fname, gshader1fname, fshader1fname, argc, argv);
        // Set shader source filenames. Done here, as opposed to in
        //  function init, so that we can use command-line arguments.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Make a window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 - Use Various Shaders");

    // Init GLEW & check status
    if (glewInit() != GLEW_OK)
    {
        cerr << "glewInit failed" << endl;
        exit(1);
    }

    // Initialize GL states & register GLUT callbacks
    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);
    glutReshapeFunc(myReshape);

    // Do something
    glutMainLoop();

    return 0;
}

