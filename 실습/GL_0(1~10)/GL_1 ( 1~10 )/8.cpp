#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define PI 3.141592
#define NUMBER 10

enum { L, R };

class Shape {
public:
	int mx, my;
	float size;
	float R, G, B;
	bool exist;
	int Random;
	int cnt_1,cnt_2;
	bool switch_back;

	Shape(int _mx = 0, int _my = 0, float _size = 1, bool _exist = false, int _Random = 0, int _cnt_1 = 0, int _cnt_2 = 0, bool switch_back = false) {}
	
	void Draw() {

		if (exist) {
			
			if (Random == 0) {
				
				for (int i = 0; i < cnt_1; i++) {
					float x, y;
					x = i / 6 * cos(PI / 180 * i) + mx;
					y = i / 6 * sin(PI / 180 * i) + my;
					
					glBegin(GL_POINTS);
					glVertex2d(x, y);
					glEnd();
				}
				
				if (cnt_1 >= 720)	switch_back = true;
				
				
				if (switch_back) {
					for (int i = 0; i < cnt_2; i++) {
						float x, y;
						x = (720 - i) / 6 * -cos(PI / 180 * i) + mx + 240;
						y = (720 - i) / 6 * sin(PI / 180 * i) + my;
						
						glBegin(GL_POINTS);
						glVertex2d(x, y);
						glEnd();
					}

					if (cnt_2 >= 720) {
						switch_back = false;
						exist = false;
						cnt_1 = 0;
						cnt_2 = 0;
					}
				}
			}

			if (Random == 1) {

				for (int i = 0; i < cnt_1; i++) {
					float x, y;
					x = i / 6 * -cos(PI / 180 * i) + mx;
					y = i / 6 * sin(PI / 180 * i) + my;
					
					glBegin(GL_POINTS);
					glVertex2d(x, y);
					glEnd();
				}

				if (cnt_1 >= 720)	switch_back = true;


				if (switch_back) {
					for (int i = 0; i < cnt_2; i++) {
						float x, y;
						x = (720 - i) / 6 * +cos(PI / 180 * i) + mx - 240;
						y = (720 - i) / 6 * sin(PI / 180 * i) + my;
						
						glBegin(GL_POINTS);
						glVertex2d(x, y);
						glEnd();
					}

					if (cnt_2 >= 720) {
						switch_back = false;
						exist = false;
						cnt_1 = 0;
						cnt_2 = 0;
					}
				}
			}

		}
	}

	void Clear() {
		exist = false;
		cnt_1 = 0;
		cnt_2 = 0;
		switch_back = false;
	}
};

Shape s[NUMBER];
int NUM = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Timer(int value);


void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3¿Â Ω«Ω¿ 8");
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(10, Timer, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < NUMBER; i++) {

		glColor4f(s[i].R, s[i].G, s[i].B, 1.0f);
		glPointSize(3.3);

		if (s[i].exist) {
			s[i].Draw();
		}
	}

	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int R = rand() % 100, G = rand() % 100, B = rand() % 100;
		s[NUM].Clear();
		s[NUM].R = R * 0.01;
		s[NUM].G = G * 0.01;
		s[NUM].B = B * 0.01;
		s[NUM].exist = true;
		s[NUM].mx = x;
		s[NUM].my = 600 - y;
		s[NUM].Random = rand() % 2;

		NUM = (NUM + 1)% NUMBER;
	}

	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, 800.0, 0, 600.0, -1.0, 1.0);
}

void Timer(int value) {
	for (int i = 0; i < NUMBER; i++) {
		if (s[i].exist) {
			if (!s[i].switch_back)
				s[i].cnt_1++;
			else if (s[i].switch_back)
				s[i].cnt_2++;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}
