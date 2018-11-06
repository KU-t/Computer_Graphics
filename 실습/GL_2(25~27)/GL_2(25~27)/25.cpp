#include <gl/freeglut.h>
#include <math.h>
#include <time.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define deg(x) 3.141592*x/180
#define SIZE 30.f
#define THROW_BALLS 100
enum VIEW { Perspective, Orthographic };
enum ANIMATION { OPEN, CLOSE };

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
void Draw_quad(
	float trans_x, float trans_y, float trans_z,
	float rotate_rad, float rotate_x, float rotate_y, float rotate_z,
	float LT_R, float LT_G, float LT_B,
	float RT_R, float RT_G, float RT_B,
	float LB_R, float LB_G, float LB_B,
	float RB_R, float RB_G, float RB_B);
void Change_animtion();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Spring {
public:
	float x, y, z, ball_x, ball_y, ball_z;
	float size = SIZE;
	float scale_y = 0.2f;
	
	void Draw() {
		y = 0.2f;
		glPushMatrix();
		glTranslatef(0.f, -40.f, 0.f);
		glScalef(1.f, scale_y, 1.f);
		glBegin(GL_POINTS);
		for (double radian = 0.f; radian < 3600.f; radian += 1.f) {
			x = size * cos(deg(radian));
			z = size * sin(deg(radian));
			glVertex3d(x, y, z);
			y += 0.05;
		}
		glEnd();
		glPopMatrix();
	}

	void update() {
		if (scale_y < 1.f)
			scale_y+=0.1;
	}
};

class Ball {
public:
	float x, y=0, z;
	float vy = 0.f, vz = 30.f;
	float radian = 0.f;
	float size = SIZE;
	float exist = false;

	void spring_ball_Draw() {
		glPushMatrix();
			x = size * cos(deg(radian));
			z = size * sin(deg(radian));
		glTranslatef(x, -40.f +y, z);
		//glScalef(1.f, scale_y, 1.f);
		//glBegin(GL_POINTS);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glutSolidSphere(10.f, 10.f, 10.f);
		glPopMatrix();
	}

	void throw_ball_Draw() {
		if (exist) {
			glPushMatrix();
			glTranslatef(x, y, z);
			glColor4f(0.7f, 0.7f, 1.f, 1.f);
			glutSolidSphere(5.f, 10.f, 10.f);
			glPopMatrix();
		}
	}

	void throw_ball_Update() {
		float vel_y = -1.f;
		if (exist) {
			vy += vel_y;
			if (z <= 150.f) {
				if (y + vy < -45.f) {
					y = -44.f;
					vy = -0.75f * vy;
				}
			}
			y += vy;
			z += vz;
		}
		else if (!exist) {
			x = rand() % 90 - 45.f;
			y = rand() % 50;
			z = rand() % 100 - 50.f;
			vy = 0.f;
			vz = 1.f;
		}
	}
};

