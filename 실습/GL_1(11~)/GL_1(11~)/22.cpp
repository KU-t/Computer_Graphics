#include <gl/freeglut.h>
#include <math.h>
#include <time.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define deg(x) x*3.141592/180

enum VIEW { Perspective, Orthographic };
enum ANIMATION { increase, decrease };
enum MOVE { UP, LEFT, DOWN, RIGHT };


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
void Obj_animation();
void Draw_Tree();
void Draw_horizontal();
void Draw_bench_press();
void 	Draw_Runningmachine();
void Move_arm_1();
void 	Draw_Plane();
void Check_Collision();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

class Circle {
public:
	int x = 0, z = 0;
	float radian_x = 0;
	float radian_y = 0;
	float radian_z = 0;
	MOVE move = LEFT;

	void Draw() {
		glPushMatrix();
		glTranslatef(x, 10.f, z);

		glRotatef(radian_x, 1.f, 0.f, 0.f);

		glRotatef(radian_y, 0.f, 1.f, 0.f);

		glRotatef(radian_z, 0.f, 0.f, 1.f);

		glColor4f(1.f, 0.f, 0.f, 0.f);
		glutWireSphere(10, 10, 10);
		//glColor4f(0.f, 0.f, 1.f, 1.f);
		//glutSolidCone(50.f, 50.f, 10.f, 10.f);
		glPopMatrix();
	}

	void Update() {
		switch (move) {
		case UP:
			if (z > -250) {
				z -= 10;
				radian_x -= 10.f;
			}
			else move = DOWN;
			break;

		case LEFT:
			if (x > -250) {
				x -= 10;
				radian_z += 10.f;
			}
			else move = RIGHT;
			break;

		case DOWN:
			if (z < 250) {
				z += 10;
				radian_x += 10.f;
			}
			else move = UP;
			break;

		case RIGHT:
			if (x < 250) {
				x += 10;
				radian_z -= 10.f;
			}
			else move = LEFT;
			break;
		}
	}
};

class Arm {
public:
	int x = 30, z = 0;
	float radian_x = 0, radian_y = 0;
	bool spin_x_sw = false, spin_y_sw = false;
	float size_x = 0, size_y = 0, size_z = 0;
	MOVE move = LEFT;
	Arm() {}

	Arm(float x, float y, float z) : size_x(x), size_y(y), size_z(z) {}

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

	void Update() {
		switch (move) {
		case UP:
			if (z > -250) {
				z -= 5;
			}
			else move = DOWN;
			break;

		case LEFT:
			if (x > -250) {
				x -= 5;
			}
			else move = RIGHT;
			break;

		case DOWN:
			if (z < 250) {
				z += 5;
			}
			else move = UP;
			break;

		case RIGHT:
			if (x < 250) {
				x += 5;
			}
			else move = LEFT;
			break;
		}
	}
};

Angle Angle_x, Angle_y, Angle_z;
float move_x = 0, move_y = 0, move_z = 0;
Circle circle;
Arm arm_1(20.f, 10.f, 20.f);
Arm arm_2(10.f, 30.f, 10.f);
Arm arm_3(6.f, 30.f, 6.f);

ANIMATION tree_animation = increase;
float tree_size = 1.f;

ANIMATION horizontal_animation = increase;
float horizontal_human_radian_x = 0.f;

ANIMATION bench_press_animation = increase;
float bench_press_human_radian_x = 0.f;

ANIMATION running_machine_animation = increase;
float running_machine_human_radian_x = 0.f;
float running_machine_radian = 0.f;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // µð½ºÇÃ·¹ÀÌ ¸ðµå ¼³Á¤
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("22");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.6f, 0.8f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y + 300.f, move_z + 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (View)	glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	Draw_bottom();


	circle.Draw();

	glPushMatrix(); // arm_1 push
	glTranslatef(arm_1.x, 0.f, arm_1.z);
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


	Draw_Tree();
	Draw_horizontal();
	Draw_bench_press();
	Draw_Runningmachine();
	Draw_Plane();
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
	Obj_animation();
	Check_Collision();
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

	case 'g':	case 'G':
		circle.move = (MOVE)((circle.move + 1) % 4);
		break;

	case 'h':	case 'H':
		arm_1.move = (MOVE)((arm_1.move + 1) % 4);
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
	//XÃà 
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

	//YÃà
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

	//ZÃà
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
	//glTranslatef(0.f, -150.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

