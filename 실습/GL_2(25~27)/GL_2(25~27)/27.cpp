#include <gl/freeglut.h>
#include <math.h>
#include <time.h>
#include <iostream>

using namespace std;

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
bool Check_Collision(float cx, float cz);
void Draw_box();
void Draw_quad(float trans_x, float trans_y, float trans_z,
	float rotate_rad, float rotate_x, float rotate_y, float rotate_z,
	float LT_R, float LT_G, float LT_B,
	float RT_R, float RT_G, float RT_B,
	float LB_R, float LB_G, float LB_B,
	float RB_R, float RB_G, float RB_B);
void Draw_human();

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

ANIMATION tree_animation = increase;
float tree_size = 1.f;

ANIMATION horizontal_animation = increase;
float horizontal_human_radian_x = 0.f;

ANIMATION bench_press_animation = increase;
float bench_press_human_radian_x = 0.f;

ANIMATION running_machine_animation = increase;
float running_machine_human_radian_x = 0.f;
float running_machine_radian = 0.f;

float open_door = 0.f;
bool sw_smooth = false;
bool sw_culling = false;
bool sw_open_door = false;
bool sw_human_jump = false;

float human_x = 0.f, human_y = 0.f, human_z = 0.f;
float human_y_vel = 30.f;
float human_radian = 0.f;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("21");
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
	if (sw_culling)	glDisable(GL_DEPTH_TEST);
	else	glEnable(GL_DEPTH_TEST);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y + 300.f, move_z + 1500.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	//Draw_bottom();
	Draw_box();


	glTranslatef(0.f, -350.f, 0.f);
	Draw_Tree();
	Draw_horizontal();
	Draw_bench_press();
	Draw_Runningmachine();

	glTranslatef(0.f, +115.f, 0.f);
	
	glPushMatrix();
	glTranslatef(human_x, human_y, human_z);
	glRotatef(human_radian, 0.f, 1.f, 0.f);
	Draw_human();
	glPopMatrix();

	glPopMatrix();
	//glPopMatrix();

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
	glutPostRedisplay();
	if (sw_open_door) {
		if (open_door < 700.f)	open_door += 10.f;
	}
	else {
		if (open_door > 0.f)	open_door -= 10.f;
	}

	if (sw_human_jump) {
		if (human_y >= 0.f) {
			float y_vel = 1.f;
			human_y_vel -= y_vel;
			human_y += human_y_vel;
		}
		else {
			human_y = 0.f;
			human_y_vel = 10.f;
		}
	}
	else {
		human_y = 0.f;
		human_y_vel = 10.f;
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
		open_door = 0.f;
		sw_open_door = false;
		break;

	case 'p':	case 'P':
		View = (VIEW)((View + 1) % 2);
		Reshape(800, 600);
		break;

	case 'o': case 'O':
		sw_open_door = (sw_open_door + 1) % 2;
		break;
	case 'u': case 'U':
		if (Check_Collision(0.f, -5.f)) {
			if(human_z >= -300.f)	human_z -= 5.f;
		}
		human_radian = 90.f;
		break;
	case 'j': case 'J':
		if (Check_Collision(0.f, 5.f)) {
			if (human_z <= 300.f)	human_z += 5.f;
		}
		human_radian = -90.f;
		break;
	case 'h': case 'H':
		if (Check_Collision(-5.f, 0.f)) {
			if (human_x >= -300.f)	human_x -= 5.f;
		}
		human_radian = 180.f;
		break;
	case 'k': case 'K':
		if (Check_Collision(5.f, 0.f)) {
			if (human_x <= 300.f)	human_x += 5.f;
		}
		human_radian = 0.f;
		break;
	}
	cout << human_x << ':' << human_z << endl;
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
	//X�� 
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

	//Y��
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

	//Z��
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
}