Angle Angle_x, Angle_y, Angle_z;
ANIMATION Front_ani = CLOSE, Top_ani = CLOSE;
Spring spring;
Ball spring_ball;
Ball throw_balls[THROW_BALLS];
bool sw_smooth = false;
bool sw_culling = false;
float move_x = 0.f, move_y = 0.f, move_z = 0.f;
float radian_front = 0.f, radian_top = 0.f;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	Angle_x.radian = 30.f;
	Angle_y.radian = 45.f;
	Angle_y.sw = true;
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
	if (sw_culling)	glDisable(GL_DEPTH_TEST);
	else	glEnable(GL_DEPTH_TEST);
	

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
	//Draw_bottom();
	spring.Draw();
	spring_ball.spring_ball_Draw();
	//앞
	glPushMatrix();
	glTranslatef(0.f, -50.f, 50.f);
	glRotatef(radian_front, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 50.f, -50.f);
	Draw_quad(0.f, 0.f, 50.f,
		0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f,
		1.f, 0.f, 1.f,
		1.f, 0.f, 0.f);
	glPopMatrix();
	glPopMatrix();
	//뒤
	Draw_quad(0.f, 0.f, -50.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 0.f);
	//좌
	Draw_quad(-50.f, 0.f, 0.f,
		-90.f, 0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		0.f, 0.f, 0.f);
	//우
	Draw_quad(50.f, 0.f, 0.f,
		-90.f, 0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 0.f, 1.f,
		0.f, 0.f, 1.f);
	//위
	glPushMatrix();
	glTranslatef(0.f, 50.f, -50.f);
	glRotatef(-radian_top, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, -50.f, 50.f);
	Draw_quad(0.f, 50.f, 0.f,
		-90.f, 1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 0.f);
	glPopMatrix();
	glPopMatrix();
	//아래
	Draw_quad(0.f, -50.f, 0.f,
		-90.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 1.f,
		1.f, 0.f, 0.f);

	for (int i = 0; i < THROW_BALLS; i++) {
		throw_balls[i].throw_ball_Draw();
	}
	//glColor4f(1.f, 0.f, 0.f, 1.f);
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

	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	glutPostRedisplay();
	Change_animtion();
	for (int i = 0; i < THROW_BALLS; i++) {
		throw_balls[i].throw_ball_Update();
	}
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
		
	case ',':
		sw_smooth = (sw_smooth + 1) % 2;
		break;
	case '.':
		sw_culling = (sw_culling + 1) % 2;
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
		Front_ani = (ANIMATION)((Front_ani + 1) % 2);
		break;

	case ']':
		Top_ani = (ANIMATION)((Top_ani + 1) % 2);
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

void Draw_quad(float trans_x, float trans_y, float trans_z, 
	float rotate_rad, float rotate_x, float rotate_y, float rotate_z,
	float LT_R, float LT_G, float LT_B,
	float RT_R, float RT_G, float RT_B,
	float LB_R, float LB_G, float LB_B,
	float RB_R, float RB_G, float RB_B){

	if(sw_smooth)	glShadeModel(GL_FLAT);
	else glShadeModel(GL_SMOOTH);

	glPushMatrix();
	glTranslatef(trans_x, trans_y, trans_z);
	glRotatef(rotate_rad, rotate_x, rotate_y, rotate_z);
	glBegin(GL_QUADS);
	glColor4f(LT_R, LT_G, LT_B,1.f);
	glVertex3f(-50.f,50.f,0.f);
	glColor4f(RT_R, RT_G, RT_B, 1.f);
	glVertex3f(50.f, 50.f, 0.f);
	glColor4f(LB_R, LB_G, LB_B, 1.f);
	glVertex3f(50.f, -50.f, 0.f);
	glColor4f(RB_R, RB_G, RB_B, 1.f);
	glVertex3f(-50.f, -50.f, 0.f);
	glEnd();
	glPopMatrix();
}

void Change_animtion() {
	if (Front_ani == OPEN) {
		if (radian_front < 90.f)
			radian_front += 3.f;
	}
	else if (Front_ani == CLOSE) {
		if (radian_front > 0.f)
			radian_front -= 3.f;
	}

	if (radian_front >= 90.f) {
		for (int i = 0; i < THROW_BALLS; i++) {
			if(throw_balls[i].exist == false)
			throw_balls[i].exist = true;
		}
	}
	else if (radian_front < 90.f) {
		for (int i = 0; i < THROW_BALLS; i++) {
			if (throw_balls[i].exist == true)
				throw_balls[i].exist = false;
		}
	}

	if (Top_ani == OPEN) {
		if (radian_top < 90.f)
			radian_top += 3.f;
	}
	else if (Top_ani == CLOSE) {
		if (radian_top > 0.f) {
			radian_top -= 3.f;
			spring_ball.y = 0.f;
			spring_ball.radian = 0.f;
		}
	}
	if (Top_ani == OPEN) {
		if (radian_top > 30.f && spring.scale_y < 1.f) {
			spring.scale_y += 0.05;
		}
		if (radian_top >= 90.f && spring.scale_y > 0.9f) {
			if (spring_ball.radian < 3600.f) {
				spring_ball.y += 0.3;
				spring_ball.radian += 6.f;
			}
		}
	}
	else if (Top_ani == CLOSE) {
		if (spring.scale_y > 0.f && radian_top < 80.f) {
			spring.scale_y -= 0.05f;
		}
	}
}
