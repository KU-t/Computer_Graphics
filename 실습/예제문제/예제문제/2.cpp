#include <gl/freeglut.h>
#include <time.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("3¿Â Ω«Ω¿ 2");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {

	int Random = rand() % 10 + 5;
	int Ran_R , Ran_G , Ran_B;
	float R, G, B;

	Ran_R = rand() % 255;
	Ran_G = rand() % 255;
	Ran_B = rand() % 255;

	R = Ran_R * 0.001;
	G = Ran_G * 0.001;
	B = Ran_B * 0.001;

	for (int i = 0; i <= Random; i++) {
		//glRectf(-400 + (i-1) * (800/Random), 300, -400 + i * (800 / Random), -300);
		for (int j = 0; j <= Random; j++) {
			glColor4f(R*(i*1.5)*(j*0.03), G*(i*1.5)*(j*0.03), B*(i*1.5)*(j*0.03), 1);
			glRectf(-400 + (i-1) * (800/Random), -300 + (j - 1) * (600 / Random), -400 + i * (800 / Random), -300 + j * (600 / Random));
		}
	}

	glFlush();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(-400.0, 400.0, -300.0, 300.0, -1.0, 1.0);
}
