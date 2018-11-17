#include <gl/freeglut.h>
#include <math.h>

#define W_x 800
#define W_y 600
#define W_z 600
#define POINTS_SIZE 10
#define MAX_POINTS 19
#define deg(x) 3.141592*x/180

enum VIEW { Perspective, Orthographic };

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
void Draw_line(int first_point);
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
	float x, y;
	float exist = false;

	void Draw() {
		float px, py;
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(x, y, 0.f);
		glutSolidSphere(POINTS_SIZE, 10.f, 10.f);
		glPopMatrix();
	}
};

Point *point[19];
Angle Angle_x, Angle_y, Angle_z;
float move_x = 0, move_y = 0, move_z = 0;
int select_index;
bool select_point = false;

void main(int argc, char **argv) {
	for (int i = 0; i < MAX_POINTS; i++) {
		point[i] = NULL;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("28");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 300.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
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
		if(point[i])	point[i]->Draw();
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
	if (point[18]) Draw_line(15);
	if (point[15]) Draw_line(12);
	if (point[12]) Draw_line(9);
	if (point[9]) Draw_line(6);
	if (point[6]) Draw_line(3);
	if (point[3]) Draw_line(0);

}

void Draw_line(int first_point) {
	GLfloat ctrlpoints[4][3] ={
		{ point[first_point]->x,point[first_point]->y,0 },
		{ point[first_point + 1]->x,point[first_point + 1]->y,0 },
		{ point[first_point + 2]->x,point[first_point + 2]->y,0 },
		{ point[first_point + 3]->x,point[first_point + 3]->y,0 }
	};
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100.0, 0.0, 1.0); 
	glEvalMesh1 (GL_LINE, 0, 100); 

	glDisable(GL_MAP1_VERTEX_3);
	
	glPointSize (5.0); 
	glColor4f (0.f, 0.f, 0.f, 1.f); 
	glBegin(GL_POINTS); 
	for (int i = 0; i < 4; i++ ) 
		glVertex3fv (&ctrlpoints[i][0]); 
	glEnd ();
}

bool Collision_circle(float px, float py, float mx, float my) {
	if (((px - mx) * (px - mx) + (py - my) * (py - my)) <= (POINTS_SIZE * POINTS_SIZE)) {
		return true;
	}
	return false;
}