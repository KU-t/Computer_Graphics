#include <gl/freeglut.h>
#include <time.h>
#include <math.h>

#define W_x 800
#define W_y 800
#define W_z 800
#define MAX_TRIANGLES 20
#define MAX_RECTANGLES 100
#define MAX_CELL_NUM 7
#define MAX_CELL_AMOUNT 84
#define GO_SLICE 100

enum VIEW { Perspective, Orthographic };
enum TRIANGLE_TYPE { E, W, S, N };
enum RECTANGLE_TYPE { CLICK_ON, CLICK_OFF };
enum CLICK { ON, OFF, END, WAIT, PICK };
enum TYPE { PARTS_1, PARTS_2 };

VIEW View = Perspective;
float tri_y = 700.f, rec_x = 0.f, size = 50.f;
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
void Draw_Mouse_Line();
void CHECK_Mouse_Line();
void CHECK_PARTS(int num, float l);

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
	float x, y;
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
	float del_color = 1.f;
	RECTANGLE_TYPE type = CLICK_ON;

	void Draw() {
		if (exist) {
			glBegin(GL_POLYGON);
			if (type == CLICK_ON)	glColor4f(1.f, 1.0f, 0.f, 1.f);
			if (type == CLICK_OFF)	glColor4f(1.f, 0.0f, 0.f, 1.f);
			glVertex3f(rec_x + size, y + size, -100.f);
			glVertex3f(rec_x + size, y - size, -100.f);
			glVertex3f(rec_x - size, y - size, -100.f);
			glVertex3f(rec_x - size, y + size, -100.f);
			glEnd();
		}
	}
};

class LCell {
public:
	bool exist[6] = { false };
	float x, y = -800.f;
	Point p[6] = {
		0.f,-800.f + size,
		0.f,-800.f + size,
		0.f,-800.f + 3 * size,
		0.f,-800.f + 3 * size,
		0.f,-800.f + 5 * size,
		0.f,-800.f + 5 * size
	};

	void Draw() {
		// 셀마다의 대각선 가로선
		glColor4f(0.7f, 0.7f, 0.f, 0.5f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(x - size, y, 0.f);
		glVertex3f(x + size, y + 2 * size, 0.f);
		glVertex3f(x - size, y + 2 * size, 0.f);
		glVertex3f(x + size, y + 2 * 2 * size, 0.f);
		glVertex3f(x - size, y + 2 * 2 * size, 0.f);
		glVertex3f(x + size, y + 3 * 2 * size, 0.f);
		glEnd();
	}
};

class RCell {
public:
	bool exist[6] = { false };
	float x, y = -800.f;
	Point p[6] = {
		0.f,-800.f + size,
		0.f,-800.f + size,
		0.f,-800.f + 3 * size,
		0.f,-800.f + 3 * size,
		0.f,-800.f + 5 * size,
		0.f,-800.f + 5 * size
	};

	void Draw() {
		// 셀마다의 대각선 가로선
		glColor4f(0.7f, 0.7f, 0.f, 0.5f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(x + size, y, 0.f);
		glVertex3f(x - size, y + 2 * size, 0.f);
		glVertex3f(x + size, y + 2 * size, 0.f);
		glVertex3f(x - size, y + 2 * 2 * size, 0.f);
		glVertex3f(x + size, y + 2 * 2 * size, 0.f);
		glVertex3f(x - size, y + 3 * 2 * size, 0.f);
		glEnd();
	}
};

class Parts {
public:
	float x, y;
	float vel = 5.f;
	float radian;
	float copy_x, copy_y;
	float copy_radian;
	float init_y, init_radian;
	float go_x, go_y, go_radian;

	CLICK click = WAIT;
	TYPE type;

	void update() {
		if (click == END) {
			x += go_x;
			y += go_y;
			radian += go_radian;
			if ((copy_y - 0.01f <= y) && (y <= copy_y + 0.01f))	click = PICK;
		}

		else if (click == OFF) {
			vel -= 0.1f;
			y += vel;
			radian = (radian + 3.f);

			if (y <= -800.f + 7 * size) {
				go_x = (copy_x - x) / GO_SLICE;
				go_y = (copy_y - y) / GO_SLICE;
				go_radian = (copy_radian - radian) / GO_SLICE;
				click = END;
			}

			if (type == PARTS_1)	x -= 3.f;
			else if (type == PARTS_2)	x += 3.f;
		}

	}

