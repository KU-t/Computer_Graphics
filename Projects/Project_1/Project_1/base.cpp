#include <gl/freeglut.h>
#include <time.h>
#include <math.h>

#define W_x 800
#define W_y 800
#define MAX_TRIANGLES 20
#define MAX_RECTANGLES 100

enum VIEW { Perspective, Orthographic };
enum TRIANGLE_TYPE { E, W, S, N };
enum RECTANGLE_TYPE { CLICK_ON, CLICK_OFF };
enum CLICK { ON, OFF };

VIEW View = Perspective;
float tri_y = 350.f,rec_x = 0.f, size = 30.f;
int tri_Count = 0;
int rec_Count = 0;
float mx, my;

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
void Check_Triangle();
void Check_Rectangle();
void 	Draw_Cell();
void Draw_CHECK_Mouse_Line();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Triangles {
public:
	float x;
	bool exist = false;
	TRIANGLE_TYPE type;

	void Draw() {
		if (exist) {
			glBegin(GL_POLYGON);
			glColor4f(1.f, 1.0f, 0.f, 1.f);
			if (!(type == E))	glVertex3f(x + size, tri_y + size, -100.f);
			if (!(type == W))	glVertex3f(x + size, tri_y - size, -100.f);
			if (!(type == S))	glVertex3f(x - size, tri_y - size, -100.f);
			if (!(type == N))	glVertex3f(x - size, tri_y + size, -100.f);
			glEnd();
		}
	}
};
void Init_Triangles(Triangles);


class Rectangles {
public:
	float y;
	bool exist = false;
	RECTANGLE_TYPE type = CLICK_ON;

	void Draw() {
		if (exist) {
			glBegin(GL_POLYGON);
			if(type == CLICK_ON)	glColor4f(1.f, 1.0f, 0.f, 1.f);
			if (type == CLICK_OFF)	glColor4f(1.f, 0.0f, 0.f, 1.f);
			glVertex3f(rec_x + size, y + size, -100.f);
			glVertex3f(rec_x + size, y - size, -100.f);
			glVertex3f(rec_x - size, y - size, -100.f);
			glVertex3f(rec_x - size, y + size, -100.f);
			glEnd();
		}
	}
};

class Mouse_Click {
public:
	float x, y, click_x, click_y;
	CLICK click = OFF;
};

Angle Angle_x, Angle_y, Angle_z;
Triangles triangle[MAX_TRIANGLES];
Rectangles rectangle[MAX_RECTANGLES];
Mouse_Click mouse;

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("저별내별");
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


	glBegin(GL_POINTS);
	glColor4f(0.7f, 0.7f, 0.f, 1.f);
	for (float i = -400.f; i < 400.f; i+=20.f) {
		glVertex3f(i , 300.f, -100.f);
	}
	glEnd();

	for (int i = 0; i < MAX_TRIANGLES; i++) {
		triangle[i].Draw();
	}
	for (int i = 0; i < MAX_RECTANGLES; i++) {
		rectangle[i].Draw();
	}

	Draw_Cell();
	Draw_CHECK_Mouse_Line();
	glPushMatrix();

	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	//Draw_bottom();

	glColor4f(1.f, 0.f, 0.f, 1.f);
	//glutWireTeapot(100);

	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse.click = ON;
		mouse.click_x = (x - 400);
		mouse.click_y = -( y - 400);
		mouse.x = (x - 400);
		mouse.y = -(y - 400);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouse.click = OFF;
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {
	mouse.x = (x - 400);
	mouse.y = -(y - 400);
	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	Check_Triangle();
	Check_Rectangle();
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

	case 'i': case 'I':
		Angle_x.radian = 0.f;
		Angle_y.radian = 0.f;
		Angle_z.radian = 0.f;
		break;

	}
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
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, 1.0, 1.0, 10000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f, 0.f, 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
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

