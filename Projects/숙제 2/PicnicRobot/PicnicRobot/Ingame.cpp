#include "Ingame.h"

VIEW view = Orthographic;
SCENE scene = TOP_VIEW;
Ground ground;
Pillar *pillar[MAX_PILLAR];
Angle Angle_x(90.f), Angle_y, Angle_z;
float move_x = 0, move_y = 0, move_z = 0;
float mx, my, mz;

int select_pillar_index;
bool select_pillar = false;
int click_not_index = -1;

void main(int argc, char **argv) {
	//init_pillar
	for (int i = 0; i < MAX_PILLAR; i++) pillar[i] = NULL;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow("PICNIC_ROBOT");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	if (!view) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 2400.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}


	glPushMatrix();

	if (view)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Objects();

	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(view);
}

void Mouse(int button, int state, int x, int y) {
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (scene == TOP_VIEW) {
			mx = 2 * (x - WINDOW_SIZE_X / 2);
			mz = 2 * (y - WINDOW_SIZE_Z / 2);

			// 마우스-기둥 찾기
			for (int i = 0; i < MAX_PILLAR; i++) {
				if (pillar[i]) {
					if (Collision_Pillar_mouse(pillar[i]->x, pillar[i]->z, mx, mz)) {
						select_pillar_index = i;
						pillar[select_pillar_index]->top_view = false;
						select_pillar = true;
					}
				}
			}

			// 기둥 생성
			if (!(select_pillar)) {
				for (int i = 0; i < MAX_PILLAR; i++) {
					if (pillar[i]) {
						if (Collision_New_Pillar(pillar[i]->x, pillar[i]->z, mx, mz)) {
							if (Collision_Pillar_Pillar(pillar[i]->x, pillar[i]->z, mx, mz)) {
								click_not_index = i;
								pillar[click_not_index]->click_mouse_collision = true;
								break;
							}
							else {
								pillar[i]->click_mouse_collision = false;
							}
						}
					}
				}
				if(Every_Pillar_Not_Click_Collision())	Add_Pillar((float)(mx), (float)(mz));
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (pillar[select_pillar_index])		pillar[select_pillar_index]->top_view = true;
		select_pillar = false;
		if (click_not_index > -1)	pillar[click_not_index]->click_mouse_collision = false;
	}

	glutPostRedisplay();
}

void Motion(int x, int y) {
	if (scene == TOP_VIEW) {
	mx = (float)(2 * (x - WINDOW_SIZE_X / 2));
	mz = (float)(2 * (y - WINDOW_SIZE_Z / 2));
		if (select_pillar) {
			for (int i = 0; i < MAX_PILLAR; i++) {
				if (i == select_pillar_index)	continue;
				if (pillar[i]) {
					if (Collision_Pillar_Pillar(pillar[i]->x, pillar[i]->z, mx, mz)) pillar[i]->top_view_draw_collision = true;
					else	pillar[i]->top_view_draw_collision = false;
				}
			}
			if (Every_Pillar_Not_Collision()) {
				pillar[select_pillar_index]->x = mx;
				pillar[select_pillar_index]->z = mz;
			}
		}
	}

	if (scene == FRONT_VIEW) {
		mx = (float)(2 * (x - WINDOW_SIZE_X / 2));
		mz = (float)(2 * (y - WINDOW_SIZE_Z / 2));
		for (int i = 0; i < MAX_PILLAR; i++) {
			//Collision_Pillar(mx, mz);
		}
	}
	
	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	if (scene == TOP_VIEW) {
		for (int i = 0; i < MAX_PILLAR; i++) {
			if (i == select_pillar_index) continue;
			if (pillar[i]) pillar[i]->top_view = true;
		}
	}

	else {
		for (int i = 0; i < MAX_PILLAR; i++) {
			if (pillar[i]) pillar[i]->top_view = false;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		Change_Scene(TOP_VIEW,0.f,0.f,0.f,90.f, Orthographic);
		break;

	case '2':
		Change_Scene(FRONT_VIEW, 0.f, 0.f, 0.f, 0.f, Orthographic);
		select_pillar_index = -1;
		break;
	case '3':
		Change_Scene(PLAY_VIEW, 0.f, 0.f, 0.f, 0.f, Perspective);
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
		view = (VIEW)((view + 1) % 2);
		Reshape(WINDOW_SIZE_X, WINDOW_SIZE_Y);
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
		glOrtho(-WINDOW_SIZE_X, WINDOW_SIZE_X, -WINDOW_SIZE_Y, WINDOW_SIZE_Y, -WINDOW_SIZE_Z, WINDOW_SIZE_Z);
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

void Change_Scene(SCENE in_scene, float in_move_x, float in_move_y, float in_move_z, float in_Angle_x_radian, VIEW in_view) {
	scene = in_scene;
	move_x = in_move_x;
	move_y = in_move_y;
	move_z = in_move_z;
	Angle_x.radian = in_Angle_x_radian;
	view = in_view;
	Reshape(WINDOW_SIZE_X, WINDOW_SIZE_Y);
}

bool Collision_CIrcles(float x1, float z1, float r1, float x2, float z2, float r2) {
	if (		(int)(x1 - x2) * (int)(x1 - x2) + (int)(z1 - z2) * (int)(z1 - z2) <= (int)(r1 + r2) * (int)(r1 + r2)		)
		return true;
	return false;
}

void Draw_Objects() {
	Draw_Coordinates();
	ground.Draw();
	for (int i = 0; i < MAX_PILLAR; i++) {
		if (pillar[i])	pillar[i]->Draw();
	}
}

void Add_Pillar(float x, float z) {
	int index = Find_Pillar();

	if (index < 0)	return;

	pillar[index] = new Pillar(x, z);
}

int Find_Pillar() {
	for (int i = 0; i < MAX_PILLAR; i++) {
		if (pillar[i] == NULL) return i;
	}
	return -1;
}

bool Collision_Pillar_mouse(float px, float pz, float mx, float my) {
	if (((px - mx) * (px - mx) + (pz - mz) * (pz - mz)) <= (PILLAR_CIRCLE_RADIUS * PILLAR_CIRCLE_RADIUS)) {
		return true;
	}
	return false;
}

bool Collision_New_Pillar(float px, float pz, float mx, float my) {
	if (((px - mx) * (px - mx) + (pz - mz) * (pz - mz)) <= ((PILLAR_CIRCLE_RADIUS + PILLAR_BUILD_ACCESS) * (PILLAR_CIRCLE_RADIUS + PILLAR_BUILD_ACCESS))) {
		return true;
	}
	return false;
}

bool Collision_Pillar_Pillar(float px, float pz, float mx, float mz) {
	if (((px - mx) * (px - mx) + (pz - mz) * (pz - mz)) <= ((PILLAR_CIRCLE_RADIUS * 2 + PILLAR_BUILD_ACCESS) * (PILLAR_CIRCLE_RADIUS * 2 + PILLAR_BUILD_ACCESS))) {
		return true;
	}
	return false;
}

bool Every_Pillar_Not_Collision() {
	for (int i = 0; i < MAX_PILLAR; i++) {
		if (pillar[i]) {
			if (pillar[i]->top_view_draw_collision)	return false;
		}
	}
	return true;
}

bool Every_Pillar_Not_Click_Collision() {
	for (int i = 0; i < MAX_PILLAR; i++) {
		if (pillar[i]) {
			if (pillar[i]->click_mouse_collision)	return false;
		}
	}
	return true;
}