	void Draw() {
		if ((click == OFF) || (click == END) || (click == PICK)) {
			glPushMatrix();
			glColor4f(1.f, 1.f, 0.f, 1.f);
			glTranslatef(x, y, 0.f);
			glRotatef(radian, 0.f, 0.f, 1.f);
			glBegin(GL_POLYGON);
			glVertex3f(-size, -size, 0.f);
			glVertex3f(+size, -size, 0.f);
			glVertex3f(+size, +size, 0.f);
			glEnd();
			glPopMatrix();
		}
	}
};

class Mouse_Click {
public:
	float x, y, click_x, click_y;
	CLICK click = OFF;
};

float move_x = 0, move_y = 0, move_z = 0;

Angle Angle_x, Angle_y, Angle_z;
Triangles triangle[MAX_TRIANGLES];
Rectangles rectangle[MAX_RECTANGLES];
LCell Lcells[MAX_CELL_NUM];
RCell Rcells[MAX_CELL_NUM];
Parts Lparts[MAX_CELL_AMOUNT];
Parts Rparts[MAX_CELL_AMOUNT];
Mouse_Click mouse;

void main(int argc, char **argv) {
	//init
	for (int i = 0; i < MAX_CELL_NUM; i++) {
		Lcells[i].x = -800.f + ((i * 2) + 1) * size;
		for (int j = 0; j < 6; j++) {
			Lcells[i].p[j].x = -800.f + ((i * 2) + 1) * size;
		}

		Rcells[i].x = 0.f + size + (i + 1) * 2 * size;
		for (int j = 0; j < 6; j++) {
			Rcells[i].p[j].x = 0.f + size + (i + 1) * 2 * size;
		}
	}
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

	// 직각투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_POINTS);
	glColor4f(0.7f, 0.7f, 0.f, 1.f);


	for (float i = -800.f; i < 800.f; i += 20.f) {
		glVertex3f(i, 600.f, 0.f);
	}
	glEnd();

	Draw_Cell();

	for (int i = 0; i < MAX_CELL_NUM; i++) {
		Lcells[i].Draw();
		Rcells[i].Draw();
	}

	for (int i = 0; i < MAX_TRIANGLES; i++) {
		triangle[i].Draw();
	}

	for (int i = 0; i < MAX_CELL_AMOUNT; i++) {
		Lparts[i].Draw();
		Rparts[i].Draw();
	}

	for (int i = 0; i < MAX_RECTANGLES; i++) {
		rectangle[i].Draw();
	}


	Draw_Mouse_Line();


	glPushMatrix();

	// 원근투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(move_x, move_y + 300.f, move_z + 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);


	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);



	Draw_Coordinates();
	//Draw_bottom();




	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
}

void Mouse(int button, int state, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse.click = ON;
		mouse.click_x = 2 * (x - 400);
		mouse.click_y = -2 * (y - 400);
		mouse.x = 2 * (x - 400);
		mouse.y = -2 * (y - 400);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouse.click = OFF;
		CHECK_Mouse_Line();
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	mouse.x = 2 * (x - 400);
	mouse.y = -2 * (y - 400);

	/*for (int i = 0; i < MAX_CELL_AMOUNT; i++) {
		if (Lparts[i].click == OFF) {
			glPushMatrix();

		}
	}*/

	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	for (int i = 0; i < MAX_CELL_AMOUNT; i++) {
		if (Lparts[i].click == OFF || Lparts[i].click == END)
			Lparts[i].update();

		if (Rparts[i].click == OFF || Rparts[i].click == END)
			Rparts[i].update();
	}


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
				//init
				triangle[i].exist = true;
				triangle[i].x = -800.f - size;
				triangle[i].type = (TRIANGLE_TYPE)(rand() % 4);
				break;
			}
		}
	}
	for (int i = 0; i < MAX_TRIANGLES; i++) {
		if (triangle[i].exist) {
			//del - 화면 밖
			triangle[i].x += 2;
			if (triangle[i].x >= 800.f + size) {
				triangle[i].exist = false;
			}
		}
	}
	tri_Count = (tri_Count + 1) % 100;
}

void Check_Rectangle() {
	if (rec_Count == 0) {
		for (int i = 0; i < MAX_RECTANGLES; i++) {
			if (!(rectangle[i].exist)) {
				//init
				rectangle[i].exist = true;
				rectangle[i].y = -800.f - size;
				rectangle[i].type = CLICK_ON;
				break;
			}
		}
	}
	for (int i = 0; i < MAX_RECTANGLES; i++) {
		if (rectangle[i].exist) {
			rectangle[i].y += 20;
			if (rectangle[i].type == CLICK_ON) {
				// 선위로 올라가면 (색바뀜)
				if (rectangle[i].y + size >= 600.f) {
					rectangle[i].type = CLICK_OFF;
				}
			}
			else if (rectangle[i].type == CLICK_OFF) {
				// del - 화면 밖
				if (rectangle[i].y - size >= 800.f)
					rectangle[i].exist = false;
			}
		}
	}
	rec_Count = (rec_Count + 1) % 25;
}

