#include <gl/freeglut.h>
#include <time.h>
#include <stdlib.h>

#define W_w 800.0
#define W_h 600.0
#define Recs 30

enum { RT, RB, LT, LB };

class REctangle {
public:
	int px, py;
	int size;
	float R, G, B;
	bool exist;
	int direction;
	int animation_cnt;
	int wait_cnt;

	REctangle() {
		int Rr, Rg, Rb;
		Rr = rand() % 100;	Rg = rand() % 100;	Rb = rand() % 100;
		R = Rr * 0.01;	G = Rg * 0.01;	B = Rb * 0.01;
		px = rand() % 780 + 20;
		py = rand() % 580 + 20;
		size = 10;
		exist = true;
		direction = rand() % 4;
		animation_cnt = size;
		wait_cnt = 0;
	}

	void Draw() {
		if (exist) {
			glColor4f(R, G, B, 1.0f);
			glBegin(GL_POLYGON);
			glVertex2i(px - size, py + size);
			glVertex2i(px + size, py + size);
			glVertex2i(px + size, py - size);
			glVertex2i(px - size, py - size);
			glEnd();
		}

		else if (!exist) {
			wait_cnt++;
			
			if (wait_cnt >= 1000)
				animation_cnt--;

			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glBegin(GL_POLYGON);
			glVertex2i(px - size + animation_cnt, py + size + animation_cnt);
			glVertex2i(px + size - animation_cnt, py + size + animation_cnt);
			glVertex2i(px + size + animation_cnt, py - size - animation_cnt);
			glVertex2i(px - size - animation_cnt, py - size - animation_cnt);
			glEnd();

			if (animation_cnt <= 0) {
				exist = true;
				wait_cnt = 0;
				animation_cnt = size;
			}
		}
	}
};

GLvoid drawScene(GLvoid);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
GLvoid Reshape(int w, int h);
void Collision_check();
void Timer(int value);

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
	glutCreateWindow("3장 실습 10");
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Recs; i++) {
		//if ((Rec + i)->exist)	
		(Rec + i)->Draw();
	}

	if (Eraser->exist)	Eraser->Draw();
	glFlush();
}

void Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Eraser->exist = true;
		Eraser->px = x;
		Eraser->py = 600 - y;
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		Eraser->exist = false;

	glutPostRedisplay();
}

//움직일때마다 충돌 체크
void Motion(int x, int y) {
	Eraser->px = x;
	Eraser->py = 600 - y;

	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, W_w, 0, W_h, -1.0, 1.0);
}

//충돌체크
void Collision_check() {
	for (int i = 0; i < Recs; i++) {
		if ((Rec + i)->exist && Eraser->exist) {
			if (abs(Eraser->px - (Rec + i)->px) <= Eraser->size + (Rec + i)->size) {
				if (abs(Eraser->py - (Rec + i)->py) <= Eraser->size + (Rec + i)->size) {
					(Rec + i)->exist = false;
					(Rec + i)->direction = rand() % 4;
				}
			}
		}
	}
}

void Timer(int value) {

	for (int i = 0; i < Recs; i++) {

		switch ((Rec+i)->direction) {
		case RT:
			if ((Rec + i)->px + (Rec + i)->size >= W_w)	
				(Rec + i)->direction = LT;
			else if ((Rec+i)->py + (Rec+i)->size >= W_h) 
				(Rec+i)->direction = RB;
			(Rec+i)->px++;
			(Rec+i)->py++;
			break;

		case RB:
			if ((Rec+i)->px + (Rec+i)->size >= W_w) 
				(Rec+i)->direction = LB;
			else if ((Rec+i)->py <= 0) 
				(Rec+i)->direction = RT;
			(Rec+i)->px++;
			(Rec+i)->py--;
			break;

		case LT:
			if ((Rec+i)->px <= 0) 
				(Rec+i)->direction = RT;
			else if ((Rec+i)->py + (Rec+i)->size >= W_h) 
				(Rec+i)->direction = LB;
			(Rec+i)->px--;
			(Rec+i)->py++;
			break;

		case LB:
			if ((Rec+i)->px <= 0) 
				(Rec+i)->direction = RB;
			else if ((Rec+i)->py <= 0) 
				(Rec+i)->direction = LT;
			(Rec+i)->px--;
			(Rec+i)->py--;
			break;
		}

		if (!(Rec + i)->exist) {
			(Rec + i)->wait_cnt++;

			if ((Rec + i)->wait_cnt >= 100)
				(Rec + i)->animation_cnt--;

			if ((Rec + i)->animation_cnt <= 0) {
				(Rec + i)->exist = true;
				(Rec + i)->wait_cnt = 0;
				(Rec + i)->animation_cnt = (Rec + i)->size;
			}
		}
	}

	Collision_check();
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}