#include "Ingame.h"

VIEW view = Orthographic;
SCENE scene = TOP_VIEW;
Ground ground;
Pillar *pillar[MAX_ROCK_PILLAR];
Rail rail;
Human *human[MAX_HUMAN];
Bullet *bullet[MAX_BULLET];
Snow *snow[MAX_SNOW];

Angle Angle_x(90.f), Angle_y, Angle_z;

float move_x = 0, move_y = 0, move_z = 300;

float mx, my, mz;

int select_pillar_index;
bool select_pillar = false;
int click_not_index = -1;

int draw_Rollercoaster_pillar_num = 0;
int draw_Rollercoaster_pillar_t = 0;

SPEED game_speed = X1;
VIEWER view_point = NORMAL;

int wether = 0;


void main(int argc, char **argv) {
	//init_pillar
	srand((unsigned)time(NULL));

	for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
		if (i < MAX_ROCK) {
			int x = (rand() % (WINDOW_SIZE_X * 2)) - WINDOW_SIZE_X;
			int z = (rand() % (WINDOW_SIZE_Z * 2)) - WINDOW_SIZE_Z;
			pillar[i] = new Pillar(x, z, true);
		}
		else pillar[i] = NULL;
	}

	for (int i = 0; i < MAX_HUMAN; i++) {
		human[i] = new Human();
		human[i]->x = (i - 1) * (WINDOW_SIZE_X / 2);
		human[i]->z = (i - 1) * (WINDOW_SIZE_Z / 2);
	}
	human[MAX_HUMAN - 1]->choice = true;

	for (int i = 0; i < MAX_SNOW; i++) snow[i] = NULL;

	for (int i = 0; i < MAX_BULLET; i++) bullet[i] = NULL;
			
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
		if (view_point == NORMAL) {
			glLoadIdentity();
			gluLookAt(move_x, move_y, move_z + 1200.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
		}
		else if (view_point == HUMAN && human[MAX_HUMAN - 1]) {
			int x, z;
			if (human[MAX_HUMAN - 1]->rad_move == 0) {
				x = 1000000;
				z = 0;
			}
			else if (human[MAX_HUMAN - 1]->rad_move == 90) {
				x = 0;
				z = -1000000;
			}
			else if (human[MAX_HUMAN - 1]->rad_move == 180) {
				x = -1000000;
				z = 0;
			}
			else if (human[MAX_HUMAN - 1]->rad_move == 270) {
				x = 0;
				z = 1000000;
			}
			glLoadIdentity();
			gluLookAt(human[MAX_HUMAN - 1]->x, human[MAX_HUMAN - 1]->y + 10, human[MAX_HUMAN - 1]->z, x, 0.f, z, 0.f, 1.f, 0.f);
		}
		else if (view_point == ROLLERCOSTER) {
			glLoadIdentity();
			gluLookAt(rail.x[1], rail.y[1] + 100 - WINDOW_SIZE_Y / 2, rail.z[1], rail.x[0], rail.y[0] + 100 - WINDOW_SIZE_Y / 2, rail.z[0], 0.f, 1.f, 0.f);
		}
	}


	glPushMatrix();

	if (view)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);

	Draw_Objects();

	Draw_Pillars_Spline();
	if (scene == PLAY_VIEW) {
		if (pillar[12]) {
			Draw_rollercoaster();
			Draw_tunnel();
		}

		for (int i = 0; i < MAX_HUMAN; i++) {
			if(human[i])	human[i]->Draw();
		}

		for (int i = 0; i < MAX_SNOW; i++) {
			if (snow[i])	snow[i]->Draw(wether);
		}
	
		for (int i = 0; i < MAX_BULLET; i++) {
			if (bullet[i])	bullet[i]->Draw();
		}
	}

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

			// 옮길 기둥 찾기
			for (int i = MAX_ROCK; i < MAX_ROCK_PILLAR; i++) {
				if (pillar[i]) {
					if (!pillar[i]->rock) {
						if (Collision_Pillar_mouse(TOP_VIEW, pillar[i]->x, pillar[i]->z, mx, mz)) {
							select_pillar_index = i;
							pillar[select_pillar_index]->top_view = false;
							select_pillar = true;
						}
					}
				}
			}

			// 기둥 생성
			if (!(select_pillar)) {
				for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
					if (pillar[i]) {
						if (Collision_New_Pillar(pillar[i]->x, pillar[i]->z, mx, mz,pillar[i]->access_size)) {
							if (Collision_Pillar_Pillar(pillar[i]->x, pillar[i]->z, mx, mz,pillar[i]->access_size)) {
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
				if (Every_Pillar_Not_Click_Collision())	Add_Pillar((float)(mx), (float)(mz));
			}
		}

		if (scene == FRONT_VIEW) {
			mx = 2 * (x - WINDOW_SIZE_X / 2);
			my = -2 * (y - WINDOW_SIZE_Y / 2);
			//cout << pillar[select_pillar]->y << "////" << my << endl;
			for (int i = MAX_ROCK; i < MAX_ROCK_PILLAR; i++) {
				if (pillar[i]) {
					if (Collision_Pillar_mouse(FRONT_VIEW, pillar[i]->x, pillar[i]->y, mx, my)) {
						pillar[i]->select_front_view = true;
						select_pillar_index = i;
						select_pillar = true;
						break;
					}
				}
			}
			cout << endl;
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (scene == TOP_VIEW) {

			if (pillar[select_pillar_index])		pillar[select_pillar_index]->top_view = true;
			if (click_not_index > -1)	pillar[click_not_index]->click_mouse_collision = false;
		}

		else if (scene == FRONT_VIEW) {
			if (-1 < select_pillar_index) {
				if (pillar[select_pillar_index])
					pillar[select_pillar_index]->select_front_view = false;
			}
			select_pillar_index = -1;
		}
		select_pillar = false;
	}

	glutPostRedisplay();
}

void Motion(int x, int y) {
	if (scene == TOP_VIEW) {
		mx = (float)(2 * (x - WINDOW_SIZE_X / 2));
		mz = (float)(2 * (y - WINDOW_SIZE_Z / 2));
		if (select_pillar) {
			for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
				if (i == select_pillar_index)	continue;
				if (pillar[i]) {
					if (Collision_Pillar_Pillar(pillar[i]->x, pillar[i]->z, mx, mz,pillar[i]->access_size)) pillar[i]->top_view_draw_collision = true;
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
		my = (float)(-2 * (y - WINDOW_SIZE_Y / 4));
		if (select_pillar){
			if (-1 < select_pillar_index) {
				if (pillar[select_pillar_index]) {
					if (-750.f < my && my < 300.f)
						pillar[select_pillar_index]->y = my + WINDOW_SIZE_Y;
				}
			}
		}
	}
	
	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();
	if (scene == TOP_VIEW) {
		for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
			if (i == select_pillar_index) continue;
			if (pillar[i]) pillar[i]->top_view = true;
		}
	}

	if (scene == PLAY_VIEW) {
		int pillar_count = 0;
		for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
			if (pillar[i]) pillar_count = i + 1;
		}

		for (int i = 0; i < 5; i++) {
			rail.draw_Rollercoaster_pillar_t[i]  += game_speed ;
			if (rail.draw_Rollercoaster_pillar_t[i] >= SPLINE_COUNT) {
				rail.draw_Rollercoaster_pillar_t[i] = rail.draw_Rollercoaster_pillar_t[i] % SPLINE_COUNT;
				rail.draw_Rollercoaster_pillar_num[i] = (rail.draw_Rollercoaster_pillar_num[i] + 1) % (pillar_count - MAX_ROCK);
			}
		}

		for (int i = 0; i < MAX_HUMAN - 1; i++) {
			if (human[i]) {
				
				int rad_random = (rand() % 60) - 30;
				int copy_rad = human[i]->rad_move;
				float copy_x = human[i]->x, copy_z = human[i]->z;

				copy_rad = ((int)(copy_rad)+rad_random) % 360;

				copy_x += HUMAN_SPEED * cos(deg(copy_rad));
				if (-WINDOW_SIZE_X > copy_x)	copy_x = -WINDOW_SIZE_X;
				if (copy_x > WINDOW_SIZE_X)	copy_x = WINDOW_SIZE_X;

				copy_z -= HUMAN_SPEED * sin(deg(copy_rad));
				if (-WINDOW_SIZE_Z > copy_z)	copy_z = -WINDOW_SIZE_Z;
				if (copy_z > WINDOW_SIZE_Z)	copy_z = WINDOW_SIZE_Z;
				
				bool move_check = true;

				for (int j = 0; j < MAX_PILLAR; j++) {
					if (pillar[j]) {
						if (Collision_CIrcles(pillar[j]->x, pillar[j]->z, PILLAR_CIRCLE_RADIUS,human[i]->x, human[i]->z, HUMAN_SIZE)) move_check = false;
					}
				}

				if (move_check) {
					human[i]->rad_move = copy_rad;
					human[i]->x = copy_x;
					human[i]->z = copy_z;
				}
				else {
					human[i]->rad_move += 180.f;
					human[i]->x += -1 * HUMAN_SPEED * cos(deg(rad_random));
					human[i]->z += -1 * HUMAN_SPEED * sin(deg(rad_random));;
				}
			}
		}
		for (int i = 0; i < MAX_HUMAN; i++) {
			if (human[i]) human[i]->Update();
		}

		if (Collision_CIrcles(human[0]->x, human[0]->z, HUMAN_SIZE, human[MAX_HUMAN - 1]->x, human[MAX_HUMAN - 1]->z, HUMAN_SIZE)) {
			human[0]->x = human[MAX_HUMAN - 1]->x - (HUMAN_SIZE * 3 / 2);
			human[0]->z = human[MAX_HUMAN - 1]->z;
			human[0]->rad_move = human[MAX_HUMAN - 1]->rad_move;
		}

		if (Collision_CIrcles(human[1]->x, human[1]->z, HUMAN_SIZE, human[MAX_HUMAN - 1]->x, human[MAX_HUMAN - 1]->z, HUMAN_SIZE)) {
			human[1]->x = human[MAX_HUMAN - 1]->x + (HUMAN_SIZE * 3 / 2);
			human[1]->z = human[MAX_HUMAN - 1]->z;
			human[1]->rad_move = human[MAX_HUMAN - 1]->rad_move;
		}

		for (int i = 0; i < MAX_BULLET; i++) {
			if (bullet[i]) {
				bullet[i]->Update();
				if (Collision_bullet(bullet[i]->x, bullet[i]->z))	bullet[i] = NULL;
			}
		}

		Update_Snow();
	}

	

	if(scene != TOP_VIEW){
		for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
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
		select_pillar = false;
		break;

	case '2':
		if (pillar[14]) {
			Change_Scene(FRONT_VIEW, 0.f, 0.f, 0.f, 0.f, Orthographic);
			select_pillar_index = -1;
			select_pillar = false;
		}
		break;
	
	case '3':
		if (pillar[14]) {
			Change_Scene(PLAY_VIEW, 0.f, 0.f, 0.f, 0.f, Perspective);
		}
		break;

	case '4':
		if (scene == PLAY_VIEW) {
			view_point = NORMAL;
			move_x = 0.f;
			move_y = 0.f;
			move_z = 0.f;
			Angle_x.radian = 0.f;
			Angle_y.radian = 0.f;
			Angle_z.radian = 0.f;
		}
		break;

	case '5':
		if (scene == PLAY_VIEW) {
			view_point = HUMAN;
			move_x = 0.f;
			move_y = 0.f;
			move_z = 0.f;
			Angle_x.radian = 0.f;
			Angle_y.radian = 0.f;
			Angle_z.radian = 0.f;
		}
		break;

	case '6':
		if (scene == PLAY_VIEW) {
			view_point = ROLLERCOSTER;
			move_x = 0.f;
			move_y = 0.f;
			move_z = 0.f;
			Angle_x.radian = 0.f;
			Angle_y.radian = 0.f;
			Angle_z.radian = 0.f;
		}

		break;

	case '8':
		if (scene == PLAY_VIEW)
			wether = 0;
		break;

	case '9':
		if (scene == PLAY_VIEW)
			wether = 1;		
		break;

	case '0':
		if (scene == PLAY_VIEW)
			wether = 2;		
		break;

	case 'x':	case 'X':
		if (scene == PLAY_VIEW)
			Angle_x.sw = (Angle_x.sw + 1) % 2;
		break;
	case 'y':	case 'Y':
		if (scene == PLAY_VIEW)
			Angle_y.sw = (Angle_y.sw + 1) % 2;
		break;
	case 'z':	case 'Z':
		if (scene == PLAY_VIEW)
			Angle_z.sw = (Angle_z.sw + 1) % 2;
		break;
	case 'a': case 'A':
		if (scene == PLAY_VIEW)
			move_x -= 10;
		break;

	case 'd': case 'D':
		if (scene == PLAY_VIEW)
			move_x += 10;
		break;

	case 'w': case 'W':
		if (scene == PLAY_VIEW)
			move_y += 10;
		break;

	case 's': case 'S':
		if (scene == PLAY_VIEW)
			move_y -= 10;
		break;
	case '+': case '=':
		if (scene == PLAY_VIEW)
			move_z += 10;
		break;

	case '-': case '_':
		if (scene == PLAY_VIEW)
			move_z -= 10;
		break;

	case 'i': case 'I':
		if (scene == PLAY_VIEW) {
			move_x = 0.f;
			move_y = 0.f;
			move_z = 0.f;
			Angle_x.radian = 0.f;
			Angle_y.radian = 0.f;
			Angle_z.radian = 0.f;
		}
		break;

	case 'p':	case 'P':
		view = (VIEW)((view + 1) % 2);
		Reshape(WINDOW_SIZE_X, WINDOW_SIZE_Y);
		break;

	case '[':
		if (game_speed == X7)	game_speed = X5;
		else if (game_speed == X5)	game_speed = X3;
		else if (game_speed == X3)	game_speed = X1;
		break;

	case ']':
		if (game_speed == X1)	game_speed = X3;
		else if (game_speed == X3)	game_speed = X5;
		else if (game_speed == X5)	game_speed = X7;
		break;

	case 'u': case 'U':
		if (scene == PLAY_VIEW) {
			if (human[MAX_HUMAN - 1]) {
				human[MAX_HUMAN - 1]->z -= HUMAN_CONTROL_SPEED;
				human[MAX_HUMAN - 1]->rad_move = 90;
			}
		}
		break;

	case 'j': case 'J':
		if (scene == PLAY_VIEW) {
			if (human[MAX_HUMAN - 1]) {
				human[MAX_HUMAN - 1]->z += HUMAN_CONTROL_SPEED;
				human[MAX_HUMAN - 1]->rad_move = 270;
			}
		}
		break;

	case 'h': case 'H':
		if (scene == PLAY_VIEW) {
			if (human[MAX_HUMAN - 1]) {
				human[MAX_HUMAN - 1]->x -= HUMAN_CONTROL_SPEED;
				human[MAX_HUMAN - 1]->rad_move = 180;
			}
		}
		break;

	case 'k': case 'K':
		if (scene == PLAY_VIEW) {
			if (human[MAX_HUMAN - 1]) {
				human[MAX_HUMAN - 1]->x += HUMAN_CONTROL_SPEED;
				human[MAX_HUMAN - 1]->rad_move = 0;
			}
		}
		break;

	case 'b': case 'B':
		if (scene == PLAY_VIEW) Add_Bullet(human[MAX_HUMAN -1]->x, human[MAX_HUMAN - 1]->z, human[MAX_HUMAN - 1]->rad_move);
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
	for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
		if (pillar[i])	pillar[i]->Draw();
	}
}

void Add_Pillar(float x, float z) {
	int index = Find_Pillar();

	if (index < 0)	return;

	pillar[index] = new Pillar(x, z, false);
}

int Find_Pillar() {
	for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
		if (pillar[i] == NULL) return i;
	}
	return -1;
}

