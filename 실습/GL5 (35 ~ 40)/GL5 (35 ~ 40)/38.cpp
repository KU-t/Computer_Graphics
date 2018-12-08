#include <gl/freeglut.h>
#include <time.h>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800
#define WINDOW_SIZE_Z 800

#define TEAPOT_SIZE 100
#define CIRCLE_SIZE 100
#define CONE_SIZE 100

#define INIT_AHLPA 0.5

#define LIGHT_X 0
#define LIGHT_Y 100
#define LIGHT_Z 150

enum VIEW { Perspective, Orthographic };

VIEW View = Orthographic;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Change_Angle_xyz();
void Change_View(VIEW view);

class Teapot {
public:
	float x, y, z = 0.f;
	float ahlpa = INIT_AHLPA;

	void Draw() {
		glPushMatrix();
		glTranslatef(x - 50, y + 50, z);
		glColor4f(1.f, 0.f, 0.f, ahlpa);
		glutSolidTeapot(TEAPOT_SIZE);
		glPopMatrix();
	}
};

class Circle {
public:
	float x, y, z = -200.f;
	float ahlpa = INIT_AHLPA;

	void Draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor4f(0.f, 0.f, 1.f, ahlpa);
		glutSolidSphere(CIRCLE_SIZE, 100, 100);
		glPopMatrix();
	}
};

class Cone {
public:
	float x, y, z = -400.f;
	float ahlpa = INIT_AHLPA;

	void Draw() {
		glPushMatrix();
		glTranslatef(x + 50, y - 100, z);
		glRotatef(-90.f, 1.f, 0.f, 0.f);
		glColor4f(0.f, 1.f, 0.f, ahlpa);
		glutSolidCone(CONE_SIZE, CONE_SIZE * 3 / 2, 100, 100);
		glPopMatrix();
	}
};

class Rec {
public:
	float x, y, z = -800.f;

	void Draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor4f(0.5, 0.5, 0.5f, 1.f);
		glutSolidCube(300);
		glPopMatrix();
	}
};

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

Angle Angle_x, Angle_y, Angle_z;
float move_x = 0, move_y = 0, move_z = 0;

Teapot teapot[9];
Circle circle[9];
Cone cone[9];
Rec rectangle;

int choose_num = 0;

GLfloat AmbientLight0[] = { 1.f, 0.f, 0.f, 1.f }; // 빛의 세기 + 빛의 색
GLfloat DiffuseLight0[] = { 0.7f, 0.7f, 0.0f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 1.0, 0.0, 0.0, 1.0 }; // 하이라이트 색
GLfloat lightPos0[] = { LIGHT_X, LIGHT_Y, LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	for (int i = 0; i < 9; i++) {
		teapot[i].x = ((i % 3) - 1) * WINDOW_SIZE_X / 2;
		teapot[i].y = -(((i)/ 3) - 1) * WINDOW_SIZE_Y / 2;
		teapot[0].y = WINDOW_SIZE_Y / 2;
		circle[i].x = ((i % 3) - 1) * WINDOW_SIZE_X / 2;
		circle[i].y = -(((i) / 3) - 1) * WINDOW_SIZE_Y / 2;
		circle[0].y = WINDOW_SIZE_Y / 2;
		cone[i].x = ((i % 3) - 1) * WINDOW_SIZE_X / 2;
		cone[i].y = -(((i) / 3) - 1) * WINDOW_SIZE_Y / 2;
		cone[0].y = WINDOW_SIZE_Y / 2;
		/*teapot[i].ahlpa = (rand() % 100) * 0.01;
		circle[i].ahlpa = (rand() % 100) * 0.01;
		cone[i].ahlpa = (rand() % 100) * 0.01;*/
	}
	rectangle.x = teapot[0].x;
	rectangle.y = teapot[0].y;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow(" - ");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.7f, 0.7f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 400.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	//그리기
	for (int i = 0; i < 9; i++) {
		switch (i){
		case 0:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case 1:
			glBlendFunc(GL_ZERO, GL_ONE);
			break;
		case 2:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case 3:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case 4:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case 5:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case 6:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
		case 7:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
			break;
		case 8:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		cone[i].Draw();
		circle[i].Draw();
		teapot[i].Draw();
	}
	glDisable(GL_BLEND);
	rectangle.Draw();
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
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		choose_num = 0;
		rectangle.x = teapot[0].x;
		rectangle.y = teapot[0].y;
		break;
	case '2':
		choose_num = 1;
		rectangle.x = teapot[1].x;
		rectangle.y = teapot[1].y;
		break;
	case '3':
		choose_num = 2;
		rectangle.x = teapot[2].x;
		rectangle.y = teapot[2].y;
		break;
	case '4':
		choose_num = 3;
		rectangle.x = teapot[3].x;
		rectangle.y = teapot[3].y;
		break;
	case '5':
		choose_num = 4;
		rectangle.x = teapot[4].x;
		rectangle.y = teapot[4].y;
		break;
	case '6':
		choose_num = 5;
		rectangle.x = teapot[5].x;
		rectangle.y = teapot[5].y;
		break;
	case '7':
		choose_num = 6;
		rectangle.x = teapot[6].x;
		rectangle.y = teapot[6].y;
		break;
	case '8':
		choose_num = 7;
		rectangle.x = teapot[7].x;
		rectangle.y = teapot[7].y;
		break;
	case '9':
		choose_num = 8;
		rectangle.x = teapot[8].x;
		rectangle.y = teapot[8].y;
		break;

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

	case 'h': case 'H':
		if (0.f < teapot[choose_num].ahlpa)
		teapot[choose_num].ahlpa -= 0.05;
		break;
	case 'j': case 'J':
		if ( teapot[choose_num].ahlpa < 1.f)
		teapot[choose_num].ahlpa += 0.05;
		break;
	case 'k': case 'K':
		if (0.f < circle[choose_num].ahlpa)
		circle[choose_num].ahlpa -= 0.05;
		break;
	case 'l': case 'L':
		if ( circle[choose_num].ahlpa < 1.f)
		circle[choose_num].ahlpa += 0.05;
		break;
	case 'n': case 'N':
		if (0.f < cone[choose_num].ahlpa)
		cone[choose_num].ahlpa -= 0.05;
		break;
	case 'm': case 'M':
		if ( cone[choose_num].ahlpa < 1.f)
		cone[choose_num].ahlpa += 0.05;
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
		glOrtho(-WINDOW_SIZE_X, WINDOW_SIZE_X, -WINDOW_SIZE_Y, WINDOW_SIZE_Y, -WINDOW_SIZE_Z, WINDOW_SIZE_Z);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}
