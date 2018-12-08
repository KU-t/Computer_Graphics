#include <gl/freeglut.h>
#include <stdio.h>
#include <windows.h>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_SIZE_Z 600

#define size 64

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
GLubyte *pBytes[6];
BITMAPINFO *info;
GLuint textures[6];

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
	glGenTextures(6, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	pBytes[0] = LoadDIBitmap("1.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	pBytes[1] = LoadDIBitmap("2.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	pBytes[2] = LoadDIBitmap("3.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	pBytes[3] = LoadDIBitmap("4.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[3]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	pBytes[4] = LoadDIBitmap("5.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[4]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	pBytes[5] = LoadDIBitmap("6.bmp", &info);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[5]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

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





	glColor4f(1.f, 1.f, 1.f, 1.f);

	glPushMatrix();
	glRotatef(0.f, 0.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180.f, 0.f, 1.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(270.f, 0.f, 1.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0.f, 0.f, size);
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, 0.f);
	glEnd();
	glPopMatrix();

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
	glTranslatef(0.f, -150.f, 0.f);
	glScalef(500.f, 1.f, 500.f);
	glColor3f(1.f, 1.f, 0.f);
	glutSolidCube(1.f);
	glPopMatrix();
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
		bitsize = (*info)->bmiHeader.biWidth * ( (*info)->bmiHeader.biBitCount / 8)  *  (*info)->bmiHeader.biHeight;
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

