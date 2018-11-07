#include <GL/freeglut.h>
#include <iostream>

#define WIDTH 600
#define HEIGHT 600

void drawScene(void);
void Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

float rotate_y = 0.f;

class Object
{
private:
	float mx = 300;
	float angle = 0.5;
	float cube_x[3] = { 0, 0, 0 };
	float cube_y[3] = { 0, 0, 0 };
	float ball_x[5] = { 0 };
	float ball_y[5] = { 0 };

	int ball_dir[5] = { 0 };
	int ball_count = 0;
	bool left_rotate = false;
	bool right_rotate = false;
	bool dir[3][4] = { false };
public:
	int count = 0;
	void set_mx(int x) { mx = x; }
	void init_dir(int cube_type)
	{
		for (int j = 0; j < 4; j++)
			dir[cube_type][j] = false;
	}
	void init_ball_dir(int i)
	{
		ball_dir[i] = 0;
	}
	void right_cube_move(int cube_type, int dir_type, float speed, float size)
	{
		if (right_rotate == true)
		{
			if (cube_x[cube_type] == 0 && cube_y[cube_type] == 0)
			{
				cube_x[cube_type] = 0, cube_y[cube_type] = 0;
				init_dir(cube_type);
				dir[dir_type][0] = true;
			}
			if (cube_x[cube_type] == 90 - size && cube_y[cube_type] == 0)
			{
				init_dir(cube_type);
				dir[dir_type][1] = true;
			}
			if (cube_x[cube_type] == 90 - size && cube_y[cube_type] == 90 - size)
			{
				init_dir(cube_type);
				dir[dir_type][2] = true;
			}
			if (cube_x[cube_type] == 0 && cube_y[cube_type] == 90 - size)
			{
				init_dir(cube_type);
				dir[dir_type][3] = true;
			}

			if (dir[dir_type][0] == true) glTranslatef(cube_x[cube_type] += speed, cube_y[cube_type], 0);
			if (dir[dir_type][1] == true) glTranslatef(cube_x[cube_type], cube_y[cube_type] += speed, 0);
			if (dir[dir_type][2] == true) glTranslatef(cube_x[cube_type] -= speed, cube_y[cube_type], 0);
			if (dir[dir_type][3] == true) glTranslatef(cube_x[cube_type], cube_y[cube_type] -= speed, 0);
		}
	}
	void left_cube_move(int cube_type, int dir_type, float speed, float size)
	{
		count++;
		if (count >= 500) {

			if (left_rotate == true)
			{
				if (cube_x[cube_type] == 0 && cube_y[cube_type] == 0)
				{
					cube_x[cube_type] = 0, cube_y[cube_type] = 0;
					init_dir(cube_type);
					dir[dir_type][0] = true;
				}
				if (cube_x[cube_type] == 0 && cube_y[cube_type] == 90 - size)
				{
					init_dir(cube_type);
					dir[dir_type][1] = true;
				}
				if (cube_x[cube_type] == 90 - size && cube_y[cube_type] == 90 - size)
				{
					init_dir(cube_type);
					dir[dir_type][2] = true;
				}
				if (cube_x[cube_type] == 90 - size && cube_y[cube_type] == 0)
				{
					init_dir(cube_type);
					dir[dir_type][3] = true;
				}

				if (dir[dir_type][0] == true) glTranslatef(cube_x[cube_type], cube_y[cube_type] += speed, 0);
				if (dir[dir_type][1] == true) glTranslatef(cube_x[cube_type] += speed, cube_y[cube_type], 0);
				if (dir[dir_type][2] == true) glTranslatef(cube_x[cube_type], cube_y[cube_type] -= speed, 0);
				if (dir[dir_type][3] == true) glTranslatef(cube_x[cube_type] -= speed, cube_y[cube_type], 0);
			}
		}
	}
	void make_ball(int type, float speed)
	{
		glPushMatrix();
		if (ball_y[type] == 50)
		{
			init_ball_dir(type);
			ball_dir[type] = 1;
		}
		if (ball_x[type] == 50)
		{
			init_ball_dir(type);
			ball_dir[type] = 2;
		}
		if (ball_y[type] == -50)
		{
			init_ball_dir(type);
			ball_dir[type] = 3;
		}
		if (ball_x[type] == -50)
		{
			init_ball_dir(type);
			ball_dir[type] = 4;
		}
		if (ball_dir[type] == 0) glTranslatef(0, ball_y[type] += speed, 0);
		if (ball_dir[type] == 1) glTranslatef(ball_x[type] += speed / 2, ball_y[type] -= speed / 2, 0);
		if (ball_dir[type] == 2) glTranslatef(ball_x[type] -= speed / 2, ball_y[type] -= speed / 2, 0);
		if (ball_dir[type] == 3) glTranslatef(ball_x[type] -= speed / 2, ball_y[type] += speed / 2, 0);
		if (ball_dir[type] == 4) glTranslatef(ball_x[type] += speed / 2, ball_y[type] += speed / 2, 0);
		glColor3f(0, 0, 1);
		glutSolidSphere(5, 5, 5);
		glPopMatrix();
	}
	void draw()
	{
		if (mx < 300)
		{
			left_rotate = true;
			right_rotate = false;
			glRotatef(angle, 0, 0, 1);
		}
		if (mx > 300)
		{
			right_rotate = true;
			left_rotate = false;
			glRotatef(angle, 0, 0, -1);
		}
		// 위
		glPushMatrix();
		glRotatef(rotate_y, 0.f, 1.f, 0.f);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
		glVertex3i(50, 50, 50);
		glVertex3i(-50, 50, 50);
		glVertex3i(-50, 50, -50);
		glVertex3i(50, 50, -50);
		glEnd();
		// 아래
		glColor3f(0.75, 0.75, 0.75);
		glBegin(GL_QUADS);
		glVertex3i(-50, -50, 50);
		glVertex3i(50, -50, 50);
		glVertex3i(50, -50, -50);
		glVertex3i(-50, -50, -50);
		glEnd();
		// 뒤
		glColor3f(0.25, 0.25, 0.25);
		glBegin(GL_QUADS);
		glVertex3i(-50, 50, -50);
		glVertex3i(50, 50, -50);
		glVertex3i(50, -50, -50);
		glVertex3i(-50, -50, -50);
		glEnd();
		// 우측
		glColor3f(0.125, 0.125, 0.125);
		glBegin(GL_QUADS);
		glVertex3i(50, 50, 50);
		glVertex3i(50, -50, 50);
		glVertex3i(50, -50, -50);
		glVertex3i(50, 50, -50);
		glEnd();
		// 좌측
		glColor3f(0.625, 0.625, 0.625);
		glBegin(GL_QUADS);
		glVertex3i(-50, 50, 50);
		glVertex3i(-50, -50, 50);
		glVertex3i(-50, -50, -50);
		glVertex3i(-50, 50, -50);
		glEnd();
		glPopMatrix();
		// 육면체 ---------------------------------------------------------------------------------------
		glPushMatrix();
		glRotatef(rotate_y, 0.f, 1.f, 0.f);
		right_cube_move(0, 0, 0.5, 0);
		left_cube_move(0, 0, 0.5, 0);
		glTranslatef(-45, -45, 0);
		glColor3f(1, 0, 0);
		glutSolidCube(10);
		glColor3f(1, 1, 1);
		glutWireCube(10.1);
		glPopMatrix();
		glPushMatrix();
		glRotatef(rotate_y, 0.f, 1.f, 0.f);
		right_cube_move(1, 1, 0.5, 10);
		left_cube_move(1, 1, 0.5, 10);
		glTranslatef(-40, -40, -15);
		glColor3f(1, 0, 0);
		glutSolidCube(20);
		glColor3f(1, 1, 1);
		glutWireCube(20.1);
		glPopMatrix();
		glPushMatrix();
		glRotatef(rotate_y, 0.f, 1.f, 0.f);
		right_cube_move(2, 2, 0.5, -5);
		left_cube_move(2, 2, 0.5, -5);
		glTranslatef(-47.5, -47.5, 7.5);
		glColor3f(1, 0, 0);
		glutSolidCube(5);
		glColor3f(1, 1, 1);
		glutWireCube(5.1);
		glPopMatrix();
		// 공 --------------------------------------------------------------------------------------------------
		if (0 < ball_count) make_ball(0, 1);
		if (1 < ball_count) make_ball(1, 0.5);
		if (2 < ball_count) make_ball(2, 2);
		if (3 < ball_count) make_ball(3, 2.5);
		if (4 < ball_count) make_ball(4, 5);
	}
	void command(char key)
	{
		switch (key)
		{
		case'b': case'B':
			if (ball_count < 6) ball_count++;
			break;
		case 'x':
			glRotatef(1, 1, 0, 0);
			break;
		case 'X':
			glRotatef(-1, 1, 0, 0);
			break;
		case 'y':
			glRotatef(-1, 0, 1, 0);
			break;
		case 'Y':
			glRotatef(1, 0, 1, 0);
			break;
		case 'z':
			glRotatef(-1, 0, 0, 1);
			break;
		case 'Z':
			glRotatef(1, 0, 0, 1);
			break;
		case 'w': case 'W':
			glTranslatef(0, -1, 0);
			break;
		case 'a': case 'A':
			glTranslatef(1, 0, 0);
			break;
		case 's': case 'S':
			glTranslatef(0, 1, 0);
			break;
		case 'd': case 'D':
			glTranslatef(-1, 0, 0);
			break;
		case '+':
			glTranslatef(0, 0, 1);
			break;
		case '-':
			glTranslatef(0, 0, -1);
			break;
		case 'i': case 'I':
			glLoadIdentity();
			right_rotate = false;
			left_rotate = false;
			mx = 300;
			count = 0;
			rotate_y = 0.f;
			for (int i = 0; i < 3; i++)
			{
				cube_x[i] = 0;
				cube_y[i] = 0;
			}
			break;
		case ';':
			for (int i = 0; i < 5; i++) {

				ball_x[i] =  0;
				ball_y[i] =  0;
				ball_dir[i] =  0;
			}

			ball_count = 0;
			break;
		case '[':
			rotate_y += 5.f;
			break;
		case ']':
			rotate_y -= 5.f;
			break;
		}
	}
};

Object obj;

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutTimerFunc(15, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

void drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPushMatrix();
	obj.draw();
	//glPopMatrix();
	glutSwapBuffers();
}

void Reshape(int w = WIDTH, int h = HEIGHT)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, 1.0, 1.0, 1000.0); // 원근투영
	glTranslatef(0.0, 0.0, -300.0); // 시야확보

	glMatrixMode(GL_MODELVIEW);
}

void TimerFunction(int value)
{
	glutTimerFunc(15, TimerFunction, 1); 

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	obj.command(key);

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	obj.set_mx(x);
}