void Draw_Tree() {
	glPushMatrix(); // x_z ��ǥ�̵�
	glTranslatef(-220.f, 0.f, -220.f);
	glPushMatrix(); // ���� �̵� 1
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(20.f, 100.f, 20.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // ���� �̵� 1

	glPushMatrix(); // ���� �̵� 2
	glColor4f(0.2f, 0.8f, 0.2f, 1.0f);

	glTranslatef(0.f, 150.f, 0.f);
	glutSolidSphere(50.f, 10.f, 10.f);

	for (float i = 0.f; i < 720.f; i += 45.f) {
		glPushMatrix(); // ��������
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
		glPopMatrix(); // ��������
	}

	glPopMatrix(); // ���� �̵� 2

	glPopMatrix(); // x_z ��ǥ�̵�
}

void Draw_horizontal() {
	glPushMatrix(); // x_z ��ǥ�̵�
	glTranslatef(-220.f, 0.f, 160.f);

	glPushMatrix(); // ö��
	glPushMatrix(); // ���
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(10.f, 100.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix(); // ���
	glTranslatef(0.f, 95.f, 0.f);
	glScalef(5.f, 5.f, 100.f);
	glColor4f(0.1f, 0.1f, 0.4f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // ö��

	glPushMatrix(); // ��� ȸ�� ���� - ö��
	glTranslatef(0.f, 95.f, 0.f);
	glRotatef(horizontal_human_radian_x, 0.f, 0.f, 1.f);

	glPushMatrix(); // ���
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
	glPopMatrix(); // ���

	glPopMatrix(); // ��� ȸ�� ����

	glPopMatrix(); // x_z ��ǥ�̵�
}

void Draw_bench_press() {
	glPushMatrix(); // x_z ��ǥ�̵�
	glTranslatef(220.f, 0.f, 160.f);
	glPushMatrix(); // ��ġ

	glPushMatrix(); // ���
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(10.f, 100.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // ���

	glPushMatrix(); // �������
	glTranslatef(-100.f, 20.f, 0.f);
	glScalef(10.f, 40.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -4.5f; i < 10.f; i += 9.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // �������

	glTranslatef(-40.f, 30.f, 0.f);
	glScalef(120.f, 10.f, 100.f);
	glutSolidCube(1);
	glPopMatrix(); // ��ġ

	glPushMatrix(); // ���
	glTranslatef(25.f, 43.f, 0.f);
	glRotatef(180.f, 0.f, 1.f, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);

	glPushMatrix(); // ��
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glRotatef(-90.f, 0.f, 0.f, 1.f);
	glRotatef(-180.f, 0.f, 1.f, 0.f);
	glTranslatef(-40.f, -15.f, 0.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix(); //��
		glTranslatef(0.f, 15.f, i);
		if (i <= -30.f)	glRotatef(180.f, 0.f, 1.f, 0.f);
		glRotatef(bench_press_human_radian_x, 1.f, 0.f, 0.f);
		glPushMatrix();//�Ʒ���
		glScalef(10.f, 30.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();//�Ʒ���
		glTranslatef(0.f, 15.f, 0.f);
		glPushMatrix(); //����
		glRotatef(-bench_press_human_radian_x * 2, 1.f, 0.f, 0.f);
		glTranslatef(0.f, 15.f, 0.f);
		glScalef(10.f, 30.f, 10.f);
		glutSolidCube(1);
		glPopMatrix(); //����
		glPopMatrix(); //��
	}

	glPushMatrix(); // ����
	glTranslatef(0.f, 60.f - 20 * sin(deg(bench_press_human_radian_x)), 0.f);
	glPushMatrix(); // ��
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix();
		glTranslatef(0.f, 0.f, i);
		glScalef(10.f, 10.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();

	}
	glPopMatrix(); // ��

	glPushMatrix(); // ��
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glScalef(5.f, 5.f, 150.f);
	glutSolidCube(1);
	glPopMatrix(); // ��

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

	glPopMatrix(); // ����
	glPopMatrix(); // ��


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
	glPopMatrix(); // ���

	glPopMatrix(); // x_z ��ǥ�̵�
}

void 	Draw_Runningmachine() {
	glPushMatrix(); // x_z ��ǥ�̵�
	glTranslatef(220.f, 0.f, -160.f);
	glPushMatrix(); // ö��
	glPushMatrix(); // ���
	glTranslatef(0.f, 40.f, 0.f);
	glScalef(10.f, 80.f, 10.f);
	glColor4f(0.6f, 0.4f, 0.2f, 1.0f);
	for (float i = -5.f; i < 10.f; i += 10.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // ���
	glTranslatef(0.f, 75.f, 0.f);
	glScalef(5.f, 5.f, 100.f);
	glColor4f(0.1f, 0.1f, 0.4f, 1.0f);
	glutSolidCube(1);
	glPopMatrix(); // ö��

	glPushMatrix(); // ���׸ӽ�
	glTranslatef(-50.f, 15.f, 0.f);
	glPushMatrix(); //�ٴ�
	glScalef(100.f, 1.f, 80.f);
	glutSolidCube(1);
	glPopMatrix(); // �ٴ�

	for (float i = -1.f; i < 2.f; i += 2.f) {
		for (float j = -1.f; j < 2.f; j += 2.f) {
			glPushMatrix(); // �䷯��
			glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
			glTranslatef(i * 50.f, -7.f, j * 40.f);
			glRotatef(running_machine_human_radian_x, 0.f, 0.f, 1.f);
			glutSolidTorus(2.5f, 5.f, 10, 10);
			glPopMatrix(); //�䷯��
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
	glPopMatrix();// ���׸ӽ�

	glPushMatrix(); // ���
	glTranslatef(-40.f, 115.f, 0.f);

	glPushMatrix(); // ��
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
	glPopMatrix(); // ��


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


	glPushMatrix(); // �ٸ�
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
	glPopMatrix(); // �ٸ�

	glPopMatrix(); // ���

	glPopMatrix(); // x_z ��ǥ�̵�
}

bool Check_Collision(float cx, float cz) {
	if (-350.f < human_x + cx && human_x + cx < -100.f && -350.f < human_z + cz && human_z + cz < -100.f) {
		sw_human_jump = true;
		return false;
	}
	if (-300.f < human_x + cx && human_x + cx < -140.f && 75.f < human_z + cz && human_z + cz < 265.f) {
		sw_human_jump = true;
		return false;
	}
	if (85.f < human_x + cx && human_x + cx < 270.f && 80.f < human_z + cz && human_z + cz < 270.f) {
		sw_human_jump = true;
		return false;
	}
	if (90.f < human_x + cx && human_x + cx < 255.f && -230.f < human_z + cz && human_z + cz < -70.f) {
		sw_human_jump = true;
		return false;
	}
	sw_human_jump = false;
	return true;
}

void Draw_box() {
	//��
	glPushMatrix();
	glTranslatef(0.f, open_door -50.f, 50.f);
	glRotatef(0.f, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 50.f, -50.f);
	Draw_quad(0.f, 0.f, 350.f,
		0.f, 0.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f,
		1.f, 0.f, 1.f,
		1.f, 0.f, 0.f);
	glPopMatrix();
	glPopMatrix();
	//��
	Draw_quad(0.f, 0.f, -350.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 0.f);
	//��
	Draw_quad(-350.f, 0.f, 0.f,
		-90.f, 0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		0.f, 0.f, 0.f);
	//��
	Draw_quad(350.f, 0.f, 0.f,
		-90.f, 0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 0.f, 1.f,
		0.f, 0.f, 1.f);
	//��
	glPushMatrix();
	glTranslatef(0.f, 50.f, -50.f);
	glRotatef(0.f, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, -50.f, 50.f);
	Draw_quad(0.f, 350.f, 0.f,
		-90.f, 1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 0.f);
	glPopMatrix();
	glPopMatrix();
	//�Ʒ�
	Draw_quad(0.f, -350.f, 0.f,
		-90.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 1.f,
		1.f, 0.f, 0.f);
}

void Draw_quad(float trans_x, float trans_y, float trans_z,
	float rotate_rad, float rotate_x, float rotate_y, float rotate_z,
	float LT_R, float LT_G, float LT_B,
	float RT_R, float RT_G, float RT_B,
	float LB_R, float LB_G, float LB_B,
	float RB_R, float RB_G, float RB_B) {

	if (sw_smooth)	glShadeModel(GL_FLAT);
	else glShadeModel(GL_SMOOTH);

	glPushMatrix();
	glTranslatef(trans_x, trans_y, trans_z);
	glRotatef(rotate_rad, rotate_x, rotate_y, rotate_z);
	glBegin(GL_QUADS);
	glColor4f(LT_R, LT_G, LT_B, 1.f);
	glVertex3f(-350.f, 350.f, 0.f);
	glColor4f(RT_R, RT_G, RT_B, 1.f);
	glVertex3f(350.f, 350.f, 0.f);
	glColor4f(LB_R, LB_G, LB_B, 1.f);
	glVertex3f(350.f, -350.f, 0.f);
	glColor4f(RB_R, RB_G, RB_B, 1.f);
	glVertex3f(-350.f, -350.f, 0.f);
	glEnd();
	glPopMatrix();
}

void Draw_human() {
	glPushMatrix(); // ���
	//glTranslatef(-40.f, 115.f, 0.f);

	//glPushMatrix(); // ��
	//glColor4f(1.f, 0.8f, 0.6f, 1.f);
	//glRotatef(-90.f, 0.f, 0.f, 1.f);
	//glRotatef(-180.f, 0.f, 1.f, 0.f);
	//for (float i = -30.f; i < 60.f; i += 60.f) {
	//	glPushMatrix(); // 1
	//	glTranslatef(-40.f, 15.f, i);
	//	glRotatef(0.f, 0.1, 0.1, 0.1);
	//	glScalef(10.f, 40.f, 10.f);
	//	glutSolidCube(1);
	//	glPopMatrix(); // 1
	//}
	//glPopMatrix(); // ��

	glPushMatrix(); // ��
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	for (float i = -1.f; i < 2.f; i += 2.f) {
		glPushMatrix();
		glTranslatef(0.f, -30.f, i * 30.f);
		glRotatef(-i * running_machine_human_radian_x, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -20.f, 0.f);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix(); // ��

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


	glPushMatrix(); // �ٸ�
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
	glPopMatrix(); // �ٸ�

	glPopMatrix(); // ���
}