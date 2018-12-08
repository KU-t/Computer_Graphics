#include "main.h"
#include <sstream>
#include <string>

VIEW View = Perspective;

PUMPKIN *pumpkin[MAX_PUMP];
WATERMELLON *watermellon[MAX_WATERM];
TANGERINE *tangerine[MAX_TANGER];
APPLE *apple[MAX_APP];
TOMATO *tomato[MAX_TOMA];
GRAPE *grape[MAX_GRAPE];
BALL *ball[MAX_BALL];
PARTICLE *particle[MAX_PARTICLE];
OBSRECT *rect[MAX_RECT];
ITEM *item[MAX_ITEM];

float move_x = 0, move_y = 0, move_z = 240;
Angle Angle_x, Angle_y, Angle_z;

int Read_num = 0;
ifstream read_txt{ "Huddle.txt" };

int Have_balls = 1;
int Score = 0;
float Huddle_speed = HUDDLE_MOVE_SPEED_1;
int Create_Huddle_Speed = 15;

int rclickbutton = 0;

bool multiball = false;
int count_multiball = 0;

float title_state_z = -8000;
float over_state_z = -8000;
float back_ground_z = 0;

bool end_state_background_switch = false;
bool end_state_score_switch = false;

bool SWITCH_Light0 = true;
GLfloat AmbientLight0[] = { 0.4f, 0.4f, 0.4f, 1.f }; // 빛의 세기 + 빛의 색
GLfloat DiffuseLight0[] = { 1.f, 1.f, 1.f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 0.0, 0.0, 0.0, 1.0 }; // 하이라이트 색
GLfloat lightPos0[] = { LIGHT_X, LIGHT_Y, LIGHT_Z + 600, 1.0 }; // 위치: (10, 5, 20) 

GLfloat in_fog_color[4] = { 0.74f, 0.74f, 1.f, 1.0 };
GLfloat fog_density = 0.2f;
GLfloat fog_start = 500, f;
GLfloat fog_end = -CREATE_HUDDLE_Z + 1000.f;

STATE state = START_STATE;

void main(int argc, char **argv) {
	//init
	if (!read_txt) {
		cout << "txt 파일 읽기 실패" << endl;
	}
	for (int i = 0; i < MAX_PUMP; i++) pumpkin[i] = NULL;
	for (int i = 0; i < MAX_WATERM; i++) watermellon[i] = NULL;
	for (int i = 0; i < MAX_TANGER; i++) tangerine[i] = NULL;
	for (int i = 0; i < MAX_APP; i++) apple[i] = NULL;
	for (int i = 0; i < MAX_TOMA; i++) tomato[i] = NULL;
	for (int i = 0; i < MAX_GRAPE; i++) grape[i] = NULL;
	for (int i = 0; i < MAX_PARTICLE; i++) particle[i] = NULL;
	for (int i = 0; i < MAX_RECT; i++) rect[i] = NULL;
	for (int i = 0; i < MAX_ITEM; i++) item[i] = NULL;
	rect[START_STATE_INITE_NUMBER] = new OBSRECT(0, 0, -3350, 0, 0, 0);
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow("FRUITS CRUSH");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);

	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.6f, 1.0f, 1.0f, 1.0f);
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

	glOrtho(-WINDOW_SIZE_X, WINDOW_SIZE_X, -WINDOW_SIZE_Y, WINDOW_SIZE_Y, -WINDOW_SIZE_Z, WINDOW_SIZE_Z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//FOG
	{


		glEnable(GL_FOG);

		glFogf(GL_FOG_MODE, GL_LINEAR);

		glFogfv(GL_FOG_COLOR, in_fog_color);
		glFogf(GL_FOG_START, fog_start);
		glFogf(GL_FOG_END, fog_end);

		glFogf(GL_FOG_DENSITY, fog_density);
	}

	//그리기
	{
		// 2D 그리기
		{
			if (state == START_STATE) {

			}

			else if (state == PLAY_STATE) {
				stringstream sstr;
				string ballnum;
				sstr << Have_balls;
				ballnum = sstr.str();

				string ball;
				ball = "Ball: " + ballnum;
				drawText(ball.data(), ball.size(), -400, 700);

				stringstream sstr2;
				string allscore;
				sstr2 << Score;
				allscore = sstr2.str();

				string strScore;
				strScore = "Score: " + allscore;
				drawText(strScore.data(), strScore.size(), 300, 700);

			}

			else if (state == END_STATE) {
				if (end_state_score_switch) {
					stringstream str;
					string score;
					str << Score;
					score = str.str();

					string total_score;
					total_score = "SCORE: " + score;
					drawText(total_score.data(), total_score.size(), -70, -250);
				}
			}
		}
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
		if (SWITCH_Light0)      glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);

	}

	//그리기
	{
		if (state == START_STATE) {
			Click_Draw();
			Draw_Objects();
			Draw_bottom();
			Write_FRUIT_CRUSH(0, 0, title_state_z);
		}

		else if (state == PLAY_STATE) {
			Draw_Objects();
			Draw_bottom();
			Write_FRUIT_CRUSH(0, 0, title_state_z);
		}

		else if (state == END_STATE) {
			Draw_Objects();
			Draw_bottom();
			Write_FRUIT_CRUSH(0, WINDOW_SIZE_Y / 2, title_state_z);
			Write_GAME_OVER(0, -WINDOW_SIZE_Y + 150, over_state_z);
			if (end_state_background_switch)	Draw_End_state_Background(back_ground_z);
		}
	}

	glPopMatrix();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	Change_View(View);
}

