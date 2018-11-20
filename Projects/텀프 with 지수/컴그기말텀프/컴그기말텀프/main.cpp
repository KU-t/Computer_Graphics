#include "main.h"



VIEW View = Perspective;

ICE *ice[MAX_ICE];
DONUT *donut[MAX_ICE];
LARVA *larva[MAX_ICE];
FLOWER *flower[MAX_ICE];

float move_x = 0, move_y = 0, move_z = 0;
Angle Angle_x, Angle_y, Angle_z;

bool SWITCH_Light0 = false;
GLfloat AmbientLight0[] = { 1.f, 0.f, 0.f, 1.f }; // 빛의 세기 + 빛의 색
GLfloat DiffuseLight0[] = { 0.7f, 0.7f, 0.0f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 1.0, 0.0, 0.0, 1.0 }; // 하이라이트 색
GLfloat lightPos0[] = { LIGHT_X, LIGHT_Y, LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 

void main(int argc, char **argv) {
	//init
	for (int i = 0; i < MAX_ICE; i++) ice[i] = NULL;
	for (int i = 0; i < MAX_ICE; i++) donut[i] = NULL;
	for (int i = 0; i < MAX_ICE; i++) larva[i] = NULL;
	for (int i = 0; i < MAX_ICE; i++) flower[i] = NULL;

	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("기말텀프");
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
	glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// 직각투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//그리기
	{

	}
	


	glPushMatrix();

	// 원근투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(move_x, move_y, move_z + 900.f, 0.f, 0.f, 300.f, 0.f, 1.f, 0.f);

	glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);
	
	// LIGHT0
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		if (SWITCH_Light0)		glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
		

	}

	//그리기
	{
		glPushMatrix();
		glTranslatef(0.f,0.f,3000.f);
		glutSolidSphere(100, 30, 30);
		glPopMatrix();
		
		Draw_Coordinates();
		Draw_bottom();

		for (int i = 0; i < MAX_ICE; i++) {
			if (ice[i]) {
				ice[i]->Draw();
				//ice[i]->Draw_Collision_rect();
			}
			if (donut[i]) {
				//donut[i]->Draw();
				//donut[i]->Draw_Collision_rect();
			}
			if (larva[i]) {
				larva[i]->Draw();
				//larva[i]->Draw_Collision_rect();
			}
			if (flower[i]) {
				flower[i]->Draw();
				//flower[i]->Draw_Collision_rect();
			}
		}
	}
	//Draw_bottom();

	glPopMatrix();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
}

void Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Add_BEE(move_x, move_y, move_z);
	}
	glutPostRedisplay();
}

void Motion(int x, int y){
	float mx = 2 * (x - W_x / 2);
	float my = 2 * (y - W_y / 2);
	cout << mx << my << endl;
	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	for (int i = 0; i < MAX_ICE; i++) {
		if (ice[i]) 
			ice[i]->Update();
		
		if (donut[i]) 
			donut[i]->Update();
		

		if (larva[i]) 
			larva[i]->Update();
		
		if (flower[i])
			flower[i]->Update();

	}
	glutPostRedisplay();
	glutTimerFunc(1000/60, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		SWITCH_Light0 = (SWITCH_Light0 + 1) % 2;
		break;
	case 'x':   case 'X':
		Angle_x.sw = (Angle_x.sw + 1) % 2;
		break;
	case 'y':   case 'Y':
		Angle_y.sw = (Angle_y.sw + 1) % 2;
		break;
	case 'z':   case 'Z':
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
	GLfloat Object1[] = { 0.7f, 0.7f, 0.7f, 1.f };
	GLfloat Object_specref1[] = { 0.5f, 0.3f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	for (float rad = 0.f; rad < 360.f; rad += 90.f) {

	glPushMatrix();
	glRotatef(rad,0.f,0.f,1.f);
	glTranslatef(0.f, -W_y, -MAP_Z_SIZE / 2);
	glScalef(2 * W_x, 1.f,MAP_Z_SIZE);
	glColor3f(1.f, rad / 100.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();

	}
}


void Add_BEE(float in_x, float in_y, float in_z) {
	int index = FindslotBee();
	if (index < 0)	return;

	 ice[index] = new ICE(in_x, in_y, in_z);
	 //donut[index] = new DONUT(in_x, in_y, in_z);
	 larva[index] = new LARVA(in_x, in_y, in_z);
	 flower[index] = new FLOWER(in_x, in_y, in_z);

}

int FindslotBee() {
	for (int i = 0; i < MAX_ICE; i++) {
		if (ice[i] == NULL)	return i;
		//if (donut[i] == NULL)return i;
		if (larva[i] == NULL)return i;
		if (flower[i] == NULL)return i;

	}
	return -1;
}