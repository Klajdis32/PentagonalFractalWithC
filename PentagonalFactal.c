#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int mouseX, mouseY; // variables to store the current position of the mouse
int popupMenu; // variable to store the ID of the popup menu
int numIterations = 15000; // declare numIterations as a global variable and initialize it to 15000
int colorMode=0; // variable to store the current color mode (0 = single color, 1 = random color)
int numVertices=5; // variable to store the number of vertices of the polygon (5 or 6)

// function to handle mouse events
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x;
        mouseY = y;
    }
}

// function to handle the popup menu
void menu(int value) {
    switch(value) {
        case 0:
            exit(0); // Quit the program
            break;
        case 1:
            numIterations = 8000; // Change number of iterations to 8000
            glutPostRedisplay(); // Mark the current window as needing to be redisplayed
            break;
        case 2:
            colorMode = 1 - colorMode; // Toggle color mode
            glutPostRedisplay(); // Mark the current window as needing to be redisplayed
            break;
        case 3:
            // Change the number of vertices to 6
            numVertices = 6;
            glutPostRedisplay(); // Mark the current window as needing to be redisplayed
            break;
        case 4:
            // Change the number of vertices to 5
            numVertices = 5;
            glutPostRedisplay(); // Mark the current window as needing to be redisplayed
            break;
    }
}

// function to handle mouse motion events
void motion(int x, int y) {
    int dx = x - mouseX;
    int dy = y - mouseY;

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(dx, -dy, 0); // translate the scene by the difference in mouse position

    mouseX = x;
    mouseY = y;

    glutPostRedisplay(); // Mark the current window as needing to be redisplayed
}

void myinit(void)
{
    /* Enable blending and set white background color */
    glEnable(GL_BLEND);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    /* Set drawing color to black */
    glColor3f(0.0, 0.0, 0.0);

    /* Enable smooth shading */
    glShadeModel(GL_SMOOTH);

    /* Set blending function */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Set up viewing */
    /* 500 x 500 window with origin lower left */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0); // set up an orthographic projection
    glMatrixMode(GL_MODELVIEW);    
}

void display(void)
{
    /* Define a point data type */
    typedef GLfloat point2[2];
    
    /* Define the vertices of the pentagon and hexagon */
    point2 vertices5[5] = {
        {250.0, 450.0},
        {72.0, 292.0},
        {178.0, 88.0},
        {322.0, 88.0},
        {428.0, 292.0}
    };

    point2 vertices6[6] = {
        {250.0, 450.0},
        {108.0, 382.0},
        {108.0, 118.0},
        {250.0, 50.0},
        {392.0, 118.0},
        {392.0, 382.0}
    };

    point2 *vertices = NULL;
    
    /* Select the vertices to use based on the current number of vertices */
    if (numVertices == 5) {
        vertices = vertices5;
    } else if (numVertices == 6) {
        vertices = vertices6;
    }
    
    int i, j, k;
    point2 p = {250.0, 250.0}; /* An arbitrary initial point */
    
    /* Clear the window */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* Compute and plot numIterations new points */
    for (k = 0; k < numIterations; k++) {
        j = rand() % numVertices; /* Pick a vertex at random */

        /* Compute point one-third of the way towards vertex */
        GLfloat r = 1.0 / 3.0;

        /* Calculate the new point by interpolating between the current point and the selected vertex */
        p[0] = r * p[0] + (1 - r) * vertices[j][0]; 
        p[1] = r * p[1] + (1 - r) * vertices[j][1];
        
        /* Plot new point */
        glBegin(GL_POINTS);
            if (colorMode == 1) { // If random color mode is enabled, set the color to a random value
                glColor3f((GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX);
            }
            glVertex2fv(p); // Plot the new point using the current color
        glEnd();
    }
    
    /* Send all output to display */
    glFlush();

    glutSwapBuffers(); // Add this line to enable double buffering
}

int main(int argc, char** argv)
{
    srand(time(NULL)); // Seed the random number generator

    /* Initialize GLUT library */
    glutInit(&argc, argv);

    /* Set up the window */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Project01");

    /* Register callback functions */
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    /* Create the popup menu */
    popupMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Toggle random coloring", 2);
    glutAddMenuEntry("Change points to 8000", 1);
    glutAddMenuEntry("Change number of vertices to 6", 3); // new option
    glutAddMenuEntry("Change number of vertices to 5", 4); // new option
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    /* Initialize OpenGL */
    myinit();

    /* Enter the main event loop */
    glutMainLoop();

    return 0;
}