void Mouse(int button, int m_state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && m_state == GLUT_DOWN) {
		int mx = 2 * (x - WINDOW_SIZE_X / 2);
		int my = -2 * (y - WINDOW_SIZE_Y / 2);

		if (state == START_STATE) {
			if (Have_balls > 0) {
				Add_Object(mx, my, 0, Ball, 0, 0, 0, 0, 0, 0, true);
				Have_balls -= 15;
			}
		}

		if (state == PLAY_STATE) {
			if (Have_balls > 0) {
				if (!multiball)	Add_Object(mx, my, 0, Ball, 0, 0, 0, 0, 0, 0, true);
				if (multiball) {
					Add_Object(mx - 150, my, 0, Ball, 0, 0, 0, 0, 0, 0, false);
					Add_Object(mx, my, 0, Ball, 0, 0, 0, 0, 0, 0, true);
					Add_Object(mx + 150, my, 0, Ball, 0, 0, 0, 0, 0, 0, false);
				}
				Have_balls--;
			}
		}

		else if (state == END_STATE) {

		}
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	float mx = 2 * (x - WINDOW_SIZE_X / 2);
	float my = 2 * (y - WINDOW_SIZE_Y / 2);

	glutPostRedisplay();
}

void Timer(int value) {
	Change_Angle_xyz();

	if (state == START_STATE) {
		rclickbutton += 10;
		Update_Objects(START_STATE);
		Collision_Object();
		if (!rect[START_STATE_INITE_NUMBER]) {
			state = PLAY_STATE;
			Score = 0;
		}
	}

	else if (state == PLAY_STATE) {
		if (title_state_z <= 1500.f) title_state_z += 100.f;

		Read_txt();
		if (multiball) {
			count_multiball = (count_multiball + 1) % TIME_MULTIBALL;
			if (count_multiball == 0)	multiball = false;
		}
		Update_Objects(PLAY_STATE);
		Collision_Object();
		Check_Update_Huddle_Speed();
		Check_Game_Over();
	}

	else if (state == END_STATE) {
		if (over_state_z <= 500.f) over_state_z += 100.f;
		if (-500 <= over_state_z && over_state_z <= 500.f) {
			end_state_background_switch = true;
			back_ground_z += 100.f;
			move_z += 50.f;
		}
		if (over_state_z > 500.f)	end_state_score_switch = true;
	}

	if (Have_balls < 0) Have_balls = 0;
	glutPostRedisplay();
	glutTimerFunc(70, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		SWITCH_Light0 = (SWITCH_Light0 + 1) % 2;
		break;
	case '2':
		state = START_STATE;
		break;
	case '3':
		state = PLAY_STATE;
		break;
	case '4':
		state = END_STATE;
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
		move_z = 240.f;
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

void Draw_bottom() {
	GLfloat Object1[] = { 0.5f, 0.5f, 1.f, 1.f };
	GLfloat Object_specref1[] = { 0.5f, 0.3f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(0.f, 0.f, -8000);
	glScalef(1500.f, 1500.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	for (float rad = 0.f; rad < 360.f; rad += 90.f) {
		glPushMatrix();
		glRotatef(rad, 0.f, 0.f, 1.f);
		glTranslatef(0.f, -WINDOW_SIZE_Y, -MAP_Z_SIZE / 2);
		glScalef(2 * WINDOW_SIZE_X, 1.f, MAP_Z_SIZE);
		glColor3f(1.f, rad / 100.f, 0.f);
		glutSolidCube(1.f);
		glPopMatrix();
	}
}

void Add_Object(float in_x, float in_y, float in_z, OBJECT object, int in_moving, float in_data_min, float in_data_max, float in_r, float in_g, float in_b, bool type) {
	int index = FindslotObject(object);
	if (index < 0)   return;

	switch (object) {
	case Apple:
		apple[index] = new APPLE(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Grape:
		grape[index] = new GRAPE(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Pumkin:
		pumpkin[index] = new PUMPKIN(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Tangerine:
		tangerine[index] = new TANGERINE(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Tomato:
		tomato[index] = new TOMATO(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Watermellon:
		watermellon[index] = new WATERMELLON(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Ball:
		if (type)	ball[index] = new BALL(in_x, in_y, false);
		else ball[index] = new BALL(in_x, in_y, true);
		break;
	case Particle:
		particle[index] = new PARTICLE(in_x, in_y, in_z, in_r, in_g, in_b);
		break;
	case Rect:
		rect[index] = new OBSRECT(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	case Item:
		item[index] = new ITEM(in_x, in_y, in_z, in_moving, in_data_min, in_data_max);
		break;
	}
}

int FindslotObject(OBJECT object) {
	switch (object) {
	case Apple:
		for (int i = 0; i < MAX_APP; i++)
			if (apple[i] == NULL)return i;
		break;
	case Grape:
		for (int i = 0; i < MAX_GRAPE; i++)
			if (grape[i] == NULL)return i;
		break;
	case Pumkin:
		for (int i = 0; i < MAX_PUMP; i++)
			if (pumpkin[i] == NULL)return i;
		break;
	case Tangerine:
		for (int i = 0; i < MAX_TANGER; i++)
			if (tangerine[i] == NULL)return i;
		break;
	case Tomato:
		for (int i = 0; i < MAX_TOMA; i++)
			if (tomato[i] == NULL)return i;
		break;
	case Watermellon:
		for (int i = 0; i < MAX_WATERM; i++)
			if (watermellon[i] == NULL)return i;
		break;
	case Ball:
		for (int i = 0; i < MAX_BALL; i++)
			if (ball[i] == NULL)return i;
		break;
	case Particle:
		for (int i = 0; i < MAX_PARTICLE; i++)
			if (particle[i] == NULL)return i;
		break;
	case Rect:
		for (int i = 0; i < MAX_RECT; i++)
			if (rect[i] == NULL)return i;
		break;
	case Item:
		for (int i = 0; i < MAX_ITEM; i++)
			if (item[i] == NULL)return i;
		break;
	}
	return -1;
}

void Draw_Objects() {
	for (int i = 0; i < MAX_PUMP; i++) {
		if (pumpkin[i]) pumpkin[i]->Draw();
	}

	for (int i = 0; i < MAX_WATERM; i++) {
		if (watermellon[i]) watermellon[i]->Draw();
	}

	for (int i = 0; i < MAX_TANGER; i++) {
		if (tangerine[i]) tangerine[i]->Draw();
	}

	for (int i = 0; i < MAX_APP; i++) {
		if (apple[i]) apple[i]->Draw();
	}

	for (int i = 0; i < MAX_TOMA; i++) {
		if (tomato[i]) tomato[i]->Draw();
	}

	for (int i = 0; i < MAX_GRAPE; i++) {
		if (grape[i]) grape[i]->Draw();
	}

	for (int i = 0; i < MAX_BALL; i++) {
		if (ball[i]) ball[i]->Draw();
	}

	for (int i = 0; i < MAX_PARTICLE; i++) {
		if (particle[i]) particle[i]->Draw();
	}

	for (int i = 0; i < MAX_RECT; i++) {
		if (rect[i]) rect[i]->Draw();
	}

	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i]) item[i]->Draw();
	}
}

void Update_Objects(STATE curr_state) {

	for (int i = 0; i < MAX_PUMP; i++) {
		if (pumpkin[i]) {
			pumpkin[i]->Update(Huddle_speed);
			if (pumpkin[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete pumpkin[i];
				pumpkin[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_WATERM; i++) {
		if (watermellon[i]) {
			watermellon[i]->Update(Huddle_speed);
			if (watermellon[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete watermellon[i];
				watermellon[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_TANGER; i++) {
		if (tangerine[i]) {
			tangerine[i]->Update(Huddle_speed);
			if (tangerine[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete tangerine[i];
				tangerine[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_APP; i++) {
		if (apple[i]) {
			apple[i]->Update(Huddle_speed);
			if (apple[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete apple[i];
				apple[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_TOMA; i++) {
		if (tomato[i]) {
			tomato[i]->Update(Huddle_speed);
			if (tomato[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete tomato[i];
				tomato[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_GRAPE; i++) {
		if (grape[i]) {
			grape[i]->Update(Huddle_speed);
			if (grape[i]->z >= DELETE_HUDDLE_Z) {
				Have_balls -= 10;
				delete grape[i];
				grape[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_BALL; i++) {
		if (ball[i]) {
			ball[i]->Update();
			if (ball[i]->z <= DELETE_BALL_Z) {
				delete ball[i];
				ball[i] = NULL;
			}
		}
	}

	for (int i = 0; i < MAX_PARTICLE; i++) {
		if (particle[i]) {
			particle[i]->Update();
			if (particle[i]->z >= DELETE_PARTICLE_Z) {
				delete particle[i];
				particle[i] = NULL;
			}
		}
	}

	if (curr_state != START_STATE) {
		for (int i = 0; i < MAX_RECT; i++) {
			if (rect[i]) {
				rect[i]->Update(Huddle_speed);
				if (rect[i]->z >= DELETE_RECT_Z) {
					Have_balls -= 30;
					delete rect[i];
					rect[i] = NULL;
				}
			}
		}
	}

	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i]) {
			item[i]->Update(Huddle_speed);
			if (item[i]->z >= DELETE_PARTICLE_Z) {
				delete item[i];
				item[i] = NULL;
			}
		}
	}
}

void Read_txt() {
	Read_num = (Read_num + 1) % Create_Huddle_Speed;
	if (Read_num == 0) {
		int create_type;
		float create_x, create_y;
		float data_min, data_max;
		int create_moving;

		if (read_txt >> create_type >> create_x >> create_y >> create_moving >> data_min >> data_max) {
			if (create_type != NONE)
				Add_Object(create_x, create_y, CREATE_HUDDLE_Z, (OBJECT)(create_type), create_moving, data_min, data_max, true);
		}
	}
}

bool Collision_check(float bx, float by, float bz, float br, float hx, float hy, float hz, float hr) {
	float x = bx - hx;
	float y = by - hy;
	float z = bz - hz;
	float r = br + hr;

	if (x*x + y * y + z * z <= r * r)   return true;
	return false;
}

bool Collision_Rect_check(float bx, float by, float bz, float hx, float hy, float hz) {
	if (hy - RECT_SIZE <= by + BALL_SIZE && by - BALL_SIZE <= hx + RECT_SIZE) {
		if (hz - RECT_SIZE <= bz + BALL_SIZE && bz - BALL_SIZE <= hz + RECT_SIZE)	return true;
	}
	return false;
}

void Collision_Object() {

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_PUMP; i++) {
				if (pumpkin[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, pumpkin[i]->x, pumpkin[i]->y, pumpkin[i]->z, PUMP_COLL)) {
						Score += pumpkin[i]->socre;
						Add_Object(pumpkin[i]->x, pumpkin[i]->y, pumpkin[i]->z, Particle, 0, 0, 0, 1.f, 1.f, 0.1f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete pumpkin[i];
						pumpkin[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_WATERM; i++) {
				if (watermellon[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, watermellon[i]->x, watermellon[i]->y, watermellon[i]->z, WATERM_COLL)) {
						Score += watermellon[i]->socre;
						Add_Object(watermellon[i]->x, watermellon[i]->y, watermellon[i]->z, Particle, 0, 0, 0, 0.01f, 0.3f, 0.01f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete watermellon[i];
						watermellon[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_TANGER; i++) {
				if (tangerine[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, tangerine[i]->x, tangerine[i]->y, tangerine[i]->z, TANGER_COLL)) {
						Score += tangerine[i]->socre;
						Add_Object(tangerine[i]->x, tangerine[i]->y, tangerine[i]->z, Particle, 0, 0, 0, 1.f, 0.5f, 0.01f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete tangerine[i];
						tangerine[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_APP; i++) {
				if (apple[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, apple[i]->x, apple[i]->y, apple[i]->z, APP_COLL)) {
						Score += apple[i]->socre;
						Add_Object(apple[i]->x, apple[i]->y, apple[i]->z, Particle, 0, 0, 0.7f, 0.7f, 0.01f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete apple[i];
						apple[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_TOMA; i++) {
				if (tomato[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, tomato[i]->x, tomato[i]->y, tomato[i]->z, TOMA_COLL)) {
						Score += tomato[i]->socre;
						Add_Object(tomato[i]->x, tomato[i]->y, tomato[i]->z, Particle, 0, 0, 0, 1.f, 0.01f, 0.01f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete tomato[i];
						tomato[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_GRAPE; i++) {
				if (grape[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, grape[i]->x, grape[i]->y, grape[i]->z, GRAPE_COLL)) {
						Score += grape[i]->socre;
						Add_Object(grape[i]->x, grape[i]->y, grape[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Have_balls += 5;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						delete grape[i];
						grape[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_RECT; i++) {
				if (rect[i]) {
					if (Collision_Rect_check(ball[j]->x, ball[j]->y, ball[j]->z, rect[i]->x, rect[i]->y, rect[i]->z)) {
						Score += rect[i]->socre;
						PlaySound(TEXT("bb.wav"), NULL, SND_ASYNC);
						Add_Object(rect[i]->x, rect[i]->y, rect[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Add_Object(rect[i]->x - WINDOW_SIZE_X / 3, rect[i]->y, rect[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Add_Object(rect[i]->x + WINDOW_SIZE_X / 3, rect[i]->y, rect[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Add_Object(rect[i]->x - WINDOW_SIZE_X * 2 / 3, rect[i]->y, rect[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Add_Object(rect[i]->x + WINDOW_SIZE_X * 2 / 3, rect[i]->y, rect[i]->z, Particle, 0, 0, 0.3f, 0.01f, 0.4f, false);
						Have_balls += 15;
						delete rect[i];
						rect[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}

	for (int j = 0; j < MAX_BALL; j++) {
		if (ball[j]) {
			for (int i = 0; i < MAX_ITEM; i++) {
				if (item[i]) {
					if (Collision_check(ball[j]->x, ball[j]->y, ball[j]->z, BALL_SIZE, item[i]->x, item[i]->y, item[i]->z, GRAPE_COLL)) {
						multiball = true;
						count_multiball += ADD_TIME_MULTIBALL;
						delete item[i];
						item[i] = NULL;
						delete   ball[j];
						ball[j] = NULL;
						break;
					}
				}
			}
		}
	}
}

void drawText(const char *text, int length, int x, int y)
{
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
}

void Check_Update_Huddle_Speed() {
	if (HUDDLE_SCORE_1 < Score && Huddle_speed < HUDDLE_MOVE_SPEED_1 + 1) {
		Huddle_speed = HUDDLE_MOVE_SPEED_2;
	}
	if (HUDDLE_SCORE_2 < Score && Huddle_speed < HUDDLE_MOVE_SPEED_2 + 1) {
		Huddle_speed = HUDDLE_MOVE_SPEED_3;
	}
	if (HUDDLE_SCORE_3 < Score && Huddle_speed < HUDDLE_MOVE_SPEED_3 + 1) {
		Huddle_speed = HUDDLE_MOVE_SPEED_4;
	}
	if (HUDDLE_SCORE_4 < Score && Huddle_speed < HUDDLE_MOVE_SPEED_4 + 1) {
		Huddle_speed = HUDDLE_MOVE_SPEED_5;
	}
}

void Click_Draw() {
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };

	glPushMatrix();
	GLfloat Object3[] = { 1.f, 1.f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(0, WINDOW_SIZE_Y - 150, 0);
	glRotatef(20, 1, 0, 0);
	glRotatef(rclickbutton, 0, 1, 0);
	glutSolidTorus(10, 40, 30, 30);
	glPopMatrix();

	string click;
	click = "Click!!";
	drawText(click.data(), click.size(), -50, 550);
	glPopMatrix();
}

void Check_Game_Over() {
	if (Have_balls <= 0) {
		for (int i = 0; i < MAX_BALL; i++) {
			if ((ball[i])) return;
		}
		state = END_STATE;
		PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
	}
}

void Write_FRUIT_CRUSH(float x, float y, float z)
{
	// Fruit
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };

	GLfloat Object0[] = { 0.7f, 0.2f, 0.2f, 1.f };
	GLfloat Object1[] = { 1.f, 1.f, 0.2f, 1.f };
	GLfloat Object2[] = { 0.2f, 1.f, 1.f, 1.f };
	GLfloat Object3[] = { 1.f, 0.2f, 1.f, 1.f };
	GLfloat Object4[] = { 02.f, 1.f, 0.2f, 1.f };
	GLfloat Object5[] = { 0.2f, 0.2f, 0.7f, 1.f };
	GLfloat Object6[] = { 0.5f, 0.5f, 0.f, 1.f };
	GLfloat Object7[] = { 0.2f, 0.5f, 0.5f, 1.f };
	GLfloat Object8[] = { 0.5f, 02.f, 0.5f, 1.f };
	GLfloat Object9[] = { 0.2f, 0.5f, 0.2f, 1.f };

	glPushMatrix();
	glTranslatef(x - 300, y, z);

	// F
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(-5, 0, 0);
	glScalef(4, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, -55, 0);
	glScalef(1, 6, 1);
	glutSolidCube(20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-5, -45, 0);
	glScalef(3, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	// R
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(60, -75, 0);
	glScalef(1, 4.2, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80, -55, 0);
	glScalef(2.5, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	// U
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(140, -75, 0);
	glScalef(1, 3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(160, -110, 0);
	glScalef(2, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(180, -75, 0);
	glScalef(1, 3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// i
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(230, -45, 0);
	glScalef(1, 1, 1);
	glutSolidCube(20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(230, -90, 0);
	glScalef(1, 3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// t
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(290, -65, 0);
	glScalef(3, 1, 1);
	glutSolidCube(20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(290, -80, 0);
	glScalef(1, 4.5, 1);
	glutSolidCube(20);
	glPopMatrix();

	// 이게마지막임
	glPopMatrix();

	// Crush
	glPushMatrix();
	glTranslatef(x, y - 200, z);

	//C
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, -50, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -100, 0);
	glScalef(5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// R
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object6);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(90, -75, 0);
	glScalef(1.3, 4.2, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(110, -55, 0);
	glScalef(2.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// U
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object7);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(170, -75, 0);
	glScalef(1.3, 3.2, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(190, -110, 0);
	glScalef(2, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(210, -75, 0);
	glScalef(1.3, 3.2, 1);
	glutSolidCube(20);
	glPopMatrix();

	// S
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object8);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(280, -55, 0);
	glScalef(2.5, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(255, -65, 0);
	glScalef(1, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(280, -85, 0);
	glScalef(2.5, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(305, -105, 0);
	glScalef(1, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(280, -115, 0);
	glScalef(2.5, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	// H
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object9);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(350, -80, 0);
	glScalef(1, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(370, -90, 0);
	glScalef(2.5, 1, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(390, -110, 0);
	glScalef(1, 2, 1);
	glutSolidCube(20);
	glPopMatrix();

	// 지우지마
	glPopMatrix();
}

void Write_GAME_OVER(float x, float y, float z)
{
	glPushMatrix();

	GLfloat Object_specref1[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object1[] = { 0.7f, 0.2f, 0.2f, 1.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glTranslatef(x - 200, y + 200, z);

	// G
	glPushMatrix();
	glScalef(4, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-40, -50, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -100, 0);
	glScalef(4, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37, -80, 0);
	glScalef(1.3, 3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, -60, 0);
	glScalef(2, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// A
	glPushMatrix();
	glTranslatef(120, 0, 0);
	glScalef(4, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120, -40, 0);
	glScalef(3.4, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(85, -60, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(155, -60, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	// M
	glPushMatrix();
	glTranslatef(200, -60, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(240, -60, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(280, -60, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(240, 0, 0);
	glScalef(5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// E
	glPushMatrix();
	glTranslatef(370, 0, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(330, -55, 0);
	glScalef(1.3, 4.5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(370, -100, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(370, -50, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();


	// 지우지마라
	glPopMatrix();

	// over
	glPushMatrix();

	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object0[] = { 0.2f, 0.2f, 0.7f, 1.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glTranslatef(x - 100, y, z);

	// O
	glPushMatrix();
	glTranslatef(-10, 0, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, -50, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, -100, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, -50, 0);
	glScalef(1.3, 5, 1);
	glutSolidCube(20);
	glPopMatrix();

	// V
	glPushMatrix();
	glTranslatef(85, -40, 0);
	glScalef(1.3, 5.5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(155, -40, 0);
	glScalef(1.3, 5.5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120, -100, 0);
	glScalef(4, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// E
	glPushMatrix();
	glTranslatef(250, 0, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(210, -55, 0);
	glScalef(1.3, 4.5, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250, -100, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250, -50, 0);
	glScalef(4.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	// R
	glPushMatrix();
	glTranslatef(360, 0, 0);
	glScalef(3.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(330, -55, 0);
	glScalef(1.3, 6, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(360, -50, 0);
	glScalef(3.5, 1.3, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(390, -30, 0);
	glScalef(1.3, 2, 1);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(390, -85, 0);
	glRotatef(20, 0, 0, 1);
	glScalef(1.3, 3, 1);
	glutSolidCube(20);
	glPopMatrix();


	// 지우지마라
	glPopMatrix();
}

void Draw_End_state_Background(float move_z) {
	GLfloat Object[] = { 0.5f, 0.5f, 1.f, 1.f };
	GLfloat Object_specref[] = { 0.5f, 0.3f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(0.f, 0.f, -8000.f + move_z);
	glScalef(15000.f, 15000.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
}