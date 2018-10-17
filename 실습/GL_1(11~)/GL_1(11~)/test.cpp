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

float move_x = 0, move_y = 0, move_z = 0;

class Object {
public:
	SHAPE shape = Sphere;
	float left_obj_radian = 0.f, right_obj_radian = 0.f;
	bool left_obj_sw = false, right_obj_sw = false;

	void Draw_Object() {
		glPushMatrix();
		glColor3f(0.f, 1.f, 0.f);
		glTranslatef(-100.f, 75.f, 0.f);
		glRotated(left_obj_radian, 0.f, 1.f, 0.f);
		switch (shape) {
		case Sphere:
			glutSolidSphere(50.f, 100.f, 100.f);
			glColor3f(0.7f, 0.7f, 0.7f);
			glutWireSphere(51.f, 25.f, 25.f);
			break;
		case Hexa:
			glutSolidCube(50.f);
			glColor3f(0.7f, 0.7f, 0.7f);
			glutWireCube(51.f);
			break;
		case Cone:
			glPushMatrix();
			glTranslatef(0.f, -30.f, 0.f);
			glRotatef(-90.f, 1.f, 0.f, 0.f);
			glutSolidCone(50.f, 100.f, 100.f, 100.f);
			glColor3f(0.7f, 0.7f, 0.7f);
			glutWireCone(51.f, 101.f, 25.f, 25.f);
			glPopMatrix();
			break;
		case Teapot:
			glutSolidTeapot(50.f);
			glColor3f(0.7f, 0.7f, 0.7f);
			glutWireTeapot(51.f);
			break;
		}
		glPopMatrix();


		glPushMatrix();
		glColor3f(0.f, 1.f, 0.f);
		glTranslatef(100.f, 75.f, 0.f);
		glRotated(right_obj_radian, 0.f, 1.f, 0.f);
		switch (shape) {
		case Sphere:
			glutWireSphere(50.f, 50.f, 50.f);
			break;
		case Hexa:
			glutWireCube(50.f);
			break;
		case Cone:
			glPushMatrix();
			glTranslatef(0.f, -30.f, 0.f);
			glRotatef(-90.f, 1.f, 0.f, 0.f);
			glutWireCone(50.f, 100.f, 50.f, 50.f);
			glPopMatrix();
			break;
		case Teapot:
			glutWireTeapot(50.f);
			break;
		}
		glPopMatrix();
	}


};

Angle Angle_x, Angle_y, Angle_z;
Object obj;
float cam_rad_xy = 0;

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("16");
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

	glLoadIdentity();
	gluLookAt(move_x, move_y, move_z - 300.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

	glPushMatrix();
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);
	Draw_Coordinates();
	Draw_bottom();
	obj.Draw_Object();
	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 1000.0);
	glTranslatef(0.0, 0.0, -0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 300.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
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
		Angle_x.sw = (Angle_x.sw + 1) % 2;
		break;
	case 'y':	case 'Y':
		Angle_y.sw = (Angle_y.sw + 1) % 2;
		break;
	case 'z':	case 'Z':
		Angle_z.sw = (Angle_z.sw + 1) % 2;
		break;

	case 'a': case 'A':
		move_x--;
		break;

	case 'd': case 'D':
		move_x++;
		break;

	case 'w': case 'W':
		move_y++;
		break;

	case 's': case 'S':
		move_y--;
		break;

	case '+': case '=':
		move_z += 10;
		break;

	case '-': case '_':
		move_z -= 10;
		break;

	case 'i': case 'I':
		move_x = 0.f;
		move_y = 0.f;
		move_z = 0.f;
		Angle_x.radian = 0.f;
		Angle_y.radian = 0.f;
		Angle_z.radian = 0.f;
		break;

	case 'o': case 'O':
		if (!(obj.left_obj_sw == true && obj.right_obj_sw == true)) {
			obj.left_obj_sw = true;
			obj.right_obj_sw = true;
		}
		else {
			obj.left_obj_sw = false;
			obj.right_obj_sw = false;
		}
		break;
	case ']':
		cam_rad_xy += 1.f;
		break;
	case '1':
		obj.shape = Sphere;
		break;
	case '2':
		obj.shape = Hexa;
		break;
	case '3':
		obj.shape = Cone;
		break;
	case '4':
		obj.shape = Teapot;
		break;
	}
	glutPostRedisplay();
}

void Change_Angle_xyz() {
	if (Angle_x.sw) Angle_x.radian += 2;
	if (Angle_y.sw) Angle_y.radian += 2;
	if (Angle_z.sw) Angle_z.radian += 2;
	obj.left_obj_radian += 2;
	obj.right_obj_radian += 2;
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
	glTranslatef(0.f, 0.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

void count_rad() {

}