#include <gl/freeglut.h>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_SIZE_Z 600
#define MAX_LIGHT 2

enum VIEW { Perspective, Orthographic };

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

//GL_LIGHT0
bool SWITCH_Light0 = false;
GLfloat AmbientLight0[] = { 0.f, 1.f, 0.f, 1.f };
GLfloat DiffuseLight0[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 1.0, 1.0, 1.0, 1.0 }; // 백색조명
GLfloat lightPos0[] = { 0.0, 0.0, 300.0, 1.0 }; // 위치: (10, 5, 20) 

//GL_LIGHT1
bool SWITCH_Light1 = false;
GLfloat AmbientLight1[] = { 1.f, 0.f, 0.f, 1.f };
GLfloat DiffuseLight1[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // 광원 색
GLfloat SpecularLight1[] = { 1.0, 1.0, 1.0, 1.0 }; // 백색조명
GLfloat lightPos1[] = { 0.0, 0.0, -300.0, 1.0 }; // 위치: (10, 5, 20) 

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow(" - ");
	
	GLfloat ambientLight[] = { 1.f, 1.f, 1.f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

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
	glEnable(GL_LIGHTING);
	glEnable(GL_AUTO_NORMAL);

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

	// LIGHT0
	{	
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	if(SWITCH_Light0)		glEnable(GL_LIGHT0);
	else glDisable(GL_LIGHT0);
	
	}
	
	// LIGHT1
	{	
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	if(SWITCH_Light1)		glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);

	}

	GLfloat green[] = { 0.f, 0.3f, 0.f, 1.f };
	GLfloat specref[] = { 1.f, 0.f, 1.f, 1.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glutSolidSphere(100.f, 100, 100);

	//빛 제거
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	//Draw_LIGHT0 
	glPushMatrix();
	glTranslatef(0.f, 0.f, 300.f);
	glColor4f(1.f, 0.f, 0.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();
	
	//Draw_LIGHT1
	glPushMatrix();
	glTranslatef(0.f, 0.f, -300.f);
	glRotatef(180.f, 1.f, 0.f, 0.f);
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();

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
		SWITCH_Light0 = (SWITCH_Light0 + 1) % 2;
		break;
	case '2':
		SWITCH_Light1 = (SWITCH_Light1 + 1) % 2;
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

	case '[':	
		DiffuseLight0[0] -= 0.05f;
		DiffuseLight1[2] -= 0.05f;
		break;
	case']':
		DiffuseLight0[0] += 0.05f;
		DiffuseLight1[2] += 0.05f;
		break;

	case ',':		
		SpecularLight0[0] -= 0.05f;
		SpecularLight0[1] -= 0.05f;
		SpecularLight0[2] -= 0.05f;
		SpecularLight1[0] -= 0.05f;
		SpecularLight1[1] -= 0.05f;
		SpecularLight1[2] -= 0.05f;
		break;
	case '.':
		SpecularLight0[0] += 0.05f;
		SpecularLight0[1] += 0.05f;
		SpecularLight0[2] += 0.05f;
		SpecularLight1[0] += 0.05f;
		SpecularLight1[1] += 0.05f;
		SpecularLight1[2] += 0.05f;
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
	glTranslatef(0.f, -150.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}