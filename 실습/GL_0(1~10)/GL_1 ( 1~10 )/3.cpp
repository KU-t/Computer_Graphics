#include <gl/freeglut.h>
#include <time.h>

using namespace std;

enum{Two,Three,Four,Five};
enum{go,re};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

class Shape {
public:
	int shape;
	bool way = go;

	Shape() {
		srand((unsigned)time(NULL));
		shape = rand() % 4;
	}

	void Draw(int x, int y) {
		
		glBegin(GL_LINE_LOOP);
		
		switch (shape) {
		case Two:
			glVertex2i(x + 30, y - 30);
			glVertex2i(x + 70, y - 70);
			break;

		case Three:
			glVertex2i(x + 30, y - 60);
			glVertex2i(x + 60, y - 60);
			glVertex2i(x + 50, y - 30);
			break;

		case Four:
			glVertex2i(x + 30, y - 30);
			glVertex2i(x + 30, y - 70);
			glVertex2i(x + 70, y - 70);
			glVertex2i(x + 70, y - 30);
			break;

		case Five:
			glVertex2i(x + 50, y - 30);
			glVertex2i(x + 30, y - 50);
			glVertex2i(x + 40, y - 70);
			glVertex2i(x + 60, y - 70);
			glVertex2i(x + 70, y - 50);
			break;
		}
		glEnd();

		if (shape == Five)
			way = re;
		else if (shape == Two)
			way = go;

		if (way == go)
			shape++;
		else if (way == re)
			shape--;
	}

};

Shape s;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(300,300);
	glutCreateWindow("3Àå ½Ç½À 3");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glBegin(GL_LINES);
	glVertex2i(0, 100);
	glVertex2i(300, 100);

	glVertex2i(0, 200);
	glVertex2i(300, 200);

	glVertex2i(100, 0);
	glVertex2i(100, 300);

	glVertex2i(200, 0);
	glVertex2i(200, 300);
	glEnd();

	for (int j = 300; j > 0; j -= 100) {
		for (int i = 0; i < 300; i+=100) {
			s.Draw(i, j);
		}
	}

	glFlush();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, 300.0, 0, 300.0, -1.0, 1.0);
}
