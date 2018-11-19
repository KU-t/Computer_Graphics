#include <gl/freeglut.h>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_SIZE_Z 600
#define MAX_LIGHT 2
#define LIGHT_X 0
#define LIGHT_Y 100
#define LIGHT_Z 150
#define MAX_SNOW 50
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

class Snow {
public:
	int i, j;
	float y;
	Snow() {
		i = rand() % 50;
		j = rand() % 50;
		y = rand() % 300 + 200.f;
	}
	void Draw() {
		glPushMatrix();
		glTranslatef(-250.f + (10 * i), y, -250.f + (10 * j));
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glutSolidSphere(10.f, 20, 20);
		glPopMatrix();
	}
};
Snow *snow[MAX_SNOW];

float move_x = 0, move_y = 0, move_z = 0;

bool SWITCH_snowing = false;

float light_rad = 0.f;

//GL_LIGHT0
bool SWITCH_Light0 = false;
GLfloat AmbientLight0[] = { 1.f, 0.f, 0.f, 1.f }; // 빛의 세기 + 빛의 색
GLfloat DiffuseLight0[] = { 0.7f, 0.7f, 0.0f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 1.0, 0.0, 0.0, 1.0 }; // 하이라이트 색
GLfloat lightPos0[] = { LIGHT_X, LIGHT_Y, LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 

														  //GL_LIGHT1
bool SWITCH_Light1 = false;
GLfloat AmbientLight1[] = { 0.f, 0.f, 1.f, 1.f }; // 빛의 세기 + 빛의색
GLfloat DiffuseLight1[] = { 0.0f, 0.7f, 0.7f, 1.0f }; // 광원 색
GLfloat SpecularLight1[] = { 0.0, 0.0, 1.0, 1.0 }; // 하이라이트색
GLfloat lightPos1[] = { LIGHT_X, LIGHT_Y, -LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 


GLfloat block_specref[] = { 0.1f, 0.1f, 0.1f, 1.f };

float bolck_bright[50][50][4] = {};

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	GLfloat block[] = { 0.1f, 0.1f, 0.1f, 1.f };
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			for (int k = 0; k < 4; k++) {
				bolck_bright[i][j][k] = block[k];
			}
		}
	}
	for (int i = 0; i < MAX_SNOW; i++) snow[i] = NULL;
	
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_AUTO_NORMAL);
	//glShadeModel(GL_SMOOTH);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

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


	glPushMatrix();
	glRotated(light_rad, 0.f, 1.f, 0.f);
	// LIGHT0
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		if (SWITCH_Light0)		glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);

	}

	// LIGHT1
	{
		glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		if (SWITCH_Light1)		glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);

	}
	glPopMatrix();


	//glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	GLfloat Object0[] = { 0.1f, 0.5f, 0.1f, 1.f };
	GLfloat Object_specref0[] = { 0.3f, 0.5f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref0);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(50.f, 100, 100, 100);
	glPopMatrix();

	glPushMatrix();
	GLfloat Object1[] = { 0.5f, 0.1f, 0.1f, 1.f };
	GLfloat Object_specref1[] = { 0.5f, 0.3f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(200.f, 0, 200.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(50.f, 100, 100, 100);
	glPopMatrix();

	glPushMatrix();
	GLfloat Object2[] = { 0.1f, 0.1f, 0.5f, 1.f };
	GLfloat Object_specref2[] = { 0.5f, 0.5f, 0.5f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref2);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(200.f, 0, -200.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(50.f, 100, 100, 100);
	glPopMatrix();

	glPushMatrix();
	GLfloat Object3[] = { 0.5f, 0.5f, 0.f, 1.f };
	GLfloat Object_specref3[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref3);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(-200.f, 0, 200.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(50.f, 100, 100, 100);
	glPopMatrix();

	glPushMatrix();
	GLfloat Object4[] = { 0.f, 0.5f, 0.5f, 1.f };
	GLfloat Object_specref4[] = { 0.f, 0.5f, 0.5f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref4);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(-200.f, 0, -200.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidCone(50.f, 100, 100, 100);
	glPopMatrix();


	Draw_bottom();

	//빛 제거
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	for (int i = 0; i < MAX_SNOW; i++) {
		if (snow[i])	snow[i]->Draw();
	}

	glPushMatrix();
	glRotated(light_rad, 0.f, 1.f, 0.f);
	//Draw_LIGHT0 
	glPushMatrix();
	glTranslatef(LIGHT_X, LIGHT_Y, LIGHT_Z);
	glColor4f(1.f, 0.f, 0.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();

	//Draw_LIGHT1
	glPushMatrix();
	glTranslatef(LIGHT_X, LIGHT_Y, -LIGHT_Z);
	glRotatef(180.f, 1.f, 0.f, 0.f);
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();


	glPopMatrix();

	/*GLfloat bottom[] = { 0.3f, 0.3f, 0.f, 1.f };
	GLfloat bottom_specref[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bottom);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bottom_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);*/
	

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
	light_rad += 3.f;
	if (SWITCH_snowing) {
		for (int i = 0; i < MAX_SNOW; i++) {
			if (snow[i] == NULL)	snow[i] = new Snow;
			if (snow[i]) {
				snow[i]->y-=10.f;
				if (snow[i]->y <= 0.f) {
					for(int j = 0; j<3;j++)	bolck_bright[snow[i]->i][snow[i]->j][j] += 1.f;
					snow[i] = NULL;
				}
			}
		}
	}
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
		for (int i = 0; i < 3; i++) {
			DiffuseLight0[i] -= 0.05f;
			DiffuseLight1[i] -= 0.05f;
		}
		break;
	case ']':
		for (int i = 0; i < 3; i++) {
			DiffuseLight0[i] += 0.05f;
			DiffuseLight1[i] += 0.05f;
		}
		break;

	case 'n':	case 'N':
		for (int i = 0; i < 3; i++) {
			AmbientLight0[i] -= 0.05f;
			AmbientLight1[i] -= 0.05f;
		}
		break;
	case 'm':	case 'M':
		for (int i = 0; i < 3; i++) {
			AmbientLight0[i] += 0.05f;
			AmbientLight1[i] += 0.05f;
		}
		break;

	case ',':
		for (int i = 0; i < 3; i++) {
			SpecularLight0[i] -= 0.05f;
			SpecularLight1[i] -= 0.05f;
		}
		break;
	case '.':
		for (int i = 0; i < 3; i++) {
			SpecularLight0[i] += 0.05f;
			SpecularLight1[i] += 0.05f;
		}
		break;
	case 'c': case 'C':
		SWITCH_snowing = (SWITCH_snowing + 1) % 2;
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
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bolck_bright[i][j]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, block_specref);
			glMateriali(GL_FRONT, GL_SHININESS, 64);
			glTranslatef(-250.f + (10 * i), 0.f, -250.f + (10 * j));
			glBegin(GL_QUADS);
			glVertex4f(-10.f, 0.f, -10.f,1.f);
			glVertex4f(10.f, 0.f, -10.f, 1.f);
			glVertex4f(10.f, 0.f, 10.f, 1.f);
			glVertex4f(-10.f, 0.f, 10.f, 1.f);
			glEnd();
			glPopMatrix();

		}
	}
	
	glPopMatrix();
}