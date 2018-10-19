#include <gl/freeglut.h>
#include <math.h>

enum { Circle, Sin, Spring, Zigzag, Click };
enum { triangle, rectangle };
enum CLICK_POINT { zero, one, two, three, four };

class Point {
public:
	float x, y;
	bool exist;

	Point() {
		x = 1000.f;
		y = 1000.f;
		exist = false;
	}
};

#define W_x 800
#define W_y 600
#define W_z 600
#define PI 3.141592
#define Draw_Circle_size 400
#define shape_small_size 20.f
#define shape_big_size 40.f
#define click_move_speed 0.1f

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Draw();
void Draw_shape(bool shape, float x, float y);
void change_shape_size();
void click_move(float, float);

int Big_Shape = Zigzag;
bool Small_shape = rectangle;
float cnt_rad = 0;
bool cnt_gradient = true;
float shape_size = shape_small_size;
float shape_size_sgin = 1.f;
float mx = -720.f, my = 0;
float cx = 0.f, cy = 0.f;
int m_TF = 1;
Point p[5];
bool Click_switch = false;
int count = 0;
CLICK_POINT click_point = zero;
bool click_move_start = true;
bool switch_spin = false;
float Angle = 0;

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("3¿Â Ω«Ω¿ 12");
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
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glRotatef(Angle, 0.f, 1.f, 0.f);


	glBegin(GL_LINES);
	glVertex3f(-W_x, 0, 0);
	glVertex3f(W_x, 0, 0);
	glVertex3f(0, -W_y, 0);
	glVertex3f(0, W_y, 0);
	glEnd();

	Draw();

	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
}