void Check_Triangle() {
	if (tri_Count == 0) {
		for (int i = 0; i < MAX_TRIANGLES; i++) {
			if (!(triangle[i].exist)) {
				triangle[i].exist = true;
				triangle[i].x = -400.f - size;
				triangle[i].type = (TRIANGLE_TYPE)(rand() % 4);
				break;
			}
		}
	}
	for (int i = 0; i < MAX_TRIANGLES; i++) {
		if (triangle[i].exist) {
			triangle[i].x += 2;
			if (triangle[i].x - size >= 400.f) {
				triangle[i].exist = false;
			}
		}
	}
	tri_Count = (tri_Count + 1) % 50;
}

void Check_Rectangle() {
	if (rec_Count == 0) {
		for (int i = 0; i < MAX_RECTANGLES; i++) {
			if (!(rectangle[i].exist)) {
				rectangle[i].exist = true;
				rectangle[i].y = -400.f - size;
				rectangle[i].type = CLICK_ON;
				break;
			}
		}
	}
	for (int i = 0; i < MAX_RECTANGLES; i++) {
		if (rectangle[i].exist) {
			rectangle[i].y += 2;
			if (rectangle[i].type == CLICK_ON) {
				if (rectangle[i].y + size >= 300.f) {
					rectangle[i].type = CLICK_OFF;
				}
			}
			else if (rectangle[i].type == CLICK_OFF) {
				if (rectangle[i].y - size >= 400.f)
					rectangle[i].exist = false;
			}
		}
	}
	rec_Count = (rec_Count + 1) % 100;
}


void Init_Triangles(Triangles t) {
	t.exist = true;
	t.x = -400.f - size;
	t.type = (TRIANGLE_TYPE)(rand() % 4);
}

void 	Draw_Cell() {
	glColor4f(0.7f, 0.7f, 0.f, 0.5f);
	for (float i = 0-size * 4; i > -400.f; i -= size * 2) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(i, -400.f + 4 * size, -100.f);
		glVertex3f(i, -400.f, -100.f);
		for (float j = 0.f; j < 3; j++) {
			glVertex3f(i + size * 2, -400.f + j * size * 2, -100.f);
			glVertex3f(i, -400.f + j * size * 2, -100.f);
		}
		glVertex3f(i + size * 2, -400.f + 4 * size, -100.f);
		glVertex3f(i + size * 2, -400.f, -100.f);
		glEnd();
	}

	for (float i = size * 2; i < 400.f - size * 2; i += size * 2) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(i, -400.f + 4 * size, -100.f);
		glVertex3f(i, -400.f, -100.f);
		for (float j = 0.f; j < 3; j++) {
			glVertex3f(i + size * 2, -400.f + j * size * 2, -100.f);
			glVertex3f(i, -400.f + j * size * 2, -100.f);
		}
		glVertex3f(i + size * 2, -400.f + 4 * size, -100.f);
		glVertex3f(i + size * 2, -400.f, -100.f);
		glEnd();
	}

	

	glBegin(GL_LINES);
	glColor4f(1.f, 0.f, 0.f, 0.5f);
	glVertex3f(0.f, -400.f, -100.f);
	glVertex3f(0.f, -325.f, -100.f);
	glEnd();
}

void Draw_CHECK_Mouse_Line() {
	if (mouse.click == ON) {
		glBegin(GL_LINES);
		glColor4f(1.f, 0.f, 0.f, 1.f);
		glVertex3f(mouse.click_x, mouse.click_y, -100.f);
		glVertex3f(mouse.x, mouse.y, -100.f);
		glEnd();
	}
	else if (mouse.click == OFF) {
		float mx = mouse.click_x - mouse.x;
		float my = mouse.click_y - mouse.y;
		for (int i = 0; i < MAX_RECTANGLES; i++) {
			if (rectangle[i].exist && rectangle[i].type == CLICK_ON) {
				float d = (float)abs(-(long)rectangle[i].y + (long)(mx*mouse.click_y / my) - (long)mouse.click_x) / (float)sqrt(my*my / (mx*mx) + 1);
				if (d <= 10.f) {
					rectangle[i].type = CLICK_OFF;
				}
			}
		}
	}
}
