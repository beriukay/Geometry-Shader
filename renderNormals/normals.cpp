/*
 * Authors: Bucky Frost
 * CS 381 Project
 * File: normals.cpp
 * Purpose: OpenGL app for using GS to render normals
 */

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
#include "lib381/globj.h"
                         // For class VBO
#include "lib381/glslprog.h"
			 // For GLSL code-handling functions*/
#include "includes/glslprog.h"
#include "includes/bitmapprinter.h"
#include "includes/globj.h"

#include <iostream>
using std::cerr;
using std::endl;
#include <cmath>
using std::sqrt;
using std::cos;
using std::sin;
#include <iomanip>
using std::setprecision;
using std::fixed;
#include <sstream>
using std::ostringstream;
#include <string>
using std::string;

// Global variables
// General
const double pi = 3.1415926535897932384626433832795;

// Keyboard
const int ESCKEY = 27;         // ASCII value of Escape

// Window/viewport
const int startwinsize = 600;  // Start window width & height (pixels)
int winw = 1, winh = 1;        // Window width, height (pixels)
                               //  (Initialize to avoid spurious errors)
double cam_xmin, cam_xmax, cam_ymin, cam_ymax;
                               // Viewport bdaries in world (cam coords)

// Mouse
double cam_mousex, cam_mousey; // Mouse pos in cam coords
int draggedobj;                // 0: not currently dragging
                               // 1: dragging object 1
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

/*Buffer objects!*/
VBO vbo1, vbo2, vbo3;          // Vertex-Buffer Objects
int whichobject;               // Which object to draw