void Keyboard(unsigned char key, int x, int y) {
	count = 0;
	switch (key) {
	case 'c': case 'C':
		Small_shape = (Small_shape + 1) % 2;
		count = 5;
		break;

	case '1':
		Big_Shape = Circle;
		cnt_rad = 0;
		Click_switch = false;
		break;

	case '2':
		Big_Shape = Sin;
		cnt_rad = -720;
		Click_switch = false;
		break;

	case '3':
		Big_Shape = Spring;
		cnt_rad = -810;
		Click_switch = false;
		break;

	case '4':
		Big_Shape = Zigzag;
		cnt_rad = -720;
		mx = -720.f;
		my = 0.f;
		Click_switch = false;
		break;

	case '5':
		Big_Shape = Click;
		for (int i = 0; i < 5; i++) {
			p[i].x = 1000.f;
			p[i].y = 1000.f;
			p[i].exist = false;
		}
		cx = 0;
		cy = 0;
		Click_switch = true;
		count = 0;
		click_point = zero;
		break;

	case '[':	case '{':
		Angle += 15.f;
		break;

	case ']': case '}':
		Angle -= 15.f;
		break;

	case 't': case 'T':
		if (Big_Shape == Circle) {
			if (!switch_spin)	switch_spin = true;
			else	switch_spin = false;
			cnt_rad = 0;
		}
		break;
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && count < 5) {
		p[count].x = 2 * (x - 400);
		p[count].y = 2 * (W_y - y - 300);
		p[count].exist = true;
		count++;
		if (count == 5)	click_move_start = true;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && count != 5 && count > 2) {
		count = 5;
		click_move_start = true;
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	cnt_rad += 4.f;
	switch (Big_Shape) {
	case Sin:	case Zigzag:
		if (cnt_rad >= 720)	cnt_rad = -720;
		break;

	case Spring:
		if (cnt_rad >= 630)	cnt_rad = -810;
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Draw() {

	float x = 0, y = 0, rad = 0;

	switch (Big_Shape) {
	case Circle:
		glBegin(GL_POINTS);
		for (rad = 0.f; rad < 360.f; rad += 0.2f) {
			x = Draw_Circle_size * cos(rad*PI / 180);
			y = Draw_Circle_size * sin(rad*PI / 180);
			glVertex2f(x, y);
		}
		x = Draw_Circle_size * cos(cnt_rad*PI / 180);
		y = Draw_Circle_size * sin(cnt_rad*PI / 180);
		glEnd();
		break;

	case Sin:
		glBegin(GL_POINTS);
		for (rad = -720.f; rad < 720.f; rad += 0.2f) {
			x = rad;
			y = Draw_Circle_size * sin(rad*PI / 180);
			glVertex2f(x, y);
		}
		x = cnt_rad;
		y = Draw_Circle_size * sin(cnt_rad*PI / 180);
		glEnd();
		break;

	case Spring:
		glBegin(GL_POINTS);
		for (rad = -810.f; rad < 630.f; rad += 0.2f) {
			x = Draw_Circle_size / 2 * cos(rad*PI / 180) + rad + 100;
			y = Draw_Circle_size / 2 * sin(rad*PI / 180);
			glVertex2f(x, y);
		}
		x = Draw_Circle_size / 2 * cos(cnt_rad*PI / 180) + cnt_rad + 100;
		y = Draw_Circle_size / 2 * sin(cnt_rad*PI / 180);
		glEnd();
		break;

	case Zigzag: {
		glBegin(GL_POINTS);
		bool gradient = true;

		for (x = -720.f; x < 720.f; x += 0.2f) {
			if (y >= 180.f)	gradient = false;
			else if (y <= -180.f)	gradient = true;

			if (gradient)	y += 0.2f;
			else if (!gradient)	y -= 0.2f;
			glVertex2f(x, y);

			if (m_TF)
				if (x == mx) {
					my = y;
					m_TF = 0;
				}
		}

		if (mx >= 720.f)	mx = -720.f;
		mx += 0.2f;
		m_TF = 1;
		x = mx;
		y = my;
		glEnd();
		break;
	}

	case Click:

		if (count < 5) {
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < 5; i++) {
				if (p[i].exist == true)
					glVertex2f(p[i].x, p[i].y);
			}
			glEnd();
		}

		else if (count == 5) {
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 5; i++) {
				if (p[i].exist == true)
					glVertex2f(p[i].x, p[i].y);
			}
			glEnd();
			click_move(x, y);
		}
		break;
	}

	if (Big_Shape != Click) {
		if (switch_spin) {
			glPushMatrix();
			glRotatef(cnt_rad, 0.f, 0.f, 1.f);
		}

		Draw_shape(Small_shape, x, y);

		if (switch_spin) 	glPopMatrix();
	}
	glutPostRedisplay();
}

void Draw_shape(bool shape, float x, float y) {
	glBegin(GL_POLYGON);
	if (shape == triangle) {
		glVertex2f(x, y + shape_size);
		glVertex2f(x - (shape_size*1.414), y - (shape_size / 2));
		glVertex2f(x + (shape_size*1.414), y - (shape_size / 2));
	}

	else if (shape == rectangle) {
		glVertex2f(x - shape_size, y - shape_size);
		glVertex2f(x - shape_size, y + shape_size);
		glVertex2f(x + shape_size, y + shape_size);
		glVertex2f(x + shape_size, y - shape_size);
	}
	glEnd();
	change_shape_size();
}

void change_shape_size() {
	if (shape_size <= shape_small_size)
		shape_size_sgin = 0.002f;
	else if (shape_size >= shape_big_size)
		shape_size_sgin = -0.002f;

	shape_size += shape_size_sgin;
}

void click_move(float x, float y) {
	float gradient;

	switch (click_point) {
	case zero:
		gradient = (p[1].y - p[0].y) / (p[1].x - p[0].x);
		if (p[1].x > p[0].x)	cx += click_move_speed;
		else cx += -click_move_speed;
		cy = gradient * cx;
		x = p[0].x + cx;
		y = p[0].y + cy;
		if (abs((int)(x - p[1].x)) == 0) {
			cx = 0;
			cy = 0;
			click_point = one;
		}
		break;

	case one:
		gradient = (p[2].y - p[1].y) / (p[2].x - p[1].x);
		if (p[2].x > p[1].x)	cx += click_move_speed;
		else cx += -click_move_speed;
		cy = gradient * cx;
		x = p[1].x + cx;
		y = p[1].y + cy;
		if (abs((int)(x - p[2].x)) == 0) {
			cx = 0;
			cy = 0;
			click_point = two;
		}
		break;

	case two:
		if (!p[3].exist) {
			p[3].x = p[0].x;
			p[3].y = p[0].y;
		}
		gradient = (p[3].y - p[2].y) / (p[3].x - p[2].x);
		if (p[3].x > p[2].x)	cx += click_move_speed;
		else cx += -click_move_speed;
		cy = gradient * cx;
		x = p[2].x + cx;
		y = p[2].y + cy;
		if (abs((int)(x - p[3].x)) == 0) {
			cx = 0;
			cy = 0;
			click_point = three;
			if (!p[3].exist) click_point = zero;
		}
		break;

	case three:
		if (!p[4].exist) {
			p[4].x = p[0].x;
			p[4].y = p[0].y;
		}
		gradient = (p[4].y - p[3].y) / (p[4].x - p[3].x);
		if (p[4].x > p[3].x)	cx += click_move_speed;
		else cx += -click_move_speed;
		cy = gradient * cx;
		x = p[3].x + cx;
		y = p[3].y + cy;
		if (abs((int)(x - p[4].x)) == 0) {
			cx = 0;
			cy = 0;
			click_point = four;
			if (!p[4].exist) click_point = zero;
		}
		break;

	case four:
		gradient = (p[0].y - p[4].y) / (p[0].x - p[4].x);
		if (p[0].x > p[4].x)	cx += click_move_speed;
		else cx += -click_move_speed;
		cy = gradient * cx;
		x = p[4].x + cx;
		y = p[4].y + cy;
		if (abs((int)(x - p[0].x)) == 0) {
			cx = 0;
			cy = 0;
			click_point = zero;
		}
		break;
	}
	Draw_shape(Small_shape, x, y);
}
