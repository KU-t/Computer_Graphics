#include <gl/freeglut.h>
#include <math.h>

enum SHAPE { Sphere, Hexa, Cone, Teapot };

#define W_x 800
#define W_y 600
#define W_z 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Change_Angle_xyz();
void Draw_Coordinates();
void Draw_bottom();
void count_rad();

class Angle {
public:
	float radian;
	bool sw;
	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Object {
public:
	float x, y;
	SHAPE shape = Sphere;

	void Draw_Object() {
		switch (shape) {
		case Sphere:
			break;
		case Hexa:
			break;
		case Cone:
			break;
		case Teapot:
			break;
		}
	}


};

Angle Angle_x, Angle_y, Angle_z;
float cam_rad_xy = 0;

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("3장 실습 16");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);
	Draw_Coordinates();
	Draw_bottom();
	
	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 1000.0);
	glTranslatef(0.0, 0.0, -300.0);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.f, 0.f, 300.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	//glOrtho(0, W_x, 0, W_y, 0, W_z);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'x':	case 'X':
		Angle_x.sw = true;
		break;
	case 'y':	case 'Y': 
		Angle_y.sw = true;
		break;
	case 'z':	case 'Z':
		Angle_z.sw = true;
		break;
	case ']':
		cam_rad_xy += 1.f;
	}
	glutPostRedisplay();
}

void Change_Angle_xyz() {

	if (Angle_x.sw) {
		Angle_x.radian+=2;
		Angle_x.sw = false;
	}
	if (Angle_y.sw) {
		Angle_y.radian+=2;
		Angle_y.sw = false;
	}
	if (Angle_z.sw) {
		Angle_z.radian+=2;
		Angle_z.sw = false;
	}
	glutPostRedisplay();
}

void Draw_Coordinates() {
	//X축 
	glPushMatrix();
	glTranslatef(26.f, 0.f, 0.f);
	glScalef(25.f, 1.f, 1.f);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50.f, 0.f, 0.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glutSolidCone(5.f, 10.f, 10.f, 10.f);
	glPopMatrix();

	//Y축
	glPushMatrix();
	glTranslatef(0.f, 26.f, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);
	glScalef(25.f, 1.f, 1.f);
	glColor3f(0.f, 1.f, 0.f);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 50.f, 0.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(5.f, 10.f, 10.f, 10.f);
	glPopMatrix();

	//Z축
	glPushMatrix();
	glTranslatef(0.f, 0.f, 26.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glScalef(25.f, 1.f, 1.f);
	glColor3f(0.f, 0.f, 1.f);
	glutSolidCube(2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.f, 0.f, 50.f);
	//glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(5.f, 10.f, 10.f, 10.f);
	glPopMatrix();
}

void Draw_bottom() {
	glPushMatrix();
	glTranslatef(0.f, -150.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f,1.f,0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

void count_rad() {

}