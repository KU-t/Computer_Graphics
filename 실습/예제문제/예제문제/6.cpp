#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUMBER 9

enum { RT, RB, LT, LB };
enum { stop, go, Clock };
enum { L, D, R, U };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void main(int argc, char ** argv);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);

int ani[NUMBER] = {};
int NUM = 0;
int mx[NUMBER] = { 0 }, my[NUMBER] = { 0 }, size[NUMBER] = { 0 };
int cx[NUMBER] = { 0 }, cy[NUMBER] = { 0 };
int way[NUMBER];
int move[NUMBER];
int cmove[NUMBER];
bool Stop_Switch;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	
	for (int i = 0; i < NUMBER; i++) {
		ani[NUMBER] = 1;
		move[i] = stop;
		cmove[NUMBER] = L;
	}
	
	Stop_Switch = false;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3Àå ½Ç½À 6");
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene);
	glutTimerFunc(50, Timer, 1);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}


GLvoid drawScene() {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < NUMBER; i++) {
		if (mx[i] != 0 && my[i] != 0) {
			glColor4f((rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, 0);
			glRectf(mx[i], my[i], mx[i] + size[i], my[i] + size[i]);
		}
	}
	glFlush();
}


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, 800.0, 0, 600.0, -1.0, 1.0);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx[NUM] = x;
		my[NUM] = 600 - y;
		size[NUM] = rand() % 20 + 10;
		way[NUM] = rand() % 4;
		move[NUM] = go;
		cmove[NUM] = L;
		NUM++;

		if (NUM > NUMBER - 1) {
			NUM = 0;
		}

		glutPostRedisplay();
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':	move[1] = Clock;
		break;
	case '2':	move[2] = Clock;
		break;
	case '3':	move[3] = Clock;
		break;
	case '4':	move[4] = Clock;
		break;
	case '5':	move[5] = Clock;
		break;
	case '6':	move[6] = Clock;
		break;
	case '7':	move[7] = Clock;
		break;
	case '8':	move[8] = Clock;
		break;	
	case '9':	move[0] = Clock;
		break;
	case '0':
		for (int i = 0; i < NUMBER; i++) 
			move[i] = Clock;
		break;
	case 's': case 'S':
		if (Stop_Switch == false)	Stop_Switch = true;
		else if (Stop_Switch == true)	Stop_Switch = false;
		break;
	}
}

void Timer(int value) {
	
	if (Stop_Switch == false) {

		for (int i = 0; i < NUMBER; i++) {
			size[i] += ani[i];
			if (size[i] >= 50)	ani[i] = -1;
			else if (size[i] <= 30)	ani[i] = 1;

			if (move[i] == go) {
				switch (way[i]) {
				case RT:
					if (mx[i] + size[i] >= 800)	way[i] = LT;
					else if (my[i] + size[i] >= 600) way[i] = RB;
					mx[i]++;
					my[i]++;
					break;

				case RB:
					if (mx[i] + size[i] >= 800) way[i] = LB;
					else if (my[i] <= 0) way[i] = RT;
					mx[i]++;
					my[i]--;
					break;

				case LT:
					if (mx[i] <= 0) way[i] = RT;
					else if (my[i] + size[i] >= 600) way[i] = LB;
					mx[i]--;
					my[i]++;
					break;

				case LB:
					if (mx[i] <= 0) way[i] = RB;
					else if (my[i] <= 0) way[i] = LT;
					mx[i]--;
					my[i]--;
					break;
				}
				cx[i] = mx[i];
				cy[i] = my[i];
			}

			else if (move[i] == Clock) {
				switch (cmove[i]) {
				case L:
					mx[i]-=10;
					if (mx[i] <= 10) cmove[i] = D;
					break;

				case D:
					my[i]-=10;
					if (my[i] <= size[i] + 15) cmove[i] = R;
					break;

				case R:
					mx[i]+=10;
					if (mx[i] >= 800 - 15 - size[i]) cmove[i] = U;
					break;

				case U:
					if (my[i] != cy[i])	my[i]+=10;
					else if (my[i] == cy[i] && mx[i] != cx[i])	mx[i]-=10;
					else if (my[i] == cy[i] && mx[i] == cx[i]) {
						move[i] = go;
						cmove[i] = L;
					}
					break;
				}
			}

		}
	}
	glutPostRedisplay();
	
	glutTimerFunc(50, Timer, 1);
}