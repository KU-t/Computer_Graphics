#include <gl/freeglut.h>
#include <math.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define deg(x) x*3.141592/180

enum VIEW { Perspective, Orthographic };
enum ANIMATION {increase, decrease};
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

void main(int argc, char **argv) {
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
	glEnable(GL_DEPTH_TEST);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y + 300.f, move_z + 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Coordinates();
	Draw_bottom();

	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	Draw_Tree();
	Draw_horizontal();
	Draw_bench_press();

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
	glutSolidSphere(50.f,10.f,10.f);
	
	for (float i = 0.f; i < 720.f; i += 45.f) {
		glPushMatrix(); // ��������
		glScalef(tree_size,tree_size,tree_size);
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
	glColor4f(0.2f, 0.2f, 0.8f, 1.f);
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
	glTranslatef(0.f,-20.f,0.f);
	glColor4f(0.f,0.f,0.f,0.f);
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
	glTranslatef(0.f,-50.f,0.f);
	glScalef(15.f,40.f,50.f);
	glColor4f(0.7f,0.f,0.f,1.f);
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
	glColor4f(0.2f, 0.2f, 0.8f, 1.f);
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
	glColor4f(0.2f, 0.2f, 0.8f, 1.f);
	for (float i = -4.5f; i < 10.f; i += 9.f) {
		glPushMatrix(); //
		glTranslatef(0.f, 0.f, i);
		glutSolidCube(1);
		glPopMatrix(); //
	}
	glPopMatrix(); // �������

	glTranslatef(-40.f,30.f,0.f);
	glScalef(120.f,10.f,100.f);
	glutSolidCube(1);
	glPopMatrix(); // ��ġ

	glPushMatrix(); // ���
	glTranslatef(25.f, 43.f, 0.f);
	glRotatef(180.f,0.f,1.f,0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);
	
	glPushMatrix(); // ��
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glRotatef(-90.f, 0.f, 0.f, 1.f);
	glRotatef(-180.f, 0.f, 1.f, 0.f);
	glTranslatef(-40.f, -15.f, 0.f);
	for (float i = -30.f; i < 60.f; i += 60.f) {
		glPushMatrix(); //��
		glTranslatef(0.f,15.f,i);
		if (i <= -30.f)	glRotatef(180.f, 0.f, 1.f, 0.f);
		glRotatef(bench_press_human_radian_x, 1.f, 0.f,0.f);
		glPushMatrix();//�Ʒ���
		glScalef(10.f, 30.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();//�Ʒ���
		glTranslatef(0.f, 15.f, 0.f);
		glPushMatrix(); //����
		glRotatef(-bench_press_human_radian_x*2, 1.f, 0.f, 0.f);
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