#include <gl/freeglut.h>

#define W_w 800
#define W_h	600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_w, W_h);
	glutCreateWindow("3¿Â Ω«Ω¿ 1");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_LINES);
	glVertex2i(-1.0f, 0.0f);
	glVertex2i(1.0f, 0.0f);
	glVertex2i(0.0f, -1.0f);
	glVertex2i(0.0f, 1.0f);
	glEnd();

	glColor4f(0.5f, 1.0f, 1.0f, 1.0f);
	glRectf(-1.0f, 1.0f, -0.5f, 0.5f);

	glColor4f(0.0f, 1.0f, 0.25f, 1.0f);
	glRectf(0.5f, 1.0f, 1.0f, 0.5f);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glRectf(-1.0f, -0.5f, -0.5f, -1.0f);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glRectf(0.5f, -0.5f, 1.0f, -1.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.2f);
	glVertex2f(-0.14f, -0.2f);
	glVertex2f(0.14f, -0.2f);
	glEnd();


	glFlush();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}