void Obj_animation() {
	if (tree_animation == increase) {
		tree_size += 0.01f;
		if (tree_size > 1.2f)	tree_animation = decrease;
	}
	else if (tree_animation == decrease) {
		tree_size -= 0.01f;
		if (tree_size < 1.0f)	tree_animation = increase;
	}

	if (horizontal_animation == increase) {
		horizontal_human_radian_x += 3.f;
		if (horizontal_human_radian_x > 45.f)	horizontal_animation = decrease;
	}
	else if (horizontal_animation == decrease) {
		horizontal_human_radian_x -= 3.f;
		if (horizontal_human_radian_x < -45.f)	horizontal_animation = increase;
	}

	if (bench_press_animation == increase) {
		bench_press_human_radian_x += 3.f;
		if (bench_press_human_radian_x > 45.f)	bench_press_animation = decrease;
	}
	else if (bench_press_animation == decrease) {
		bench_press_human_radian_x -= 3.f;
		if (bench_press_human_radian_x < 0.f)	bench_press_animation = increase;
	}

	if (running_machine_animation == increase) {
		running_machine_human_radian_x += 3.f;
		if (running_machine_human_radian_x > 30.f)	running_machine_animation = decrease;
	}
	else if (running_machine_animation == decrease) {
		running_machine_human_radian_x -= 3.f;
		if (running_machine_human_radian_x < -30.f)	running_machine_animation = increase;
	}

	running_machine_radian += 3.f;
	circle.Update();
	arm_1.Update();
}