void Add_Bullet(float x, float z, float rad) {
	int index = Find_Bullet();

	if (index < 0)	return;

	bullet[index] = new Bullet(x, -350, z, rad);
}

int Find_Bullet() {
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i] == NULL) return i;
	}
	return -1;
}

bool Collision_Pillar_mouse(SCENE scene, float px, float pyz, float mx, float myz) {
	if (scene == TOP_VIEW) {
		if (((px - mx) * (px - mx) + (pyz - myz) * (pyz - myz)) <= (PILLAR_CIRCLE_RADIUS * PILLAR_CIRCLE_RADIUS)) {
			return true;
		}
		return false;
	}
	if (scene == FRONT_VIEW) {
		if (mx < px - PILLAR_CIRCLE_RADIUS)		return false;
		if (px + PILLAR_CIRCLE_RADIUS < mx)	return false;
		if (pyz < -WINDOW_SIZE_Y/2)	return false;
		if ((-WINDOW_SIZE_Y / 2) + (pyz * 2) < myz)	return false;

		return true;
	}
}

bool Collision_New_Pillar(float px, float pz, float mx, float mz, float in_access_size) {
	if (((px - mx) * (px - mx) + (pz - mz) * (pz - mz)) <= ((PILLAR_CIRCLE_RADIUS + in_access_size) * (PILLAR_CIRCLE_RADIUS + in_access_size))) {
		return true;
	}
	return false;
}

