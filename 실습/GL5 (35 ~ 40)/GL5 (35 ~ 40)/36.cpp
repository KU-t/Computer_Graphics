#include <gl/freeglut.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_SIZE_Z 600
#define MAX_LIGHT 2
#define LIGHT_X 0
#define LIGHT_Y 100
#define LIGHT_Z 150
#define deg(x) 3.141592*x/180
#define BALL_LENGTH 100
#define PYRAMID_WIDGHT 100
#define PYRAMID_HEIGHT PYRAMID_WIDGHT * 1.73205080757

enum VIEW { Perspective, Orthographic };

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
void Draw_Pyramid();
GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);

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

bool SWITCH_light_rad = false;
bool SWITCH_normal_vector = false;
float light_rad = 0.f;

//GL_LIGHT0
bool SWITCH_Light0 = false;
GLfloat AmbientLight0[] = { 1.f, 0.f, 0.f, 1.f }; // 빛의 세기 + 빛의 색
GLfloat DiffuseLight0[] = { 0.7f, 0.7f, 0.0f, 1.0f }; // 광원 색
GLfloat SpecularLight0[] = { 1.0, 0.0, 0.0, 1.0 }; // 하이라이트 색
GLfloat lightPos0[] = { LIGHT_X, LIGHT_Y, LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 

														  //GL_LIGHT1
bool SWITCH_Light1 = false;
GLfloat AmbientLight1[] = { 0.f, 0.f, 1.f, 1.f }; // 빛의 세기 + 빛의색
GLfloat DiffuseLight1[] = { 0.0f, 0.7f, 0.7f, 1.0f }; // 광원 색
GLfloat SpecularLight1[] = { 0.0, 0.0, 1.0, 1.0 }; // 하이라이트색
GLfloat lightPos1[] = { LIGHT_X, LIGHT_Y, -LIGHT_Z, 1.0 }; // 위치: (10, 5, 20) 

float ball_rad = 0.f;

GLubyte *pBytes;
BITMAPINFO *info;
GLuint textures[2];

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow(" - ");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	//glShadeModel(GL_SMOOTH);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	pBytes = LoadDIBitmap("2.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	pBytes = LoadDIBitmap("3.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);

	if (!View) {
		glLoadIdentity();
		gluLookAt(move_x, move_y, move_z + 600.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}

	glPushMatrix();

	if (View)
		glTranslatef(-move_x, -move_y, -move_z);
	glRotatef(Angle_x.radian, 1.f, 0.f, 0.f);
	glRotatef(Angle_y.radian, 0.f, 1.f, 0.f);
	glRotatef(Angle_z.radian, 0.f, 0.f, 1.f);


	glPushMatrix();
	glRotated(light_rad, 0.f, 1.f, 0.f);
	// LIGHT0
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		if (SWITCH_Light0)		glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);

	}

	// LIGHT1
	{
		glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		if (SWITCH_Light1)		glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);

	}
	glPopMatrix();


	//glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	GLfloat Object[] = { 1.f, 1.f, 1.1f, 1.f };
	GLfloat Object_specref[] = { 0.f, 0.5f, 0.5f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	float x = BALL_LENGTH * cos(deg(ball_rad));
	float z = BALL_LENGTH * sin(deg(ball_rad));
	glTranslatef(x, 50, z);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutSolidSphere(50, 100, 100);
	glPopMatrix();

	glPushMatrix();
	GLfloat Object0[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object_specref0[] = { 0.3f, 0.5f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref0);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	Draw_Pyramid();
	glPopMatrix();

	glPushMatrix();
	GLfloat Object1[] = { 0.5f, 0.1f, 0.1f, 1.f };
	GLfloat Object_specref1[] = { 0.5f, 0.3f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(200.f, 0, 200.f);
	Draw_Pyramid();
	glPopMatrix();

	glPushMatrix();
	GLfloat Object2[] = { 0.1f, 0.1f, 0.5f, 1.f };
	GLfloat Object_specref2[] = { 0.5f, 0.5f, 0.5f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref2);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(200.f, 0, -200.f);
	Draw_Pyramid();
	glPopMatrix();

	glPushMatrix();
	GLfloat Object3[] = { 0.5f, 0.5f, 0.f, 1.f };
	GLfloat Object_specref3[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref3);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(-200.f, 0, 200.f);
	Draw_Pyramid();
	glPopMatrix();

	glPushMatrix();
	GLfloat Object4[] = { 0.f, 0.5f, 0.5f, 1.f };
	GLfloat Object_specref4[] = { 0.f, 0.5f, 0.5f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref4);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glTranslatef(-200.f, 0, -200.f);
	Draw_Pyramid();
	glPopMatrix();




	//빛 제거
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glRotated(light_rad, 0.f, 1.f, 0.f);
	//Draw_LIGHT0 
	glPushMatrix();
	glTranslatef(LIGHT_X, LIGHT_Y, LIGHT_Z);
	glColor4f(1.f, 0.f, 0.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();

	//Draw_LIGHT1
	glPushMatrix();
	glTranslatef(LIGHT_X, LIGHT_Y, -LIGHT_Z);
	glRotatef(180.f, 1.f, 0.f, 0.f);
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glutSolidCone(20, 40, 10, 10);
	glPopMatrix();

	glPopMatrix();

	GLfloat bottom[] = { 0.3f, 0.3f, 0.f, 1.f };
	GLfloat bottom_specref[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bottom);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bottom_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	Draw_bottom();

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
	glutPostRedisplay();
	if (SWITCH_light_rad)	light_rad += 3.f;
	ball_rad += 5.f;
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		SWITCH_Light0 = (SWITCH_Light0 + 1) % 2;
		break;
	case '2':
		SWITCH_Light1 = (SWITCH_Light1 + 1) % 2;
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
		View = (VIEW)((View + 1) % 2);
		Reshape(800, 600);
		break;

	case '[':
		for (int i = 0; i < 3; i++) {
			DiffuseLight0[i] -= 0.05f;
			DiffuseLight1[i] -= 0.05f;
		}
		break;
	case ']':
		for (int i = 0; i < 3; i++) {
			DiffuseLight0[i] += 0.05f;
			DiffuseLight1[i] += 0.05f;
		}
		break;

	case 'n':	case 'N':
		for (int i = 0; i < 3; i++) {
			AmbientLight0[i] -= 0.05f;
			AmbientLight1[i] -= 0.05f;
		}
		break;
	case 'm':	case 'M':
		for (int i = 0; i < 3; i++) {
			AmbientLight0[i] += 0.05f;
			AmbientLight1[i] += 0.05f;
		}
		break;

	case ',':
		for (int i = 0; i < 3; i++) {
			SpecularLight0[i] -= 0.05f;
			SpecularLight1[i] -= 0.05f;
		}
		break;
	case '.':
		for (int i = 0; i < 3; i++) {
			SpecularLight0[i] += 0.05f;
			SpecularLight1[i] += 0.05f;
		}
		break;
	case 'c': case 'C':
		SWITCH_light_rad = (SWITCH_light_rad + 1) % 2;
		break;
	case 'v': case 'V':
		SWITCH_normal_vector = (SWITCH_normal_vector + 1) % 2;
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

void Draw_bottom() {
	glPushMatrix();
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-500, 500, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-500, -500, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(500, -500, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(500, 500, 0.f);
	glEnd();
	glPopMatrix();
}

void Draw_Pyramid() {
	for (float i = 0.f; i < 360.f; i += 90.f) {
		glPushMatrix();
		glRotatef(i, 0.f, 1.f, 0.f);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_TRIANGLES);
		if (SWITCH_normal_vector)	glNormal3f(0.0, 1.0, 1.73205080757);
		else glNormal3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(PYRAMID_WIDGHT / 2, 0.0, PYRAMID_WIDGHT / 2);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-PYRAMID_WIDGHT / 2, 0.0, PYRAMID_WIDGHT / 2);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(0.0, PYRAMID_HEIGHT, 0.0);
		glEnd();
		glPopMatrix();

	}
}

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info) {
	FILE *fp; GLubyte *bits; int bitsize, infosize; BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL) return NULL;
	// 비트맵 파일 헤더를 읽는다. 
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다. 
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = (*info)->bmiHeader.biWidth * ((*info)->bmiHeader.biBitCount / 8)  *  (*info)->bmiHeader.biHeight;
	// 비트맵의 크기만큼 메모리를 할당한다. 
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다. 
	auto n = fread(bits, 1, bitsize, fp);
	if (n < (unsigned int)bitsize) {
		free(*info);
		free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp); return bits;
}