void Draw_Tree() {
	glPushMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
	glTranslatef(-220.f, 0.f, -220.f);
	glPushMatrix(); // ³ôÀÌ ÀÌµ¿ 1
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(20.f, 100.f, 20.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // ³ôÀÌ ÀÌµ¿ 1

	glPushMatrix(); // ³ôÀÌ ÀÌµ¿ 2
	glColor4f(0.2f, 0.8f, 0.2f, 1.0f);

	glTranslatef(0.f, 150.f, 0.f);
	glutSolidSphere(50.f, 10.f, 10.f);

	for (float i = 0.f; i < 720.f; i += 45.f) {
		glPushMatrix(); // ³ª¹µ°¡Áö
		glScalef(tree_size, tree_size, tree_size);
		if (i < 360.f) {
			glColor4f(0.15f, 0.6f, 0.15f, 1.0f);
			float x = 40 * cos(deg(i));
			float z = 40 * sin(deg(i));
			glTranslatef(x, -20.f, z);
		}
		else {
			glColor4f(0.1f, 0.4f, 0.1f, 1.0f);
			float x = 60 * cos(deg(i));
			float z = 60 * sin(deg(i));
			glTranslatef(x, -60.f, z);
		}
		glutSolidSphere(30.f, 10.f, 10.f);
		glPopMatrix(); // ³ª¹µ°¡Áö
	}

	glPopMatrix(); // ³ôÀÌ ÀÌµ¿ 2

	glPopMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
}

void Draw_horizontal() {
	glPushMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
	glTranslatef(-220.f, 0.f, 160.f);

	glPushMatrix(); // Ã¶ºÀ
	glPushMatrix(); // ±âµÕ
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(10.f, 100.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix(); // ±âµÕ
	glTranslatef(0.f, 95.f, 0.f);
	glScalef(5.f, 5.f, 100.f);
	glColor4f(0.1f, 0.1f, 0.4f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // Ã¶ºÀ

	glPushMatrix(); // »ç¶÷ È¸Àü ±âÁØ - Ã¶ºÀ
	glTranslatef(0.f, 95.f, 0.f);
	glRotatef(horizontal_human_radian_x, 0.f, 0.f, 1.f);

	glPushMatrix(); // »ç¶÷
	for (float i = -30.f; i < 40.f; i += 60.f) {
		glColor4f(1.f, 0.8f, 0.6f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -15.f, i);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0.f, -20.f, 0.f);
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glutSolidSphere(20.f, 10.f, 10.f);
	glTranslatef(10.f, -5.f, 0.f);
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glutSolidSphere(15.f, 10.f, 10.f);
	for (float i = -5.f; i < 15.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(13.f, -2.f, i);
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glutSolidSphere(2.f, 10.f, 10.f);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, -50.f, 0.f);
	glScalef(15.f, 40.f, 50.f);
	glColor4f(0.7f, 0.f, 0.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	for (float i = -15.f; i < 30.f; i += 30.f) {
		glColor4f(0.f, 0.f, 0.7f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -80.f, i);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix(); // »ç¶÷

	glPopMatrix(); // »ç¶÷ È¸Àü ±âÁØ

	glPopMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
}

void Draw_bench_press() {
	glPushMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
	glTranslatef(220.f, 0.f, 160.f);
	glPushMatrix(); // º¥Ä¡

	glPushMatrix(); // ±âµÕ
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(10.f, 100.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // ±âµÕ

	glPushMatrix(); // ÀÛÀº±âµÕ
	glTranslatef(-100.f, 20.f, 0.f);
	glScalef(10.f, 40.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -4.5f; i < 10.f; i += 9.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // ÀÛÀº±âµÕ

	glTranslatef(-40.f, 30.f, 0.f);
	glScalef(120.f, 10.f, 100.f);
	glutSolidCube(1);
	glPopMatrix(); // º¥Ä¡

	glPushMatrix(); // »ç¶÷
	glTranslatef(25.f, 43.f, 0.f);
	glRotatef(180.f, 0.f, 1.f, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);

	glPushMatrix(); // ÆÈ
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glRotatef(-90.f, 0.f, 0.f, 1.f);
	glRotatef(-180.f, 0.f, 1.f, 0.f);
	glTranslatef(-40.f, -15.f, 0.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix(); //ÆÈ
		glTranslatef(0.f, 15.f, i);
		if (i <= -30.f)	glRotatef(180.f, 0.f, 1.f, 0.f);
		glRotatef(bench_press_human_radian_x, 1.f, 0.f, 0.f);
		glPushMatrix();//¾Æ·¡ÆÈ
		glScalef(10.f, 30.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();//¾Æ·¡ÆÈ
		glTranslatef(0.f, 15.f, 0.f);
		glPushMatrix(); //À­ÆÈ
		glRotatef(-bench_press_human_radian_x * 2, 1.f, 0.f, 0.f);
		glTranslatef(0.f, 15.f, 0.f);
		glScalef(10.f, 30.f, 10.f);
		glutSolidCube(1);
		glPopMatrix(); //À­ÆÈ
		glPopMatrix(); //ÆÈ
	}

	glPushMatrix(); // ¿ª±â
	glTranslatef(0.f, 60.f - 20 * sin(deg(bench_press_human_radian_x)), 0.f);
	glPushMatrix(); // ¼Õ
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix();
		glTranslatef(0.f, 0.f, i);
		glScalef(10.f, 10.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();

	}
	glPopMatrix(); // ¼Õ

	glPushMatrix(); // ºÀ
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glScalef(5.f, 5.f, 150.f);
	glutSolidCube(1);
	glPopMatrix(); // ºÀ

	for (float i = -1.f; i < 2.f; i += 2.f) {
		glPushMatrix();
		glTranslatef(0.f, 0.f, i * 75.f);
		glColor4f(0.7f, 0.f, 0.7f, 0.f);
		glutSolidCylinder(20.f, 5.f, 15, 15);
		glTranslatef(0.f, 0.f, i * 5.f);
		glColor4f(0.f, 0.7f, 0.7f, 0.f);
		glutSolidCylinder(15.f, 5.f, 15, 15);
		glTranslatef(0.f, 0.f, i * 5.f);
		glColor4f(0.7f, 0.7f, 0.f, 0.f);
		glutSolidCylinder(10.f, 5.f, 15, 15);
		glPopMatrix();
	}

	glPopMatrix(); // ¿ª±â
	glPopMatrix(); // ÆÈ


	glPushMatrix(); // 1
	glTranslatef(0.f, -20.f, 0.f);
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glutSolidSphere(20.f, 10.f, 10.f);
	glTranslatef(10.f, -5.f, 0.f);
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glutSolidSphere(15.f, 10.f, 10.f);
	for (float i = -5.f; i < 15.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(13.f, -2.f, i);
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glutSolidSphere(2.f, 10.f, 10.f);
		glPopMatrix();
	}
	glPopMatrix(); // 1

	glPushMatrix();
	glTranslatef(0.f, -50.f, 0.f);
	glScalef(15.f, 40.f, 50.f);
	glColor4f(0.7f, 0.f, 0.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	for (float i = -15.f; i < 30.f; i += 30.f) {
		glColor4f(0.f, 0.f, 0.7f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -80.f, i);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix(); // »ç¶÷

	glPopMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
}

void 	Draw_Runningmachine() {
	glPushMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
	glTranslatef(220.f, 0.f, -160.f);
	glPushMatrix(); // Ã¶ºÀ
	glPushMatrix(); // ±âµÕ
	glTranslatef(0.f, 40.f, 0.f);
	glScalef(10.f, 80.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // ±âµÕ
	glTranslatef(0.f, 75.f, 0.f);
	glScalef(5.f, 5.f, 100.f);
	glColor4f(0.1f, 0.1f, 0.4f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // Ã¶ºÀ

	glPushMatrix(); // ·±´×¸Ó½Å
	glTranslatef(-50.f, 15.f, 0.f);
	glPushMatrix(); //¹Ù´Ú
	glScalef(100.f, 1.f, 80.f);
	glutSolidCube(1);
	glPopMatrix(); // ¹Ù´Ú

	for (float i = -1.f; i < 2.f; i += 2.f) {
		for (float j = -1.f; j < 2.f; j += 2.f) {
			glPushMatrix(); // Åä·¯½º
			glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
			glTranslatef(i * 50.f, -7.f, j * 40.f);
			glRotatef(running_machine_human_radian_x, 0.f, 0.f, 1.f);
			glutSolidTorus(2.5f, 5.f, 10, 10);
			glPopMatrix(); //Åä·¯½º
		}
		glColor4f(0.1f, 0.1f, 0.4f, 1.0f);

		glPushMatrix();
		glTranslatef(i * 50.f, -7.f, 0.f);
		glRotatef(running_machine_radian, 0.f, 0.f, 1.f);
		for (float j = 0.f; j < 360.f; j += 45) {
			float x = 7 * cos(deg(j));
			float y = 7 * sin(deg(j));
			glPushMatrix();
			glTranslatef(x, y, 0.f);
			glRotatef(j + 90.f, 0.f, 0.f, 1.f);
			glScalef(5.f, 1.f, 80.f);
			glutSolidCube(1);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();// ·±´×¸Ó½Å

	glPushMatrix(); // »ç¶÷
	glTranslatef(-40.f, 115.f, 0.f);

	glPushMatrix(); // ÆÈ
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glRotatef(-90.f, 0.f, 0.f, 1.f);
	glRotatef(-180.f, 0.f, 1.f, 0.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix(); // 1
		glTranslatef(-40.f, 15.f, i);
		glRotatef(0.f, 0.1, 0.1, 0.1);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix(); // 1
	}
	glPopMatrix(); // ÆÈ


	glPushMatrix(); // 1
	glTranslatef(0.f, -20.f, 0.f);
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glutSolidSphere(20.f, 10.f, 10.f);
	glTranslatef(10.f, -5.f, 0.f);
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glutSolidSphere(15.f, 10.f, 10.f);
	for (float i = -5.f; i < 15.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(13.f, -2.f, i);
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glutSolidSphere(2.f, 10.f, 10.f);
		glPopMatrix();
	}
	glPopMatrix(); // 1

	glPushMatrix();
	glTranslatef(0.f, -50.f, 0.f);
	glScalef(15.f, 40.f, 50.f);
	glColor4f(0.7f, 0.f, 0.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix(); // ´Ù¸®
	for (float i = -1.f; i < 2.f; i += 2.f) {
		glColor4f(0.f, 0.f, 0.7f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -60.f, i * 15.f);
		glRotatef(i * running_machine_human_radian_x, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -20.f, 0.f);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix(); // ´Ù¸®

	glPopMatrix(); // »ç¶÷

	glPopMatrix(); // x_z ÁÂÇ¥ÀÌµ¿
}

void 	Draw_Plane() {
	glPushMatrix();
	glTranslatef(0.f,200.f,0.f);
	glutSolidCube(10);
	glPopMatrix();
}

void Check_Collision() {
	if (abs(circle.x - arm_1.x) <= 30)
		if (abs(circle.z - arm_1.z) <= 30) {
			circle.move = (MOVE)(rand() % 4);
			arm_1.move = (MOVE)(rand() % 4);
		}
}