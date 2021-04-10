#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
using namespace std;
#define PI 3.14159265358979324
#define xyz(r,a,t) r*cos(a)*cos(t) , r*cos(a)*sin(t) , r*sin(a)
#define color(R,G,B) .3+R/(1.3*r) , .3+G/(1.3*r) , .3+B/(1.3*r)
GLint  WindowHeight = 1000;
GLint  WindowWidth  = 1000;

GLfloat angle = 0;
int stacks = 41, sectors = 40;

int r = 300;
void circle(int x, int y, int r)
{
	GLfloat t, dt = 2 * PI / 100;
	glBegin(GL_LINE_STRIP);
	glLineWidth(3.0);
	// Draw circle
	for (t = 0; t <= 2 * PI + dt; t += dt)
	{
		glVertex2f(x + r * cos(t), y + r * sin(t));		// Formula of circle
	}
	glEnd();
}

void sphere()
{
	glPushMatrix();
	glRotatef(angle, 1, 0 , 0);
	glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, 0, 1);
	GLdouble t, dt = 2 * PI / stacks;
	for (t = 0; t <= 2 * PI ; t += dt)
	{
		GLfloat x = r * cos(t), y = r * sin(t);
		glPushMatrix();
		glRotatef(atan(y / x) * 180 / PI, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glColor3f(0.0, 0.0, 0.0);
		circle(0, 0, r);
		glPopMatrix();
	}

	dt = 2 * PI / sectors;
	for (t = 0; t <= 2 * PI ; t += dt)
	{
		glPushMatrix();
		glTranslatef(0, sin(t)*r, 0);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.0, 0.0, 0.0);
		circle(0, 0, sqrt(pow(r, 2.0) - pow(sin(t)*r, 2.0)));
		glPopMatrix();
	}


	glColor3f(1.0, 1.0, 0.0);

	GLfloat a, da = 2 * PI / sectors;
	for (a = 0; a <= 2 * PI ; a += da)
	{
		GLdouble t, dt = 2 * PI / stacks;
		for (t = 0; t <= 2 * PI ; t += dt)
		{
			GLfloat vertex[4][3] = {
				{ xyz(r, a, t) },
				{ xyz(r, a, t + dt) },
				{ xyz(r, a + da, t + dt) },
				{ xyz(r, a + da, t) }
			};

			glBegin(GL_POLYGON);
			// glColor3f(color(vertex[0][0],vertex[0][1],vertex[0][2]));
			for (int i = 0; i < 4; i++)
			{	
				glColor3f(color(vertex[i][0],vertex[i][1],vertex[i][2]));
				glVertex3fv(vertex[i]);
			}
			glEnd();

			// glEnable(GL_POINT_SMOOTH);
			// glPointSize(20.0);
			// glBegin(GL_POINTS);
			// for (int i = 0; i < 4; i++)
			// {
			// 	glColor3f(0, 0, 0);
			// 	glVertex3fv(vertex[i]);
			// }
			// glEnd();
		}
	}
	glPopMatrix();
}
void display()
{
	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	sphere();
	glutSwapBuffers();
}

void update(int time)
{
	angle += .5;
	glutPostRedisplay();
	glutTimerFunc(time, update, time);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);//resizing the window

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	WindowWidth = w;
	WindowHeight = h;
	// L      R     B    T      N       F
	glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void key(unsigned char key, int, int)
{
	if (key == 27) glutLeaveMainLoop();
	if (key == 'a') if (stacks > 0)stacks-=1;
	if (key == 'd') stacks+=1;
	if (key == 'w') sectors+=1;
	if (key == 's') if (sectors > 0)sectors-=1;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);	// Initialize GLUT // Without this : Other glut functions can't be called .
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);  // Position the window's initial top-left corner
	glutInitWindowSize(WindowWidth, WindowHeight);	// Set the window's initial width & height // Default value : 300,300
	glutCreateWindow("Donut");	 // Create a window with the given title	// Without this : ERROR:  glutMainLoop called with no windows created.
	glutDisplayFunc(display);	  // Register display callback handler for window re-paint  // Without this: ERROR:  No display callback registered for window 1
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutTimerFunc(10, update, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(3.0);
	glEnable(GL_LINE_SMOOTH);
	// init();
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMainLoop();		 // Enter the event-processing loop
	return 0;
}