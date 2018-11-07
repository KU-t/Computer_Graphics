#include <GL/freeglut.h>

#define WIDTH 600
#define HEIGHT 600
#define MAX 19

int COUNT = 0;

float ctrlpoints[MAX][3] = { 0, 0, 0 };

void drawScene(void);
void Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

class Vertex
{
private:
public:
	float mx, my;
	float get_mx() { return mx; }
	float get_my() { return my; }
	bool click = false;
	void set_mouse(int x, int y)
	{
		mx = x;
		my = 600 - y;
	}
	void draw()
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2i(mx, my);
		glEnd();
	}
};

void init()
{
	COUNT = 0;
}

void command(char key)
{
	if (key == 'r' || key == 'R')
		init();
}

void draw_curve(int start_vertex)
{
	glEnable(GL_MAP1_VERTEX_3);

	if (COUNT > start_vertex + 3)
	{
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[start_vertex][0]);

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 100; i++)
			glEvalCoord1f((float)i / 100.0);
		glEnd();
		glDisable(GL_MAP1_VERTEX_3);
	}
}

Vertex vert[20];
bool sw = false;

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
	glutMainLoop();
}

void drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < COUNT; ++i)
		vert[i].draw();

	for (int i = 0; i < COUNT; i += 3)
		draw_curve(i);

	glutSwapBuffers();
}

void Reshape(int w = WIDTH, int h = HEIGHT)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(60.0, 1.0, 1.0, 1000.0); // 원근투영
	//glTranslatef(0.0, 0.0, -300.0); // 시야확보

	glOrtho(0, 600, 0, 600, -1, 1); // 직각투영

	glMatrixMode(GL_MODELVIEW);
}

void TimerFunction(int value)
{
	glutTimerFunc(15, TimerFunction, 1);
	for (int i = 0; i < 20; i++) {
		if (vert[i].click == true) {
			sw = true;
			break;
		}
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	command(key);

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
		&& COUNT < MAX)
	{
		
		if (COUNT < 20) {
			for (int i = COUNT; i < COUNT + 1; ++i)
			{
				ctrlpoints[i][0] = x;
				ctrlpoints[i][1] = 600 - y;
			}

			vert[COUNT].set_mouse(x, y);
			COUNT++;
		}

		
		else {
			if (sw == false) {
				for (int i = 0; i < 20; i++) {
					if ((x - 5 <= vert[i].mx) && (vert[i].mx <= x + 5) && (y - 5 <= vert[i].my) && (vert[i].my <= y + 5)) {
						vert[i].click = true;
						break;
					}
				}
			}

			else {
				for (int i = 0; i < 20; i++) {
					if (vert[i].click == true) {
						vert[i].mx = x;
						vert[i].my = y;
						vert[i].click = false;
						sw = false;
					}
				}
			}
		}

	}
	glutPostRedisplay();
}