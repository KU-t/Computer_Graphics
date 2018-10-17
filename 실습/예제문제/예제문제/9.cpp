#include <gl/freeglut.h>
#include <time.h>
#include <stdlib.h>

#define W_w 800.0
#define W_h 600.0
#define Recs 100

class REctangle {
public:
	int px, py;
	int size;
	float R, G, B;
	bool exist;

	REctangle() {
		int Rr, Rg, Rb;
		Rr = rand() % 100;	Rg = rand() % 100;	Rb = rand() % 100;
		R = Rr * 0.01;	G = Rg * 0.01;	B = Rb * 0.01;
		px = rand() % 780 + 20;
		py = rand() % 580 + 20;
		size = 10;
		exist = true;
	}

	void Draw() {
		glColor4f(R, G, B, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2i(px - size, py + size);
		glVertex2i(px + size, py + size);
		glVertex2i(px + size, py - size);
		glVertex2i(px - size, py - size);
		glEnd();
	}
};

GLvoid drawScene(GLvoid);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
GLvoid Reshape(int w, int h);
void Collision_check();

REctangle *Rec;
REctangle *Eraser;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	Rec = new REctangle[Recs];
	Eraser = new REctangle;
	
	Eraser->R = 0.f;
	Eraser->G = 0.f;
	Eraser->B = 0.f;
	Eraser->size = 20;
	Eraser->exist = false;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_w, W_h);
	glutCreateWindow("3장 실습 9");
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Recs; i++) {
		if((Rec+i)->exist)	(Rec+i)->Draw();
	}

	if (Eraser->exist)	Eraser->Draw();
	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	
	Eraser->exist = false;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Eraser->exist = true;
		Eraser->px = x;
		Eraser->py = 600 - y;
	}

	glutPostRedisplay();
}

//움직일때마다 충돌 체크
void Motion(int x, int y) {
	Eraser->px = x;
	Eraser->py = 600 - y;

	Collision_check();

	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, W_w, 0, W_h, -1.0, 1.0);
}

//충돌체크
void Collision_check() {
	for (int i = 0; i < Recs; i++) {
		if ((Rec + i)->exist) {
			if (abs(Eraser->px - (Rec + i)->px) <= Eraser->size + (Rec + i)->size) {
				if (abs(Eraser->py - (Rec + i)->py) <= Eraser->size + (Rec + i)->size ) {
					(Rec + i)->exist = false;
				}
			}
		}
	}
}