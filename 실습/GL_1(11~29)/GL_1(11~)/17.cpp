#include <gl/freeglut.h>
#include <math.h>
#include <time.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define RADIAN(x) 3.141592 / 180 * (x)

enum SHAPE { SOLID, WIRE };
enum VIEW {Perspective, Orthographic};

VIEW View = Perspective;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Change_Angle_xyz();
void Change_View(VIEW view);
void Draw_Coordinates();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Orbit {
public:
	float size = 200.f;

	void Draw() {
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_POINTS);
		for (float i = 0.f; i < 360.f; i += 0.5f) {
			float px = size * cos(RADIAN(i));
			float pz = size * sin(RADIAN(i));
			glVertex3f(px, 0, pz);
		}
		glEnd();
	}
};

class Sphere {
public:

	float size = 50;
	float radian = 0;
	SHAPE shape = SOLID;
	float R = rand() % 100 / 100.f, G = rand() % 100 / 100.f, B = rand() % 100 / 100.f;

	void Draw() {
		glPushMatrix();
		glColor3f(R, G, B);
		if (!shape) glutSolidSphere(size, 100.f, 100.f);
		else glutWireSphere(size, 20.f, 20.f);
		glPopMatrix();
	}
};

Angle Angle_x, Angle_y, Angle_z;

Orbit or_big_0, or_big_plus45, or_big_minus45;
Orbit or_small_0, or_small_plus45, or_small_minus45;

Sphere big;
Sphere middle_0, middle_plus45, middle_minus45;
Sphere small_0, small_plus45, small_minus45;

float move_x = 0, move_y = 0, move_z = 0;

void main(int argc, char **argv) {


	middle_0.size = 35;
	middle_plus45.size = 25;
	middle_minus45.size = 15;
	or_small_0.size = 75.f;
	or_small_plus45.size = 60.f;
	or_small_minus45.size = 35.f;
	small_0.size = 20.f;
	small_plus45.size = 10.f;
	small_minus45.size = 5.f;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("17");
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

	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}

	

	glPushMatrix();//우주-
	if(View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);
	Draw_Coordinates();

	big.Draw();
	or_big_0.Draw();//0도 - 궤도

	glPushMatrix();//0도-지구공전
	glRotatef(middle_0.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_big_0.size, 0.f, 0.f);
	middle_0.Draw();
	or_small_0.Draw();
	glPushMatrix();//0도-달공전
	glRotatef(small_0.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_small_0.size, 0.f, 0.f);
	small_0.Draw();
	glPopMatrix();//- 0도 - 달공전
	glPopMatrix();//- 0도 - 지구공전


	glRotatef(45.f, 0.f, 0.f, 1.f);//45도 - 궤도
	or_big_plus45.Draw();

	glPushMatrix();//45도 - 지구공전
	glRotatef(middle_plus45.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_big_plus45.size, 0.f, 0.f);
	middle_plus45.Draw();
	or_small_plus45.Draw();
	glPushMatrix();//45도-달공전
	glRotatef(small_plus45.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_small_plus45.size, 0.f, 0.f);
	small_plus45.Draw();
	glPopMatrix();//- 45도 - 달공전
	glPopMatrix();// -45도 - 지구공전



	glRotatef(-90.f, 0.f, 0.f, 1.f);// (- 45)도 - 궤도
	or_big_minus45.Draw();

	glPushMatrix();// (-45)도 - 지구 공전
	glRotatef(middle_minus45.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_big_minus45.size, 0.f, 0.f);
	middle_minus45.Draw();
	or_small_minus45.Draw();
	glPushMatrix();//(-45)도-달공전
	glRotatef(small_minus45.radian, 0.f, 1.f, 0.f);
	glTranslatef(or_small_minus45.size, 0.f, 0.f);
	small_minus45.Draw();
	glPopMatrix();//- (-45)도 - 달공전
	glPopMatrix();// - ( -45)도 - 지구 공전





	glPopMatrix();//-우주

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
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
	middle_0.radian += 3.f;
	middle_plus45.radian += 4.f;
	middle_minus45.radian += 5.f;
	small_0.radian += 4.f;
	small_plus45.radian += 5.f;
	small_minus45.radian += 3.f;
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
		move_x -= 10;
		break;

	case 'd': case 'D':
		move_x += 10;
		break;

	case 'w': case 'W':
		move_y += 10;
		break;

	case 's': case 'S':
		move_y -= 10;
		break;
	case '+': case '=':
		move_z += 10;
		break;

	case '-': case '_':
		move_z -= 10;
		break;

	case 'c':	case 'C':
		small_0.shape = small_plus45.shape = small_minus45.shape = middle_0.shape = middle_plus45.shape = middle_minus45.shape = big.shape = (SHAPE)((big.shape + 1) % 2);
		break;

	case 'i': case 'I':
		move_x = 0.f;
		move_y = 0.f;
		move_z = 0.f;
		Angle_x.radian = 0.f;
		Angle_y.radian = 0.f;
		Angle_z.radian = 0.f;
		break;

	case 'p':	case 'P':
		View = (VIEW)((View + 1) % 2);
		Reshape(800, 600);
		break;
	}
	glutPostRedisplay();
}

void Draw() {
	glutPostRedisplay();
}

void Change_Angle_xyz() {
	if (Angle_x.sw) Angle_x.radian += 2;
	if (Angle_y.sw) Angle_y.radian += 2;
	if (Angle_z.sw) Angle_z.radian += 2;
	glutPostRedisplay();
}

void Change_View(VIEW view) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!view) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, 1.0, 1.0, 10000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f, 0.f, 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}
	else if (view) {
		glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
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