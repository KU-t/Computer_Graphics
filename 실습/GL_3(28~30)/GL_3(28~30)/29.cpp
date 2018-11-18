#include <gl/freeglut.h>
#include <math.h>
#include <time.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define POINTS_SIZE 10
#define MAX_POINTS 18
#define deg(x) 3.141592*x/180
#define WAVE_HEGHT 100

enum VIEW { Perspective, Orthographic };
enum TYPE { UP = -1, DOWN = 1};

VIEW View = Orthographic;

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
void 	Draw_Lines();
void Draw_line(int first_point, TYPE type);
bool Collision_circle(float px, float py, float mx, float my);

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Point {
public:
	float x, y, z = 0;
	float radian = 0.f;
	float exist = false;

	void Draw() {
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor4f(0.f, 0.f, 1.f, 1.f);
		glutSolidSphere(POINTS_SIZE, 10.f, 10.f);
		glPopMatrix();
	}
};

Point *point[MAX_POINTS];
Angle Angle_x, Angle_y, Angle_z;
TYPE type = UP;
float z[3];
float z_radian = 0.f;
float move_x = 0, move_y = 0, move_z = 0;
int select_index;
bool select_point = false;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAX_POINTS; i++)	point[i] = NULL;
	for (int i = 0; i < 3; i++)	z[i] = rand() % 200 - 100.f;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("29");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 900.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	//Draw_bottom();

	/*glColor4f(1.f, 0.f, 0.f, 1.f);
	glutWireTeapot(100);*/
	for (int i = 0; i < MAX_POINTS; i++) {
		if (point[i])	point[i]->Draw();
	}

	Draw_Lines();
	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int mx = 2 * (x - 400);
		int my = -2 * (y - 300);

		for (int i = 0; i < MAX_POINTS; i++) {
			if (point[i]) {
				if (Collision_circle(point[i]->x, point[i]->y, mx, my)) {
					select_index = i;
					select_point = true;
				}
			}
		}

		if (!(select_point)) {
			for (int i = 0; i < MAX_POINTS; i++) {
				if (point[i] == NULL) {
					point[i] = new Point;
					point[i]->x = mx;
					point[i]->y = my;
					break;
				}
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) select_point = false;
	glutPostRedisplay();
}

void Motion(int x, int y) {
	if (select_point) {
		int mx = 2 * (x - 400);
		int my = -2 * (y - 300);

		point[select_index]->x = mx;
		point[select_index]->y = my;
	}
	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	glutPostRedisplay();
	for (int i = 0; i < MAX_POINTS; i++) {
		if (point[i]) {
			point[i]->radian += 10.f;
			if (point[i]->radian > 360.f)	point[i]->radian = 0.f;
			point[i]->z = WAVE_HEGHT * sin(deg(point[i]->radian));
		}
	}
	z_radian += 5.f;
	for (int i = 0; i < 3; i++) {
		z[i] += 10 * sin(deg(z_radian));
	}
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

	case 'r': case 'R':
		for (int i = 0; i < MAX_POINTS; i++) {
			point[i] = NULL;
		}
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
	glTranslatef(0.f, -150.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

void 	Draw_Lines() {
	glColor4f(1.f, 0.f, 0.f, 1.f);

	for (int i = 3; i < MAX_POINTS; i += 2) {
		if (point[i]) {
			if (type == UP)	type = DOWN;
			else if (type == DOWN)	type = UP;
			Draw_line(i - 3, type);
		}
	}
}

void Draw_line(int first_point, TYPE type) {
	float p0x = point[first_point]->x, p0y = point[first_point]->y, p0z = point[first_point]->z;
	float p1x = point[first_point + 1]->x, p1y = point[first_point + 1]->y, p1z = point[first_point + 1]->z;
	float p2x = point[first_point + 2]->x, p2y = point[first_point + 2]->y, p2z = point[first_point + 2]->z;
	float p3x = point[first_point + 3]->x, p3y = point[first_point + 3]->y, p3z = point[first_point + 3]->z;

	GLfloat ctrlpoints[3][3][3] = {
		{ { p0x,p0y,p0z },{ (p0x + p1x) / 2,(p0y + p1y) / 2,(p0z + p1z) / 2 },{ p1x,p1y,p1z } },
		{{ (p0x + p2x) / 2,(p0y + p2y) / 2, (z[0]) + (p0z + p2z) / 2 }, { ((p0x + p2x) / 2 + (p1x + p3x) / 2) / 2,((p0y + p2y) / 2 + (p1y + p3y) / 2) / 2, (z[1]) + ((p0z + p2z) / 2 + (p1z + p3z) / 2) / 2 }, { (p1x + p3x) / 2,(p1y + p3y) / 2, (z[2]) + (p1z + p3z) / 2 }},
		{ { p2x,p2y,p2z },{ (p2x + p3x) / 2,(p2y + p3y) / 2,(p2z + p3z) / 2 },{ p3x,p3y,p3z } }
	};

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);

	glMapGrid2f(10.0, 0.0, 1.0, 10.0, 0.f, 1.f);
	glEvalMesh2(GL_LINE, 0, 10, 0, 10);

	glDisable(GL_MAP1_VERTEX_3);

	glPointSize(5.0);
	
	//제어점 그리기
	/*glBegin(GL_POINTS);
	for (int i = 0; i < 3; i++) 
		for (int j = 0; j < 3; j++) 
			glVertex3fv(ctrlpoints[i][j]);
	glEnd();*/
}

bool Collision_circle(float px, float py, float mx, float my) {
	if (((px - mx) * (px - mx) + (py - my) * (py - my)) <= (POINTS_SIZE * POINTS_SIZE)) {
		return true;
	}
	return false;
}