#include <gl/freeglut.h>
#include <iostream>
#include <math.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define PI 3.141592
#define rect_size 40

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Draw_sin_graph();
void Draw_spring_graph();
void Draw_ribbon();
void Draw_rectangle();

bool sw_Move_animation = true;
bool sw_Angle_animation = false;
int cnt_animation = 0;
int Posx = 0, Posy = 0;
float Scale = 1;
float Angle = 0;

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("3¿Â Ω«Ω¿ 11");
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();
	glPushMatrix();

	glScalef(Scale, 1.0f, 1.0f);
	glRotatef(Angle, 1.0f, 0.f, 0.f);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(-W_x, 0, 0);
		glVertex3f(W_x, 0, 0);
		glVertex3f(0, -W_y, 0);
		glVertex3f(0, W_y, 0);
	glEnd();
	

	Draw_sin_graph();
	Draw_spring_graph();
	Draw_ribbon();
	Draw_rectangle();

	glPopMatrix();
	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a': case 'A':
		Posx-=2;
		break;

	case 'd': case 'D':
		Posx+=2;
		break;

	case 'w': case 'W':
		Posy+=2;
		break;

	case 's': case 'S':
		Posy-=2;
		break;

	case 't': case 'T':
		sw_Move_animation = (sw_Move_animation + 1) % 2;
		break;

	case 'y': case 'Y':
		sw_Angle_animation = (sw_Angle_animation + 1) % 2;
		break;

	case 'q': case 'Q':
		exit(true);
		break;

	case '-': case '_':
		if(Scale <= 1.7f)	Scale += 0.1f;
		break;

	case '=': case '+':
		if (Scale >= 0.3f)	Scale -= 0.1f;
		break;

	case '[':	case '{':
		Angle += 15.f;
		break;

	case ']': case '}':
		Angle -= 15.f;
		break;
	}
}

void Mouse(int button, int state, int x, int y) {

	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	if (sw_Move_animation)	cnt_animation = (cnt_animation + 1) % 360;
	if (sw_Angle_animation) Angle = (Angle + 1.f);
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}

void Draw_sin_graph() {
	float x = 0;
	float y = 0;
	
	glColor3f(1.0, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 360 * 3; i++, x++) {
		x = i - (W_x * 2 / 3) + Posx;
		y = 100 * sin((i + cnt_animation) * PI / 180) + (W_y * 3 / 5) + Posy;
			glVertex3f(x, y, 0);
	}
	glEnd();
	glutPostRedisplay();
}

void Draw_spring_graph() {
	float x = 0;
	float y = 0;

	glColor3f(0, 1.0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 360 * 3; i++, x++) {
		x = 180 * cos((i + cnt_animation) * PI / 180) + i - (W_x * 2 / 3) + Posx;
		y = 180 * sin((i + cnt_animation) * PI / 180) + (W_y / 5) + Posy;
		glVertex3f(x, y, 0);
	}
	glEnd();
	glutPostRedisplay();
}

void Draw_ribbon() {
	glColor3f(0, 0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(Posx, -W_y/3 + Posy, 0);
	glVertex3f(-1.5 * cnt_animation + Posx, -W_y/3 + cnt_animation / 3 + Posy, 0);
	glVertex3f(-1.5 * cnt_animation + Posx, -W_y/3 - cnt_animation / 3 + Posy, 0);
	glVertex3f(1.5 * cnt_animation + Posx, -W_y / 3 + cnt_animation / 3 + Posy, 0);
	glVertex3f(1.5 * cnt_animation + Posx, -W_y / 3 - cnt_animation / 3 + Posy, 0);
	glEnd();
	glutPostRedisplay();
}

void Draw_rectangle() {
	glColor4f(1, 1, 0, 0);

	glRectf(-720 + cnt_animation - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, -720 + cnt_animation + rect_size + Posx, -W_y * 2 / 3 + rect_size + Posy);
	glRectf(-360 + cnt_animation - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, -360 + cnt_animation + rect_size + Posx, -W_y * 2 / 3 + rect_size + Posy);
	glRectf(cnt_animation - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, cnt_animation + rect_size + Posx, -W_y * 2 / 3 + rect_size + Posy);

	if (cnt_animation < 360 - 2 * rect_size)
		glRectf(360 + cnt_animation - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, 360 + cnt_animation + rect_size + Posx, -W_y * 2 / 3 + rect_size + Posy);
	else if (cnt_animation >= 360 - 2 * rect_size) {
		glRectf(360 + cnt_animation - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, 680 + Posx, -W_y * 2 / 3 + rect_size + Posy);
		glRectf(-720 - rect_size + Posx, -W_y * 2 / 3 - rect_size + Posy, -720 + ( cnt_animation - 320) + Posx, -W_y * 2 / 3 + rect_size + Posy);
	}
	glutPostRedisplay();
}