void 	Draw_Cell() {
	glColor4f(0.7f, 0.7f, 0.f, 0.5f);

	glBegin(GL_LINES);
	// Cell 맨위에 선
	glVertex3f(-800.f, -800.f + 3 * 2 * size, 0.f);
	glVertex3f(800.f, -800.f + 3 * 2 * size, 0.f);

	//Cell마다 가로줄
	for (float j = 0.f; j < 8.f; j++) {
		glVertex3f(-800.f + j * 2 * size, -800.f, 0.f);
		glVertex3f(-800.f + j * 2 * size, -800.f + 3 * 2 * size, 0.f);
		glVertex3f((j + 1) * 2 * size, -800.f, 0.f);
		glVertex3f((j + 1) * 2 * size, -800.f + 3 * 2 * size, 0.f);
	}
	glEnd();

	glBegin(GL_LINES);
	//빨간 유도선
	glColor4f(1.f, 0.f, 0.f, 0.5f);
	glVertex3f(0.f, -800.f, 0.f);
	glVertex3f(0.f, -700.f, 0.f);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(0.5f, 0.5f, 0.f, 0.f);
	glVertex3f(-800.f, -800.f + 6 * size, -50.f);
	glVertex3f(-800.f, -800.f + 8 * size, -50.f);
	glVertex3f(800.f, -800.f + 8 * size, -50.f);
	glVertex3f(800.f, -800.f + 6 * size, -50.f);
	glEnd();
}

void Draw_Mouse_Line() {
	if (mouse.click == ON) {
		glBegin(GL_LINES);
		glColor4f(1.f, 0.f, 0.f, 1.f);
		glVertex3f(mouse.click_x, mouse.click_y, 0.f);
		glVertex3f(mouse.x, mouse.y, 0.f);
		glEnd();
	}
}

void CHECK_Mouse_Line() {
	float mx = mouse.click_x - mouse.x;
	float my = mouse.click_y - mouse.y;
	float l = my / mx;
	if ((0.6f <= l && l <= 1.4f) || (-1.4f <= l && l <= -0.6f)) {
		for (int i = 0; i < MAX_RECTANGLES; i++) {
			if ((rectangle[i].y) && (rectangle[i].y)) {

				if (rectangle[i].exist && rectangle[i].type == CLICK_ON) {
					float d = (-(float)rectangle[i].y + (float)(mouse.y) - (float)(l*mouse.x)) / (float)sqrt((l*l) + 1);
					if (-10.f <= d && d <= 10.f) {
						rectangle[i].exist = false; // exist로
						CHECK_PARTS(i, l);
					}
				}
			}
		}
	}
}

void CHECK_PARTS(int num, float l) {
	for (int p = 0; p < MAX_CELL_AMOUNT; p++) {
		if (Lparts[p].click == WAIT) {
			Lparts[p].click = OFF;
			for (int j = 0; j < 6; j++) {
				bool sw = false;
				for (int i = MAX_CELL_NUM - 1; i >= 0; i--) {
					if (Lcells[i].exist[j] == false) {
						Lcells[i].exist[j] = true;
						Lparts[p].radian = 180.f;
						if (l < 0.f)	Lparts[p].radian = 270.f;
						Lparts[p].copy_x = Lcells[i].p[j].x + 2.f;
						Lparts[p].copy_y = Lcells[i].p[j].y;
						Lparts[p].y = rectangle[num].y;
						Lparts[p].type = PARTS_1;
						if (j % 2 == 1)	Lparts[p].copy_radian = 180.f;
						sw = true;
						break;
					}
					if (sw)	break;
				}
				if (sw)	break;
			}
			break;
		}
	}

	for (int p = 0; p < MAX_CELL_AMOUNT; p++) {
		if (Rparts[p].click == WAIT) {
			Rparts[p].click = OFF;
			for (int j = 0; j < 6; j++) {
				bool sw = false;
				for (int i = 0; i < MAX_CELL_NUM; i++) {
					if (Rcells[i].exist[j] == false) {
						Rcells[i].exist[j] = true;

						if (l < 0.f)	Rparts[p].radian = 90.f;
						Rparts[p].copy_x = Rcells[i].p[j].x - 4.f;
						Rparts[p].copy_y = Rcells[i].p[j].y;
						Rparts[p].y = rectangle[num].y;
						Rparts[p].type = PARTS_2;
						if (j % 2 == 1)	Rparts[p].copy_radian = 180.f;
						Rparts[p].copy_radian -= 90.f;
						sw = true;
						break;
					}
					if (sw)	break;
				}
				if (sw)	break;
			}
			break;
		}
	}
}