bool Collision_Pillar_Pillar(float px, float pz, float mx, float mz, float p_access_size) {
	if (((px - mx) * (px - mx) + (pz - mz) * (pz - mz)) <= ((PILLAR_CIRCLE_RADIUS * 2 + p_access_size) * (PILLAR_CIRCLE_RADIUS * 2 + p_access_size))) {
		return true;
	}
	return false;
}

bool Every_Pillar_Not_Collision() {
	for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
		if (pillar[i]) {
			if (pillar[i]->top_view_draw_collision)	return false;
		}
	}
	return true;
}

bool Every_Pillar_Not_Click_Collision() {
	for (int i = 0; i < MAX_ROCK_PILLAR; i++) {
		if (pillar[i]) {
			if (pillar[i]->click_mouse_collision)	return false;
		}
	}
	return true;
}

void Draw_Spline(Pillar *P1, Pillar *P2, Pillar *P3, Pillar *P4, int count) {
	float px[4] = { P1->x, P2->x, P3->x, P4->x };
	float py[4] = { P1->y, P2->y, P3->y, P4->y };
	float pz[4] = { P1->z, P2->z, P3->z, P4->z };
	int num = count - MAX_ROCK;
	int k = 0;

	for (int i = 0; i < SPLINE_COUNT; i += 2) {
		float t = i * 0.01;
		float x = ((-t * t * t + 2 * t * t - t)*px[0] + (3 * t * t * t - 5 * t * t + 2)*px[1] + (-3 * t * t * t + 4 * t * t + t)*px[2] + (t * t * t - t * t)*px[3]) / 2;
		float y = ((-t * t * t + 2 * t * t - t)*py[0] + (3 * t * t * t - 5 * t * t + 2)*py[1] + (-3 * t * t * t + 4 * t * t + t)*py[2] + (t * t * t - t * t)*py[3]) / 2;
		float z = ((-t * t * t + 2 * t * t - t)*pz[0] + (3 * t * t * t - 5 * t * t + 2)*pz[1] + (-3 * t * t * t + 4 * t * t + t)*pz[2] + (t * t * t - t * t)*pz[3]) / 2;

		float nt = (i+1) * 0.01;
		float nx = ((-nt * nt * nt + 2 * nt * nt - nt)*px[0] + (3 * nt * nt * nt - 5 * nt * nt + 2)*px[1] + (-3 * nt * nt * nt + 4 * nt * nt + nt)*px[2] + (nt * nt * nt - nt * nt)*px[3]) / 2;
		float ny = ((-nt * nt * nt + 2 * nt * nt - nt)*py[0] + (3 * nt * nt * nt - 5 * nt * nt + 2)*py[1] + (-3 * nt * nt * nt + 4 * nt * nt + nt)*py[2] + (nt * nt * nt - nt * nt)*py[3]) / 2;
		float nz = ((-nt * nt * nt + 2 * nt * nt - nt)*pz[0] + (3 * nt * nt * nt - 5 * nt * nt + 2)*pz[1] + (-3 * nt * nt * nt + 4 * nt * nt + nt)*pz[2] + (nt * nt * nt - nt * nt)*pz[3]) / 2;

		float xx = nx - x;
		float yy = ny - y;
		float zz = nz - z;

		float spline_y = rad(acos(xx / sqrt(xx*xx+zz*zz)));
		float spline_z = rad(acos( sqrt(xx*xx + zz * zz) / sqrt(xx*xx + zz * zz + yy * yy) )  );


		glPushMatrix();
		glTranslatef(x, y - WINDOW_SIZE_Y/2 + 30, z);
		glRotatef(-spline_y, 0.f, 1.f, 0.f);
		glRotatef(spline_z, 0.f, 0.f, 1.f);
		rail.Draw();

		if (num == 2) {
			if (25 < i && 0 <= k && k < 25) {
				rail.tunnel_x[k] = x;
				rail.tunnel_y[k] = y;
				rail.tunnel_z[k] = z;
				rail.tunnel_spline_y[k] = spline_y;
				rail.tunnel_spline_z[k++] = spline_z;
			}
		}

		for (int j = 0; j < 5; j++) {
			if (num == rail.draw_Rollercoaster_pillar_num[j]) {
				if (i == rail.draw_Rollercoaster_pillar_t[j]) {
					rail.x[j] = x;
					rail.y[j] = y;
					rail.z[j] = z;
					rail.spline_y[j] = spline_y;
					rail.spline_z[j] = spline_z;
				}
			}
		}
		glPopMatrix();
	}
}

