#include <gl/freeglut.h>
#include <time.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define nSides 10
#define nRings 10
#define slices 10
#define stacks 10

enum OBJECT_SHAPE{cone,cube,sphere,torus,teapot};
enum OBJECT_SPIN{_X,_Y,_Z};

class OBJECT{
public:
	float x, y, z;
	bool exist;
	float angle;
	float scale;
	OBJECT_SHAPE shape;
	OBJECT_SPIN spin;
	double size;
	double height;

	OBJECT() {
		exist = false;
		angle = 0.f;
		scale = 1.f;
	}

	void Draw_OBJECT() {

		glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(scale, scale, scale);
		switch (spin){
		case _X:
			glRotatef(angle, x, y, 0.f);
			break;

		case _Y:
			glRotatef(angle, 0.f, y, z);
			break;

		case _Z:
			glRotatef(angle, x, 0.f, z);
			break;
		}

		switch (shape) {
		case cone:
			glutWireCone(size, height, slices, stacks);
			break;

		case cube:
			glutWireCube(size);
			break;

		case sphere:
			glutWireSphere(size, slices, stacks);
			break;

		case torus:
			glutWireTorus(size / 2, size, nSides, nRings);
			break;

		case teapot:
			glutWireTeapot(size);
			break;
		
		}

		glPopMatrix();
		glutPostRedisplay();
	}
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Initalization(float x, float y);

float ANGLE = 2.f;
int count = 0;
OBJECT *obj;

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	obj = new OBJECT[20];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("3¿Â Ω«Ω¿ 14");
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

	for (int i = 0; i < 20; i++) {
		if((obj+i)->exist)
			(obj + i)->Draw_OBJECT();
	}
	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, W_x, 0, W_y, -W_z, 0);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
		Initalization(x, y);
		count = (count + 1) % 20;
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	for (int i = 0; i < 20; i++) {
		if ((obj + i)->exist) {
			(obj+i)->angle += ANGLE;
			(obj + i)->scale -= 0.005f;
			if ((obj + i)->scale <= 0.f) {
				(obj + i)->scale = 0.f;
				(obj + i)->exist = false;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == 'c' || key == 'C')	ANGLE = -ANGLE;
	else if (key == 'q' || key == 'Q')	exit(true);
}

//cone,cube,sphere,torus,teapot


void Initalization(float x, float y) {
	(obj + count)->exist = true;
	(obj + count)->x = x;
	(obj + count)->y = W_y - y;
	(obj + count)->z = 100;
	(obj + count)->scale = 1.f;
	(obj + count)->shape = (OBJECT_SHAPE)(rand() % 5);
	(obj + count)->spin = (OBJECT_SPIN)(rand() % 3);

	switch ((obj + count)->shape){
	case cone:
		(obj + count)->size = 100;
		(obj + count)->height = 100;
		break;

	case cube:
		(obj + count)->size = 100;
		break;

	case sphere:
		(obj + count)->size = 100;
		break;

	case torus:
		(obj + count)->size = 50;
		break;

	case teapot:
		(obj + count)->size = 70;
		break;
	}
}