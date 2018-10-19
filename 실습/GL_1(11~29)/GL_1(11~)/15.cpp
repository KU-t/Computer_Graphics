#include <gl/freeglut.h>
#include <time.h>
#include <iostream>

using namespace std;

#define W_x 300.f
#define W_y 300.f
#define W_z 300.f

#define len 6.f
#define len_z (len /2.f)
#define area 8.f
#define gap (area * 1.8f)
#define unit ((len * area) + gap)
#define wire_cube 0.1f
#define point_x 98.f
#define point_y 20.f

void SHOW();
void Init();
void Blink(int _x, int _y);

float y_angle = 0;
bool blink = false;

class number {
public:
	float x, y, z;
	int NUM = 0;
	
	bool handle[7];

	void Draw_Object(float Trans_x,float Trans_y,float  Trans_z, float Rot) {
		glPushMatrix();
		glTranslatef(Trans_x, Trans_y, Trans_z);
		glRotatef(Rot, 0.f, 0.f, 1.f);
		glScalef(len, 1.f, 1.f);
		glScalef(1.f, 1.f, len_z);
		glColor4f(rand() % 255 / 255.0, rand() % 255 / 255.0, rand() % 255 / 255.0, 0.5f);
		glutSolidCube(area);
		glColor4f(rand() % 255 / 255.0, rand() % 255 / 255.0, rand() % 255 / 255.0, 0.5f);
		glutWireCube(area + wire_cube);
		glPopMatrix();
	}

	void draw() {
			glPushMatrix();
			glTranslatef(x, y, z);
			
			if (handle[0])		Draw_Object(0.f, unit, 0.f, 0.f);
			if (handle[1])		Draw_Object(-unit / 2.f, unit / 2.f, 0.f, 90.f);
			if (handle[2])		Draw_Object(unit / 2.f, unit / 2.f, 0.f, 90.f);
			if (handle[3])		Draw_Object(0.f, 0.f, 0.f, 0.f);
			if (handle[4])		Draw_Object(-unit / 2.f, -unit / 2.f, 0.f, 90.f);
			if (handle[5])		Draw_Object(unit / 2.f, -unit / 2.f, 0.f, 90.f);
			if (handle[6])		Draw_Object(0.f, -unit, 0.f, 0.f);
			
			glPopMatrix();
	}

	void draw_2(){
		switch (NUM){	// false = 꺼진다. true = 켜진다.
		case 0:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[3] = false;
			break;
		case 1:
			for (int i = 0; i < 7; i++) handle[i] = false;		handle[2] = handle[5] = true;
			break;
		case 2:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[1] = 	handle[5] = false;
			break;
		case 3:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[1] = handle[4] = false;
			break;
		case 4:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[0] = handle[4] = handle[6] = false;
			break;
		case 5:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[2] = handle[4] =  false;
			break;
		case 6:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[2] = false;
			break;
		case 7:
			for (int i = 0; i < 7; i++) handle[i] = false;		handle[0] = handle[2] = handle[5] = true;
			break;
		case 8:
			for (int i = 0; i < 7; i++) handle[i] = true;
			break;
		case 9:
			for (int i = 0; i < 7; i++) handle[i] = true;		handle[4] = false;
			break;
		}
	}
};

number p[6];

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);

void main(int argc, char **argv) {
	Init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("3장 실습 15");
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, Timer, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.f, 0.f, 0.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glRotatef(30.f, 1.f, -1.f, 0.f);
	glRotatef(y_angle, 0.f, 1.f, 0.f);
	SHOW();

	if (blink) {
		Blink(1, 1);
		Blink(1, -1);
		Blink(-1, 1);
		Blink(-1, -1);
	}

	glPopMatrix();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
}

void Timer(int value) {
	time_t timer = time(NULL); // 현재 시각을 초 단위로 얻기
	struct tm *t;

	t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

	int hour10 = t->tm_hour / 10;
	int hour1 = t->tm_hour % 10;
	int min10 = t->tm_min / 10;
	int min1 = t->tm_min % 10;
	int sec10 = t->tm_sec / 10;
	int sec1 = t->tm_sec % 10;

	p[0].NUM = hour10;
	p[1].NUM = hour1;
	p[2].NUM = min10;
	p[3].NUM = min1;
	p[4].NUM = sec10;
	p[5].NUM = sec1;

	cout << hour10 << hour1 << " : " << min10 << min1 << " : " << sec10 << sec1 << endl;

	blink = sec1 % 2;

	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '[': case'{':		y_angle -= 10.f;
		break;
	case ']': case'}':		y_angle += 10.f;
		break;
	}
	glutPostRedisplay();
}

void SHOW() {
	for (int i = 0; i < 6; i++) {
		p[i].draw_2();
		p[i].draw();
	}
}

void Init(){
	p[0].x = -240.f;
	p[1].x = -155.f;
	p[2].x = -42.f;
	p[3].x = 42.f;
	p[4].x = 155.f;
	p[5].x = 240.f;
}

void Blink(int _x, int _y) {
	glPushMatrix();
	glTranslatef(_x * point_x, _y * point_y, 0.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glutSolidCube(10);
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glutWireCube(10 + wire_cube);
	glPopMatrix();
}