// drawObject
// Draws appropriate object at the origin.
// Uses previously initialized VBOs.
void drawObject()
{
    // Bind the appropriate VBO
    switch (whichobject)
    {
    case 1:
        vbo1.bind();
        break;
    case 2:
        vbo2.bind();
        break;
    case 3:
        vbo3.bind();
        break;
    }

    // Describe vertex-array data
    glVertexPointer(2, GL_DOUBLE, 2*sizeof(GLdouble), (GLvoid *)(0));
    // Parameters: components per vertex, type of component,
    //  stride (bytes), ptr to start of vertex data.

    // Enable vertex-array functionality
    glEnableClientState(GL_VERTEX_ARRAY);

    // Draw using vertex array
    // Parameters for glDrawArrays:
    //  primitive, index of first vertex, number of vertices.
    switch (whichobject)
    {
    case 1:
        glDrawArrays(GL_POLYGON, 0, 4);
        break;
    case 2:
        glDrawArrays(GL_POLYGON, 0, 3);
        break;
    case 3:
        glDrawArrays(GL_POLYGON, 0, 4);
        break;
    }
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

    //Shaders used? Wireframe?
    switch (shade)
    {
        case 0:  // 0: filled polygons, use shaders, smooth
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            theprog = prog1;
            break;
        case 1:  // 1: filled polygons, no shader
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            theprog = prog1;
            break;
        case 2:  // 2: wireframe, no shader
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            theprog = prog1;
            break;
    }

// Set up 3-D
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glTranslated(0., 0., -9.);
glRotated(rotangle, 1.,2.,1.);
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
    if (theprog)
    {
       GLint loc;  // Location for shader vars

        loc = glGetUniformLocationARB(theprog, "myb1");
        if (loc != -1)
        {
            glUniform1i(loc, shaderbool1);
        }

        loc = glGetUniformLocationARB(theprog, "myf1");
        if (loc != -1)
        {
            glUniform1f(loc, GLfloat(shaderfloat1));
        }
    }

    // Draw the appropriate object
    glRotated(rotangle, 1.,2.,1.);
    
    glColor3d(0.8, 0.4, 0.6);
    drawBezierPatch(numsubdivs);

    glBegin(GL_TRIANGLES);
	glVertex3d(0,1,1);
	glVertex3d(0,-1,-1);
	glVertex3d(0,2,0);
    glEnd();

/*    // Set up 3-D
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glColor3d(0.1, 0.1, 0.9);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < circle_segs; ++i)
        {
            double x = spinner_radius * cos(i * 2.*pi/circle_segs);
            double y = spinner_radius * sin(i * 2.*pi/circle_segs);
            glVertex2d(x, y);
        }
    glEnd();    

    // Draw object
    glLoadIdentity();
    glTranslated(s1posx, s1posy, 0.);
    glScaled(s1halfside, s1halfside, s1halfside);
    glColor3d((draggedobj == 1) ? 1.0 : 0.0, 0.3, 0.3);
    drawObject();
*/    

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
    glColor3d(0., 0., 0.);        // Black text
    BitmapPrinter p(20., winh-20., 20.);
    p.print("Try clicking & dragging the shape");
    p.print("1 2 3    Change object");
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

/*    // Set up projection
    // Save max/min x/y coords in globals
    // Projection is orthographic. Aspect ratio is correct,
    // and region -1..1 in x, y always just fits in viewport.
    if (w > h)
    {
        cam_xmin = -double(w)/h;
        cam_xmax = double(w)/h;
        cam_ymin = -1.;
        cam_ymax = 1.;
    }
    else
    {
        cam_xmin = -1.;
        cam_xmax = 1.;
        cam_ymin = -double(h)/w;
        cam_ymax = double(h)/w;
    }
*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60., double(w)/h, 0.5, 100.);

    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode

    // Move our object into the window, if it is outside
//    fixObject1();
}


// saveMouse
// Given mouse pos in GLUT format; computes mouse pos in cam coords,
//  saves it in globals cam_mousex, cam_mousey.
// Uses globals winw, winh, cam_xmin, cam_xmax, cam_ymin, cam_ymax.
void saveMouse(int x, int y)
{
    double t;  // Intermediate value of lirping

    t = double(x) / winw;
    cam_mousex = cam_xmin + t * (cam_xmax - cam_xmin);

    t = double(y) / winh;
    cam_mousey = cam_ymax + t * (cam_ymin - cam_ymax);
}

/*
// myMouse
// The GLUT mouse function
void myMouse(int button, int state, int x, int y)
{
    // Save old mouse pos, for relative mouse-movement computation
    double old_cam_mousex = cam_mousex;
    double old_cam_mousey = cam_mousey;

    // Find mouse pos in cam coords (saved in cam_mousex, cam_mousey)
    saveMouse(x, y);

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)  // Left mouse down
        {
            // Extent test for object 1
            if (cam_mousex >= s1posx-s1halfside
             && cam_mousex <= s1posx+s1halfside
             && cam_mousey >= s1posy-s1halfside
             && cam_mousey <= s1posy+s1halfside)
            {
                // We have a mouse-down on object 1
                draggedobj = 1;
                s1dragposx = s1posx;
                s1dragposy = s1posy;
                glutPostRedisplay();
            }
        }
        else // Left mouse up
        {
            if (draggedobj == 1)  // We were dragging object 1
            {
                // Move object 1 by relative mouse movement
                s1dragposx += (cam_mousex - old_cam_mousex);
                s1dragposy += (cam_mousey - old_cam_mousey);
                s1posx = s1dragposx;
                s1posy = s1dragposy;
                fixObject1();
                glutPostRedisplay();
            }
            draggedobj = 0;
        }
    }
}
*/
/*
// myMotion
// The GLUT motion function
void myMotion(int x, int y)
{
    // Save old mouse pos, for relative mouse-movement computation
    double old_cam_mousex = cam_mousex;
    double old_cam_mousey = cam_mousey;

    // Find mouse pos in cam coords (saved in cam_mousex, cam_mousey)
    saveMouse(x, y);

    if (draggedobj == 1)  // We are dragging object 1
    {
        // Move object 1 by relative mouse movement
        s1dragposx += (cam_mousex - old_cam_mousex);
        s1dragposy += (cam_mousey - old_cam_mousey);
        // To get the bad "snapping" effect, comment-out the previous
        //  two lines and uncomment the following.
        //s1dragposx = cam_mousex;
        //s1dragposy = cam_mousey;
        s1posx = s1dragposx;
        s1posy = s1dragposy;
        fixObject1();
        glutPostRedisplay();
    }
}
*/

// init
// Initialize GL states & global data
// Called by main after window creation
void init()
{
/*    // Mouse
    draggedobj = 0;
    cam_mousex = 0.0;
    cam_mousey = 0.0;

    // Objects
    s1posx = 0.0;
    s1posy = 0.0;*/
    whichobject = 1;

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

    // Vertex Buffer Objects

    // Vertex-array data: 4 vertices, each with 2 components (x, y)
    GLdouble vdata1[2*4] = {
        -1., -1.,
         1., -1.,
         1.,  1.,
        -1.,  1.
    };

    // Generate & bind VBO
    vbo1.bind();

    // Send data to server
    glBufferData(GL_ARRAY_BUFFER,       // Target
                 2*4*sizeof(GLdouble),  // Size (bytes)
                 vdata1,                // Ptr
                 GL_STATIC_DRAW);       // Usage hint

    // Vertex-array data: 3 vertices, each with 2 components (x, y)
    GLdouble vdata2[2*3] = {
        -1., -0.577,
         1., -0.577,
         0.,  1.155
    };

    // Generate & bind VBO
    vbo2.bind();

    // Send data to server
    glBufferData(GL_ARRAY_BUFFER,       // Target
                 2*3*sizeof(GLdouble),  // Size (bytes)
                 vdata2,                // Ptr
                 GL_STATIC_DRAW);       // Usage hint

    // Vertex-array data: 3 vertices, each with 2 components (x, y)
    GLdouble vdata3[2*4] = {
         0.,    -1.414,
         1.414,  0.,
         0.,     1.414,
        -1.414,  0.
    };

    // Generate & bind VBO
    vbo3.bind();

    // Send data to server
    glBufferData(GL_ARRAY_BUFFER,       // Target
                 2*4*sizeof(GLdouble),  // Size (bytes)
                 vdata3,                // Ptr
                 GL_STATIC_DRAW);       // Usage hint

    // OpenGL Stuff
    // (Nothing here)
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
    glutCreateWindow("CS 381 - Spinner & VBO Demo");

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
//    glutMouseFunc(myMouse);
//    glutMotionFunc(myMotion);

    // Do something
    glutMainLoop();

    return 0;
}

