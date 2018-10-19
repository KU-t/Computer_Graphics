#include <gl/freeglut.h>

#define W_x 800
#define W_y 600
#define W_z 600

enum VIEW { Perspective, Orthographic };
enum DIRECTION { Left, Right };
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
void Draw_bottom();
void Move_arm_1();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Arm {
public:
	DIRECTION direction = Left;
	float x = 0;
	float radian_x = 0, radian_y = 0;
	bool spin_x_sw = false, spin_y_sw = false;
	float size_x = 0, size_y = 0, size_z = 0;

	Arm(){}

	Arm(float x, float y, float z) : size_x(x),size_y(y),size_z(z){}

	void Draw(float bottom) {
		glTranslatef(0, bottom, 0);
		glRotatef(radian_x, 1.f, 0.f, 0.f);
		glRotatef(radian_y, 0.f, 1.f, 0.f);

		glPushMatrix();
		glTranslatef(0.f, size_y / 2, 0.f);
		glScalef(size_x, size_y, size_z);
		glutSolidCube(1);
		glScalef((size_x + 1) / size_x, (size_y + 1) / size_y, (size_z + 1) / size_z);
		glColor4f(0.f, 0.f, 0.f, 0.f);
		glutWireCube(1);
		glPopMatrix();
	}
};

Angle Angle_x, Angle_y, Angle_z;
float move_x = 0, move_y = 0, move_z = 0;
Arm arm_1(100.f, 50.f, 100.f);
Arm arm_2(50.f, 150.f, 50.f);
Arm arm_3(30.f, 150.f, 30.f);

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("11");
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

	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	Draw_bottom();

	glPushMatrix(); // arm_1 push
	glTranslatef(arm_1.x, 0.f, 0.f);
	glColor4f(1.f, 0.f, 0.f, 0.f);
	arm_1.Draw(0.f);
	

	glPushMatrix(); // arm_2 push
	glColor4f(0.f, 1.f, 0.f, 1.f);
	arm_2.Draw(arm_1.size_y);
	

	glPushMatrix(); // arm_3 push
	glColor4f(0.f, 0.f, 1.f, 1.f);
	arm_3.Draw(arm_2.size_y);

	glPopMatrix(); // arm_1 pop

	glPopMatrix(); // arm_2 pop
	
	glPopMatrix(); // arm_3 pop

	glPopMatrix();

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
	Move_arm_1();
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

	case 'f': case 'F':
		if(arm_2.radian_x < 90.f)	arm_2.radian_x += 3.f;
		break;

	case 'v': case 'V':
		if (arm_2.radian_x > -90.f)	arm_2.radian_x -= 3.f;
		break;

	case 'g': case 'G':
		if (arm_2.radian_y < 90.f) arm_2.radian_y += 3.f;
		break;

	case 'b': case 'B':
		if (arm_2.radian_y > -90.f)	arm_2.radian_y -= 3.f;
		break;

	case 'h': case 'H':
		if (arm_3.radian_x < 90.f)	arm_3.radian_x += 3.f;
		break;

	case 'n': case 'N':
		if (arm_3.radian_x > -90.f)	arm_3.radian_x -= 3.f;
		break;

	case 'j': case 'J':
		if (arm_3.radian_y < 90.f)	arm_3.radian_y += 3.f;
		break;

	case 'm': case 'M':
		if (arm_3.radian_y > -90.f)	arm_3.radian_y -= 3.f;
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
	glScalef(1.f, 25.f, 1.f);
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
	glScalef(1.f, 1.f, 25.f);
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
	glTranslatef(0.f, -arm_1.size_y/2, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

void Move_arm_1() {
	if (arm_1.direction == Left) {
		if (arm_1.x > -250.f + (float)(arm_1.size_x / 2))	arm_1.x -= 5.f;
		else arm_1.direction = Right;
	}
	else if (arm_1.direction == Right) {
		if (arm_1.x < 250.f - (float)(arm_1.size_x / 2))	arm_1.x += 5.f;
		else arm_1.direction = Left;
	}
}