void Draw_Pillars_Spline() {
	if (pillar[MAX_ROCK]) {
		int pillar_exist = 0;
		for (int i = MAX_ROCK; i < MAX_ROCK_PILLAR; i++) {
			if (pillar[i]) {
				pillar_exist++;
			}
			else
				break;
		}

		for (int i = 0; i < pillar_exist; i++) {
			int p1 = (pillar_exist + i) % pillar_exist + MAX_ROCK;
			int p2 = (pillar_exist + i + 1) % pillar_exist + MAX_ROCK;
			int p3 = (pillar_exist + i + 2) % pillar_exist + MAX_ROCK;
			int p4 = (pillar_exist + i + 3) % pillar_exist + MAX_ROCK;
			Draw_Spline(pillar[p1], pillar[p2], pillar[p3], pillar[p4],p2);
		}
	}
}

void Draw_rollercoaster() {
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(rail.x[i], rail.y[i] - WINDOW_SIZE_Y / 2 + 30, rail.z[i]);
		glRotatef(rail.spline_y[i], 0.f, 1.f, 0.f);
		glRotatef(rail.spline_z[i], 0.f, 0.f, 1.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		if (i == 0) {

		glColor4f(0.f, 1.f, 0.f, 1.f);
		glutSolidSphere(30, 30, 30);

		glPushMatrix();
		glTranslatef(0.f, 15.f, 0.f);
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glutSolidSphere(20, 30, 30);
		glPopMatrix();
		}

		else {
			glColor4f(0.f, 1.f, 0.f, 1.f);
			glutSolidSphere(30 - 2 * i, 30, 30);
		}
		glPopMatrix();
	}
}

void Draw_tunnel() {
	for (int i = 0; i < MAX_TUNNEL; i++) {
		glPushMatrix();
		glTranslatef(rail.tunnel_x[i], rail.tunnel_y[i] - WINDOW_SIZE_Y / 2 + 30, rail.tunnel_z[i]);
		glRotatef(rail.tunnel_spline_y[i], 0.f, 1.f, 0.f);
		glRotatef(rail.tunnel_spline_z[i], 0.f, 0.f, 1.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		glColor4f(0.2f, 0.125f, 0.f, 1.f);
		glutSolidTorus(20, 100, 10, 10);
			
		glPopMatrix();
	}
}

void Update_Snow() {
	for (int i = 0; i < MAX_SNOW; i++) {
		if (snow[i] == NULL)	snow[i] = new Snow;
		if (snow[i]) {
			snow[i]->y -= 10.f;
			if (snow[i]->y <= 0.f) snow[i] = NULL;
		}
	}
}

bool Collision_bullet(float bx, float bz) {
	if (bx <= -WINDOW_SIZE_X)	return true;
	if (WINDOW_SIZE_X <= bx)	return true;
	if (bz <= -WINDOW_SIZE_Z)	return true;
	if (WINDOW_SIZE_Z <= bz)	return true;

	for (int i = 0; i < MAX_HUMAN -1; i++) {
		if (human[i]) {
			if (Collision_CIrcles(human[i]->x, human[i]->z, HUMAN_SIZE, bx, bz, BULLET_SIZE)) {
				return true;
			}
		}
	}

	for (int i = 0; i < MAX_PILLAR; i++) {
		if (pillar[i]) {
			if (Collision_CIrcles(pillar[i]->x, pillar[i]->z, PILLAR_CIRCLE_RADIUS, bx, bz, BULLET_SIZE)) {
				return true;
			}
		}
	}
	return false;
}
