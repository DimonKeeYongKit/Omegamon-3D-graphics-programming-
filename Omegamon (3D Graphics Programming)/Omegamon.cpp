#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Omegamon Giant Robot"

BITMAP BMP;           // BITMAP STRUCTURE
HBITMAP hBMP = NULL;  //BITMAP HANDLE
int textureChoice = 1;
float RmouthU = 0.0, LmouthU = 0.0;
float RmouthC = 0.0, LmouthC = 0.0;
bool RisOpen = false, RWeaponOn = false, LisOpen = false, LWeaponOn = false;
float RweaponScale = 0.0f, RweaponTranslate = 0.0f, LweaponScale = 0.3f, LweaponTranslate = 0.0f;
float LlowerlegRotate = 0.0f, RlowerlegRotate = 0.0f, LupperlegRotate = 0.0f, RupperlegRotate = 0.0f;
bool walkFront = false, LfirstMovement = false, RfirstMovement = false, leftlegMovementDone = false, rightlegMovementDone = false, allMovement = true, halfMovement = false;
float currentallTranslate = 0.0f, targetallTranslate = 0.0f, targetLlegTranslate = 0.0f, targetRlegTranslate = 0.0f;
bool RHRotateFront = false, LHRotateFront = false, RHRotateUP = false, LHRotateUP = false;
float currentRHRotateUD = 0.0f, targetRHRotateUD = 0.0f, currentLHRotateUD = 0.0f, targetLHRotateUD = 0.0f, currentRHRotateFB = 0.0f, targetRHRotateFB = 0.0f, currentLHRotateFB = 0.0f, targetLHRotateFB = 0.0f;
bool RHLMoveUP = false, LHLMoveUP = false;
float currentRHLRotateUP = 0.0f, targetRHLRotateUP = 0.0f, currentLHLRotateUP = 0.0f, targetLHLRotateUP = 0.0f;
float currentHeadRotate = 0.0f, targetHeadRotate = 0.0f;
bool isOrtho = true; //projection
float tz = 0.0f, tSpeed = 1.0f, ry = 0.0f, rSpeed = 1.0f, tx = 0.0f;
bool animation1 = false, a1 = false, a2 = false;
float currentCannonT = 0.0f, targetCannonT = 0.0f, currentCannonS = 0.0f, targetCannonS = 0.0f;
//lighting
bool isLightOn = false;
float moveablePos[] = { 0.0, 5.0, 0.0 };
float amb[] = { 1.0, 0.0, 0.0 };  // ambient light
float diff[] = { 1.0, 0.0, 0.0 }; // diffuse light
//material

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == 0x30 && textureChoice == 1) textureChoice = 2;
		else if (wParam == 0x30 && textureChoice == 2) textureChoice = 3;
		else if (wParam == 0x30 && textureChoice == 3) textureChoice = 4;
		else if (wParam == 0x30 && textureChoice == 4) textureChoice = 1;
		else if (wParam == 0x31) RisOpen = !RisOpen, RWeaponOn = !RWeaponOn, LisOpen = !LisOpen, LWeaponOn = !LWeaponOn;
		else if (wParam == 0x32) walkFront = true, targetallTranslate -= 0.2f, LfirstMovement = false, RfirstMovement = false, leftlegMovementDone = false, rightlegMovementDone = false;
		else if (wParam == 0x33) RisOpen = true, RWeaponOn = true, LisOpen = true, LWeaponOn = true, targetRHLRotateUP = 0.0f, targetLHLRotateUP = 0.0f, targetRHRotateUD = -70.0f, targetLHRotateUD = 0.0f, targetRHRotateFB = 90.0f, targetLHRotateFB = 0.0f, animation1 = true, currentCannonT = 0.0f, currentCannonS = 0.0f;
		else if (wParam == 0x34) RisOpen = true, RWeaponOn = true, LisOpen = true, LWeaponOn = true, targetRHRotateUD = 0.0f, targetLHRotateUD = -20.0f, targetRHRotateFB = 0.0f, currentLHRotateFB = 0.0f, targetLHRotateFB = -90.0f, currentRHLRotateUP = 0.0f, targetRHLRotateUP = 0.0f, currentLHLRotateUP = 0.0f, targetLHLRotateUP = -80.0f;
		else if (wParam == 0x4B && targetHeadRotate > -60.0f) targetHeadRotate -= 30.0f;
		else if (wParam == 0x4C && targetHeadRotate < 60.0f) targetHeadRotate += 30.0f;
		else if (wParam == 0x59 && targetLHRotateUD > -70.0f) LHRotateUP = true, targetLHRotateUD -= 10.0f;		 //Press 'Y' move up left hand
		else if (wParam == 0x55 && targetLHRotateUD < 0.0f) LHRotateUP = false, targetLHRotateUD += 10.0f;       //Press 'U' move down left hand
		else if (wParam == 0x48 && targetLHRotateFB > -90.0f)LHRotateFront = true, targetLHRotateFB -= 10.0f;     //Press 'H' move front left hand
		else if (wParam == 0x4A && targetLHRotateFB < 10.0f)LHRotateFront = false, targetLHRotateFB += 10.0f;   //Press 'J' move back left hand
		else if (wParam == 0x4E && targetLHLRotateUP > -80.0f) LHLMoveUP = true, targetLHLRotateUP -= 40.0f;      //Press 'N' to move up right lower arm 
		else if (wParam == 0x4D && targetLHLRotateUP < 0.0f) LHLMoveUP = false, targetLHLRotateUP += 40.0f;      //Press 'M' to move down right lower arm
		else if (wParam == 0x52 && targetRHRotateUD > -70.0f) RHRotateUP = true, targetRHRotateUD -= 10.0f;		 //Press 'R' move up right hand
		else if (wParam == 0x54 && targetRHRotateUD < 0.0f) RHRotateUP = false, targetRHRotateUD += 10.0f;       //Press 'T' move down right hand
		else if (wParam == 0x46 && targetRHRotateFB < 90.0f)RHRotateFront = false, targetRHRotateFB += 10.0f;   //Press 'F' move front right hand
		else if (wParam == 0x47 && targetRHRotateFB > -10.0f)RHRotateFront = true, targetRHRotateFB -= 10.0f;     //Press 'G' move back right hand
		else if (wParam == 0x56 && targetRHLRotateUP < 80.0f) RHLMoveUP = true, targetRHLRotateUP += 40.0f;      //Press 'V' to move up right lower arm 
		else if (wParam == 0x42 && targetRHLRotateUP > 0.0f) RHLMoveUP = false, targetRHLRotateUP -= 40.0f;      //Press 'B' to move down right lower arm
		else if (wParam == 0x4F) isOrtho = true, tz = 0.0f;
		else if (wParam == 0x50) isOrtho = false, tz = 15.0f;//15
		else if (wParam == VK_SPACE) isLightOn = !isLightOn;
		else if (wParam == 0x41) moveablePos[0] -= 1.0f;       //A
		else if (wParam == 0x44) moveablePos[0] += 1.0f;       //D
		else if (wParam == 0x57) moveablePos[1] += 1.0f;       //W
		else if (wParam == 0x53) moveablePos[1] -= 1.0f;      //S
		else if (wParam == 0x51) moveablePos[2] += 1.0f;       //Q
		else if (wParam == 0x45) moveablePos[2] -= 1.0f;       //E
		else if (wParam == VK_UP) {
			if (isOrtho) {
				if (tz < 1.0f)
					tz += tSpeed;
			}
			else {
				if (tz > 5.0f)
					tz -= tSpeed;
			}
		}
		else if (wParam == VK_DOWN) {
			if (isOrtho) {
				if (tz > -10.0f)
					tz -= tSpeed;
			}
			else {
				if (tz < 25.0f)
					tz += tSpeed;
			}
		}
		else if (wParam == VK_LEFT) ry += rSpeed;
		else if (wParam == VK_RIGHT) ry -= rSpeed;
		else if (wParam == 0x5A) tx += tSpeed;
		else if (wParam == 0x43) tx -= tSpeed;
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------
bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
//void cylinder(float basR, float topR, float height, int slide, int stack) {
//	GLUquadricObj *cylinder = NULL;
//	cylinder = gluNewQuadric();
//	gluQuadricDrawStyle(cylinder, GLU_FILL);
//	gluCylinder(cylinder, basR, topR, height, slide, stack);
//	gluDeleteQuadric(cylinder);
//}
//void sphere(float R, int slide, int stack) {
//	GLUquadricObj *sphere = NULL;
//	sphere = gluNewQuadric();
//	gluQuadricDrawStyle(sphere, GLU_FILL);
//	gluSphere(sphere, R, slide, stack);
//	gluDeleteQuadric(sphere);
//}
//void rectangle(float height, float length, float width) {
//
//	// Face 1 - front
//	glBegin(GL_QUADS);
//	glVertex3f(0.0f, 0.0f, width);
//	glVertex3f(length, 0.0f, width);
//	glVertex3f(length, height, width);
//	glVertex3f(0.0f, height, width);
//	glEnd();
//
//	//Face 2 - back
//	glBegin(GL_QUADS);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(length, 0.0f, 0.0f);
//	glVertex3f(length, height, 0.0f);
//	glVertex3f(0.0f, height, 0.0f);
//	glEnd();
//
//	//Face 3 - left
//	glBegin(GL_QUADS);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, width);
//	glVertex3f(0.0f, height, width);
//	glVertex3f(0.0f, height, 0.0f);
//	glEnd();
//
//	//Face 4 - right
//	glBegin(GL_QUADS);
//	glVertex3f(length, 0.0f, width);
//	glVertex3f(length, 0.0f, 0.0f);
//	glVertex3f(length, height, 0.0f);
//	glVertex3f(length, height, width);
//	glEnd();
//
//	//Face 5 - up
//	glBegin(GL_QUADS);
//	glVertex3f(0.0f, height, width);
//	glVertex3f(length, height, width);
//	glVertex3f(length, height, 0.0f);
//	glVertex3f(0.0f, height, 0.0f);
//	glEnd();
//	//Face 6 - down
//	glBegin(GL_QUADS);
//	glVertex3f(0.0f, 0.0f, width);
//	glVertex3f(length, 0.0f, width);
//	glVertex3f(length, 0.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glEnd();
//
//}
//void drawSphereWithoutGLU()
//{
//	const float PI = 3.141592f;
//	GLfloat x, y, z, sliceA, stackA;
//	GLfloat radius = 0.5;
//	int sliceNo = 30, stackNo = 30;
//	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
//	{
//		glBegin(GL_LINE_STRIP);
//		for (stackA = 0.0; stackA < 2 * PI; stackA += PI / stackNo)
//		{
//			x = radius * cos(stackA) * sin(sliceA);
//			y = radius * sin(stackA) * sin(sliceA);
//			z = radius * cos(sliceA);
//			glVertex3f(x, y, z);
//			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
//			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
//			z = radius * cos(sliceA + PI / sliceNo);
//			glVertex3f(x, y, z);
//		}
//		glEnd();
//	}
//}
//void partOfCircle() {
//	float radius = 0.5;
//	float PI = 3.142;
//	float twoPI = 2 * PI;
//	float PIper4 = PI / 4;
//	float halfPI = PI / 2;
//	float ThreePerFourPI = 3 * PI / 4;
//	float quaterPI = 3 * (2 * PI) / 4;
//
//
//	glBegin(GL_POINTS);
//	//half circle    0-180
//	/*for (float i = 0.0; i <= PI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//half circle 90-270
//	/*for (float i = halfPI; i <= quaterPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//half circle 180-360
//	/*for (float i = PI; i <= twoPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//
//
//
//	//a quarter circle 0-90
//	/*for (float i = 0.0; i <= halfPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//a quarter circle 90-180
//	/*for (float i = halfPI; i <= PI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	////a quarter circle 180-270
//	//for (float i = PI; i <= quaterPI; i += 0.001) {
//	//	glVertex2f((sin(i)*radius), (cos(i)*radius));
//	//}
//
//	//a quarter circle 270-360
//	/*for (float i = quaterPI; i <= twoPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//
//
//	//0-45
//	/*for (float i = 0.0; i <= PIper4; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//45-90
//	/*for (float i = PIper4; i <= halfPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//90-135
//	/*for (float i = halfPI; i <= ThreePerFourPI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	//135-180
//	/*for (float i = ThreePerFourPI; i <= PI; i += 0.001) {
//		glVertex2f((sin(i)*radius), (cos(i)*radius));
//	}*/
//
//	glEnd();
//}
void lighting() {
	if (isLightOn)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	//glEnable(GL_LIGHTING);
	//Light 0 - red color ambient light
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_POSITION, moveablePos);
	glEnable(GL_LIGHT0);



}
GLuint loadTexture(LPCSTR filename) {
	//get from step 1
	GLuint texture = 0;//texture name
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);


	//step4
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//get from step 5
	DeleteObject(hBMP);

	return texture;
}
void lowerbody(float size)
{
	//total 47 polygon
	float lowerbodyM[] = { 1.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, lowerbodyM);
	glPushMatrix();
	glScalef(size, size, size);
	glColor3f(0.95f, 0.95f, 0.95f);
	glBegin(GL_QUADS);   //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.7f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //2
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, -0.7f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);  //3
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.7f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //4
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, -0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, -0.7f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);  //5
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.3f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.3f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //6
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.3f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);  //7
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.3f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.3f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //8
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.3f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, -0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -0.5f);
	glEnd();

	glBegin(GL_QUADS);  //9
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //10
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.8f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 0.6f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.8f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //11
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.3f, 0.9f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //12
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //13
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.8f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.6f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.8f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //14
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //15
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //16
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.8f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 0.6f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.8f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //17
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 0.8f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.3f, 0.9f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //18
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //19
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.8f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.6f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.8f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //20
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glEnd();

	glBegin(GL_POLYGON); //21
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, 0.375f);
	glEnd();

	glBegin(GL_POLYGON); //22
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 1.1f, 0.375f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 1.0f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glEnd();

	glBegin(GL_POLYGON);  //23
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, -0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, -0.375f);
	glEnd();

	glBegin(GL_POLYGON);  //24
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glVertex3f(-0.2f, 1.1f, -0.375f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glEnd();

	glBegin(GL_TRIANGLES);  //25 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, -0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.25f);
	glEnd();

	glBegin(GL_TRIANGLES);  //26 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.1f, 0.0f, -0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.25f);
	glEnd();

	glBegin(GL_QUADS);  //27 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.1f, 0.0f, -0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.1f, 0.3f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //28 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, -0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.3f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //29 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //30 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //31 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.4f, 0.8f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //32 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.6f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //33 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4f, 0.8f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 0.8f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //34 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.4f, 0.8f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 0.8f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //35 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4f, 1.0f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.9f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 1.0f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //36 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.4f, 1.0f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.4f, 1.0f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //37 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.4f, 1.0f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.4f, 1.0f, -0.25f);
	glEnd();

	glBegin(GL_QUADS);  //38 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.1f, 0.375f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, 1.1f, -0.375f);
	glEnd();

	glBegin(GL_QUADS);  //39 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, 0.375f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 1.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 1.1f, -0.25f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, -0.375f);
	glEnd();

	glBegin(GL_QUADS);  //40 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.1f, 0.375f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, 1.1f, -0.375f);
	glEnd();

	glBegin(GL_QUADS);  //41 side
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, 0.375f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 1.1f, -0.375f);
	glEnd();

	glColor3f(0.9725f, 0.7647f, 0.3412f);
	glPushMatrix();   //right cylinder 1
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.9f, 0.2f);
	GLUquadricObj *c = NULL;
	c = gluNewQuadric();
	gluQuadricDrawStyle(c, GLU_FILL);
	gluCylinder(c, 0.1, 0.1, 0.38, 30, 30);
	gluDeleteQuadric(c);
	glPopMatrix();

	glPushMatrix();   //right cylinder 2
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.87f, 0.5f);
	glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *c2 = NULL;
	c2 = gluNewQuadric();
	gluQuadricDrawStyle(c2, GLU_FILL);
	gluCylinder(c2, 0.1, 0.1, 0.31, 30, 30);
	gluDeleteQuadric(c2);
	glPopMatrix();

	glPushMatrix();   //right cylinder 3
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 1.52f, 0.69f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *c3 = NULL;
	c3 = gluNewQuadric();
	gluQuadricDrawStyle(c3, GLU_FILL);
	gluCylinder(c3, 0.0, 0.1, 0.5, 30, 30);
	gluDeleteQuadric(c3);
	glPopMatrix();

	glPushMatrix();   //left cylinder 1
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.9f, -0.6f);
	GLUquadricObj *cc = NULL;
	cc = gluNewQuadric();
	gluQuadricDrawStyle(cc, GLU_FILL);
	gluCylinder(cc, 0.1, 0.1, 0.38, 30, 30);
	gluDeleteQuadric(cc);
	glPopMatrix();

	glPushMatrix();   //left cylinder 2
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.87f, -0.5f);
	glRotatef(-135.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *cc2 = NULL;
	cc2 = gluNewQuadric();
	gluQuadricDrawStyle(cc2, GLU_FILL);
	gluCylinder(cc2, 0.1, 0.1, 0.31, 30, 30);
	gluDeleteQuadric(cc2);
	glPopMatrix();

	glPushMatrix();   //left cylinder 3
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 1.52f, -0.69f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *cc3 = NULL;
	cc3 = gluNewQuadric();
	gluQuadricDrawStyle(cc3, GLU_FILL);
	gluCylinder(cc3, 0.0, 0.1, 0.5, 30, 30);
	gluDeleteQuadric(cc3);
	glPopMatrix();

	glPopMatrix();

}
void righthandArmor(float size) {
	//total 25 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glColor3f(1.0f, 0.8078f, 0.3490f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	GLUquadricObj *armor1 = NULL;                   //1
	armor1 = gluNewQuadric();
	gluQuadricDrawStyle(armor1, GLU_FILL);
	gluCylinder(armor1, 0.1, 0.0, 0.4, 40, 40);
	gluDeleteQuadric(armor1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.3f);
	GLUquadricObj *armor2 = NULL;                   //2
	armor2 = gluNewQuadric();
	gluQuadricDrawStyle(armor2, GLU_FILL);
	gluCylinder(armor2, 0.1, 0.0, 0.4, 40, 40);
	gluDeleteQuadric(armor2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-135.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.3f);
	GLUquadricObj *armor3 = NULL;                   //3
	armor3 = gluNewQuadric();
	gluQuadricDrawStyle(armor3, GLU_FILL);
	gluCylinder(armor3, 0.1, 0.0, 0.4, 40, 40);
	gluDeleteQuadric(armor3);
	glPopMatrix();
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //4
	glVertex3f(0.2f, 0.3f, 0.1f);
	glVertex3f(-0.2f, 0.3f, 0.1f);
	glVertex3f(-0.2f, 0.3f, -0.1f);
	glVertex3f(0.2f, 0.3f, -0.1f);

	glVertex3f(0.2f, 0.3f, 0.1f);                   //5
	glVertex3f(-0.2f, 0.3f, 0.1f);
	glVertex3f(-0.2f, 0.1f, 0.3f);
	glVertex3f(0.2f, 0.1f, 0.3f);

	glVertex3f(-0.2f, 0.3f, -0.1f);                   //6
	glVertex3f(0.2f, 0.3f, -0.1f);
	glVertex3f(0.2f, 0.1f, -0.3f);
	glVertex3f(-0.2f, 0.1f, -0.3f);

	glVertex3f(0.2f, 0.1f, 0.3f);                   //7
	glVertex3f(-0.2f, 0.1f, 0.3f);
	glVertex3f(-0.2f, -0.1f, 0.3f);
	glVertex3f(0.2f, -0.1f, 0.3f);

	glVertex3f(0.2f, 0.1f, -0.3f);                   //8
	glVertex3f(-0.2f, 0.1f, -0.3f);
	glVertex3f(-0.2f, -0.1f, -0.3f);
	glVertex3f(0.2f, -0.1f, -0.3f);
	//-------------------------------------------------
	glVertex3f(0.2f, 0.35f, 0.1f);                   //9
	glVertex3f(-0.2f, 0.35f, 0.1f);
	glVertex3f(-0.2f, 0.35f, -0.1f);
	glVertex3f(0.2f, 0.35f, -0.1f);

	glVertex3f(0.2f, 0.35f, 0.1f);                   //10
	glVertex3f(-0.2f, 0.35f, 0.1f);
	glVertex3f(-0.2f, 0.1f, 0.35f);
	glVertex3f(0.2f, 0.1f, 0.35f);

	glVertex3f(-0.2f, 0.35f, -0.1f);                   //11
	glVertex3f(0.2f, 0.35f, -0.1f);
	glVertex3f(0.2f, 0.1f, -0.35f);
	glVertex3f(-0.2f, 0.1f, -0.35f);

	glVertex3f(0.2f, 0.1f, 0.35f);                   //12
	glVertex3f(-0.2f, 0.1f, 0.35f);
	glVertex3f(-0.2f, -0.1f, 0.35f);
	glVertex3f(0.2f, -0.1f, 0.35f);

	glVertex3f(0.2f, 0.1f, -0.35f);                   //13
	glVertex3f(-0.2f, 0.1f, -0.35f);
	glVertex3f(-0.2f, -0.1f, -0.35f);
	glVertex3f(0.2f, -0.1f, -0.35f);
	//-----------------------------------------
	glVertex3f(0.2f, -0.1f, -0.3f);                   //14
	glVertex3f(-0.2f, -0.1f, -0.3f);
	glVertex3f(-0.2f, -0.1f, -0.35f);
	glVertex3f(0.2f, -0.1f, -0.35f);

	glVertex3f(0.2f, -0.1f, 0.3f);                   //15
	glVertex3f(-0.2f, -0.1f, 0.3f);
	glVertex3f(-0.2f, -0.1f, 0.35f);
	glVertex3f(0.2f, -0.1f, 0.35f);

	glVertex3f(-0.2f, 0.1f, 0.3f);                   //16
	glVertex3f(-0.2f, -0.1f, 0.3f);
	glVertex3f(-0.2f, -0.1f, 0.35f);
	glVertex3f(-0.2f, 0.1f, 0.35f);

	glVertex3f(0.2f, 0.1f, 0.3f);                   //17
	glVertex3f(0.2f, -0.1f, 0.3f);
	glVertex3f(0.2f, -0.1f, 0.35f);
	glVertex3f(0.2f, 0.1f, 0.35f);

	glVertex3f(-0.2f, 0.1f, -0.3f);                   //18
	glVertex3f(-0.2f, -0.1f, -0.3f);
	glVertex3f(-0.2f, -0.1f, -0.35f);
	glVertex3f(-0.2f, 0.1f, -0.35f);

	glVertex3f(0.2f, 0.1f, -0.3f);                   //19
	glVertex3f(0.2f, -0.1f, -0.3f);
	glVertex3f(0.2f, -0.1f, -0.35f);
	glVertex3f(0.2f, 0.1f, -0.35f);

	glVertex3f(-0.2f, 0.3f, 0.1f);                   //20
	glVertex3f(-0.2f, 0.1f, 0.3f);
	glVertex3f(-0.2f, 0.1f, 0.35f);
	glVertex3f(-0.2f, 0.35f, 0.1f);

	glVertex3f(0.2f, 0.3f, 0.1f);                   //21
	glVertex3f(0.2f, 0.1f, 0.3f);
	glVertex3f(0.2f, 0.1f, 0.35f);
	glVertex3f(0.2f, 0.35f, 0.1f);

	glVertex3f(-0.2f, 0.3f, -0.1f);                   //22
	glVertex3f(-0.2f, 0.1f, -0.3f);
	glVertex3f(-0.2f, 0.1f, -0.35f);
	glVertex3f(-0.2f, 0.35f, -0.1f);

	glVertex3f(0.2f, 0.3f, -0.1f);                   //23
	glVertex3f(0.2f, 0.1f, -0.3f);
	glVertex3f(0.2f, 0.1f, -0.35f);
	glVertex3f(0.2f, 0.35f, -0.1f);

	glVertex3f(-0.2f, 0.3f, 0.1f);                   //24
	glVertex3f(-0.2f, 0.3f, -0.1f);
	glVertex3f(-0.2f, 0.35f, -0.1f);
	glVertex3f(-0.2f, 0.35f, 0.1f);

	glVertex3f(-0.2f, 0.3f, 0.1f);                   //25
	glVertex3f(-0.2f, 0.3f, -0.1f);
	glVertex3f(-0.2f, 0.35f, -0.1f);
	glVertex3f(-0.2f, 0.35f, 0.1f);


	glEnd();
	glPopMatrix();
}
void lefthandArmor(float size) {
	//total 10 polygon
	GLuint texture;
	texture = loadTexture("valour.bmp");
	glPushMatrix();
	glScalef(size, size, size);
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.2f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);                   //1
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.5f, 0.25f, 0.0f);
	glVertex3f(-0.5f, -0.25f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);
	glVertex3f(0.5f, -0.25f, 0.0f);
	glVertex3f(0.5f, 0.25f, 0.0f);
	glEnd();

	glColor3f(1.0f, 0.8078f, 0.3490f);
	glBegin(GL_POLYGON);                   //2
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 0.5f, -0.2f);
	glTexCoord2f(0.0f, 0.8f);
	glVertex3f(-0.4f, 0.25f, -0.2f);
	glTexCoord2f(0.0f, 0.2f);
	glVertex3f(-0.4f, -0.25f, -0.2f);
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, -0.2f);
	glTexCoord2f(1.0f, 0.2f);
	glVertex3f(0.4f, -0.25f, -0.2f);
	glTexCoord2f(1.0f, 0.8f);
	glVertex3f(0.4f, 0.25f, -0.2f);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //3
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.5f, 0.25f, 0.0f);
	glVertex3f(-0.4f, 0.25f, -0.2f);
	glVertex3f(0.0f, 0.5f, -0.2f);

	glVertex3f(-0.5f, 0.25f, 0.0f);                   //4
	glVertex3f(-0.5f, -0.25f, 0.0f);
	glVertex3f(-0.4f, -0.25f, -0.2f);
	glVertex3f(-0.4f, 0.25f, -0.2f);

	glVertex3f(-0.5f, -0.25f, 0.0f);                   //5
	glVertex3f(0.0f, -0.6f, 0.0f);
	glVertex3f(0.0f, -0.5f, -0.2f);
	glVertex3f(-0.4f, -0.25f, -0.2f);

	glVertex3f(0.0f, -0.6f, 0.0f);                   //6
	glVertex3f(0.5f, -0.25f, 0.0f);
	glVertex3f(0.4f, -0.25f, -0.2f);
	glVertex3f(0.0f, -0.5f, -0.2f);

	glVertex3f(0.5f, -0.25f, 0.0f);                   //7
	glVertex3f(0.5f, 0.25f, 0.0f);
	glVertex3f(0.4f, 0.25f, -0.2f);
	glVertex3f(0.4f, -0.25f, -0.2f);

	glVertex3f(0.5f, 0.25f, 0.0f);                   //8
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(0.0f, 0.5f, -0.2f);
	glVertex3f(0.4f, 0.25f, -0.2f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);                   //9
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 0.8078f, 0.3490f);
	GLUquadricObj *armor = NULL;
	armor = gluNewQuadric();
	gluQuadricDrawStyle(armor, GLU_FILL);
	gluCylinder(armor, 0.15, 0.35, 0.2, 40, 40);                   //10
	gluDeleteQuadric(armor);
	glPopMatrix();
	glPopMatrix();
	glDeleteTextures(1, &texture);
}
void cannonBall(float size) {
	glPushMatrix();
	glScalef(size, size, size);
	GLUquadricObj *cannon = NULL;
	cannon = gluNewQuadric();
	gluQuadricDrawStyle(cannon, GLU_FILL);
	gluSphere(cannon, 1.0, 60, 60);
	gluDeleteQuadric(cannon);
	glPopMatrix();
}
void righthandWeapon(float size) {
	//total 5 polygon
	GLuint texture;
	texture = loadTexture("cannon.bmp");
	glPushMatrix();
	glScalef(size, size, size);
	glPushMatrix();
	glScalef(2.3f, 1.3f, 1.3f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.3f);

	glPushMatrix();
	glColor3f(0.3490f, 0.3490f, 0.3490f);
	GLUquadricObj *rhw1 = NULL;
	rhw1 = gluNewQuadric();
	gluQuadricDrawStyle(rhw1, GLU_FILL);                   //1
	gluQuadricTexture(rhw1, true);
	gluCylinder(rhw1, 0.35, 0.3, 1.0, 60, 60);
	gluDeleteQuadric(rhw1);
	glPopMatrix();

	glPushMatrix();

	if (RWeaponOn) {
		if (RweaponTranslate < 0.99f) {
			RweaponTranslate += 0.05f;
		}
		else if (RweaponScale < 1.0f) {
			RweaponScale += 0.05f;
		}
	}
	else {
		if (RweaponScale > 0.30f) {
			RweaponScale -= 0.05f;
		}
		else if (RweaponTranslate > 0.0f) {
			RweaponTranslate -= 0.05f;
		}
	}

	glTranslatef(0.0f, 0.0f, RweaponTranslate);
	glScalef(RweaponScale, RweaponScale, RweaponScale);
	glColor3f(0.3490f, 0.3490f, 0.3490f);
	GLUquadricObj *rhw2 = NULL;                   //2
	rhw2 = gluNewQuadric();
	gluQuadricDrawStyle(rhw2, GLU_FILL);
	gluQuadricTexture(rhw2, true);
	gluCylinder(rhw2, 0.3, 0.5, 0.5, 60, 60);
	gluDeleteQuadric(rhw2);

	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_QUADS);                   //3
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glEnd();
	glColor3f(0.3490f, 0.3490f, 0.3490f);
	GLUquadricObj *rhw3 = NULL;
	rhw3 = gluNewQuadric();
	gluQuadricDrawStyle(rhw3, GLU_FILL);                   //4
	gluQuadricTexture(rhw3, true);
	gluCylinder(rhw3, 0.5, 0.3, 1.0, 60, 60);
	gluDeleteQuadric(rhw3);

	glColor3f(0.5, 0.5, 0.5);
	GLUquadricObj *rhw4 = NULL;                   //5
	rhw4 = gluNewQuadric();
	gluQuadricDrawStyle(rhw4, GLU_FILL);
	gluCylinder(rhw4, 0.15, 0.3, 1.0, 60, 60);
	gluDeleteQuadric(rhw4);
	glDeleteTextures(1, &texture);
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	if (currentCannonT > targetCannonT) {
		currentCannonT -= 0.200f;
	}
	if (currentCannonS < targetCannonS) {
		currentCannonS += 0.010f;
	}
	glScalef(currentCannonS, currentCannonS, currentCannonS);
	glTranslatef(currentCannonT, 0.0f, 0.0f);

	cannonBall(0.5);
	glPopMatrix();
	glPopMatrix();
}
void lefthandWeapon(float size) {
	//total 18 polygon
	GLuint texture;
	texture = loadTexture("sword.bmp");
	glPushMatrix();
	glScalef(size, size, size);
	glScalef(1.5f, 1.0f, 0.9f);

	glPushMatrix();
	if (LWeaponOn) {
		if (LweaponScale < 1.0f) {
			LweaponScale += 0.005f;
		}
	}
	else {
		if (LweaponScale > 0.0f) {
			LweaponScale -= 0.01f;
		}
	}
	glScalef(LweaponScale, LweaponScale, LweaponScale);
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_POLYGON);                   //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 0.05f, 0.4f);
	glTexCoord2f(0.0f, 0.15f);
	glVertex3f(5.0f, 0.05f, 0.4f);
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(5.5f, 0.05f, 0.0f);
	glTexCoord2f(1.0f, 0.15f);
	glVertex3f(5.0f, 0.05f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.05f, -0.4f);
	glEnd();

	glBegin(GL_POLYGON);                   //2
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, -0.05f, 0.4f);
	glTexCoord2f(0.0f, 0.15f);
	glVertex3f(5.0f, -0.05f, 0.4f);
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(5.5f, -0.05f, 0.0f);
	glTexCoord2f(1.0f, 0.15f);
	glVertex3f(5.0f, -0.05f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -0.05f, -0.4f);
	glEnd();

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);                   //3
	glVertex3f(1.0f, 0.05f, 0.4f);
	glVertex3f(5.0f, 0.05f, 0.4f);
	glVertex3f(5.0f, 0.10f, 0.5f);
	glVertex3f(1.0f, 0.10f, 0.5f);

	glVertex3f(1.0f, 0.05f, -0.4f);                   //4
	glVertex3f(5.0f, 0.05f, -0.4f);
	glVertex3f(5.0f, 0.10f, -0.5f);
	glVertex3f(1.0f, 0.10f, -0.5f);

	glVertex3f(1.0f, -0.05f, 0.4f);                   //5
	glVertex3f(5.0f, -0.05f, 0.4f);
	glVertex3f(5.0f, -0.10f, 0.5f);
	glVertex3f(1.0f, -0.10f, 0.5f);

	glVertex3f(1.0f, -0.05f, -0.4f);                   //6
	glVertex3f(5.0f, -0.05f, -0.4f);
	glVertex3f(5.0f, -0.10f, -0.5f);
	glVertex3f(1.0f, -0.10f, -0.5f);
	//-----------------------------------
	glVertex3f(5.0f, 0.05f, 0.4f);                   //7
	glVertex3f(5.5f, 0.05f, 0.0f);
	glVertex3f(5.6f, 0.10f, 0.0f);
	glVertex3f(5.0f, 0.10f, 0.5f);

	glVertex3f(5.0f, 0.05f, -0.4f);                   //8
	glVertex3f(5.5f, 0.05f, -0.0f);
	glVertex3f(5.6f, 0.10f, -0.0f);
	glVertex3f(5.0f, 0.10f, -0.5f);

	glVertex3f(5.0f, -0.05f, 0.4f);                   //9
	glVertex3f(5.5f, -0.05f, 0.0f);
	glVertex3f(5.6f, -0.10f, 0.0f);
	glVertex3f(5.0f, -0.10f, 0.5f);

	glVertex3f(5.0f, -0.05f, -0.4f);                   //10
	glVertex3f(5.5f, -0.05f, -0.0f);
	glVertex3f(5.6f, -0.10f, -0.0f);
	glVertex3f(5.0f, -0.10f, -0.5f);
	//---------------------------------
	//---------------------------------
	glVertex3f(1.0f, 0.00f, 0.6f);                   //11
	glVertex3f(5.0f, 0.00f, 0.6f);
	glVertex3f(5.0f, 0.10f, 0.5f);
	glVertex3f(1.0f, 0.10f, 0.5f);

	glVertex3f(1.0f, 0.00f, -0.6f);                   //12
	glVertex3f(5.0f, 0.00f, -0.6f);
	glVertex3f(5.0f, 0.10f, -0.5f);
	glVertex3f(1.0f, 0.10f, -0.5f);

	glVertex3f(1.0f, -0.00f, 0.6f);                   //13
	glVertex3f(5.0f, -0.00f, 0.6f);
	glVertex3f(5.0f, -0.10f, 0.5f);
	glVertex3f(1.0f, -0.10f, 0.5f);

	glVertex3f(1.0f, -0.00f, -0.6f);                   //14
	glVertex3f(5.0f, -0.00f, -0.6f);
	glVertex3f(5.0f, -0.10f, -0.5f);
	glVertex3f(1.0f, -0.10f, -0.5f);
	//---------------------------------
	glVertex3f(5.0f, 0.00f, 0.6f);                   //15
	glVertex3f(6.0f, 0.00f, 0.0f);
	glVertex3f(5.6f, 0.10f, 0.0f);
	glVertex3f(5.0f, 0.10f, 0.5f);

	glVertex3f(5.0f, 0.00f, -0.6f);                   //16
	glVertex3f(6.0f, 0.00f, -0.0f);
	glVertex3f(5.6f, 0.10f, -0.0f);
	glVertex3f(5.0f, 0.10f, -0.5f);

	glVertex3f(5.0f, -0.00f, 0.6f);                   //17
	glVertex3f(6.0f, -0.00f, 0.0f);
	glVertex3f(5.6f, -0.10f, 0.0f);
	glVertex3f(5.0f, -0.10f, 0.5f);

	glVertex3f(5.0f, -0.00f, -0.6f);                   //18
	glVertex3f(6.0f, -0.00f, -0.0f);
	glVertex3f(5.6f, -0.10f, -0.0f);
	glVertex3f(5.0f, -0.10f, -0.5f);

	glEnd();
	glPopMatrix();


	glPopMatrix();
	glDeleteTextures(1, &texture);
}
void lefthandChin(float size) {

	//total 82 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glScalef(1.1f, 1.0f, 0.95f);
	glColor3f(0.8313f, 0.8313f, 0.8313f);


	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.2f, 0.0f); //(z,y,x)
	GLUquadricObj * q1 = NULL;
	q1 = gluNewQuadric();
	gluQuadricDrawStyle(q1, GLU_FILL);                   //1
	gluCylinder(q1, 0.2, 0.2, 0.8, 14, 14);
	gluDeleteQuadric(q1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25f, 0.2f, -0.4f);
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);                   //2
	gluCylinder(cylinder, 0.3, 0.3, 0.8, 14, 14);
	gluDeleteQuadric(cylinder);

	GLUquadricObj * q2 = NULL;
	q2 = gluNewQuadric();
	gluQuadricDrawStyle(q2, GLU_FILL);                   //3
	gluCylinder(q2, 0.0, 0.3, 0.8, 14, 14);
	gluDeleteQuadric(q2);

	GLUquadricObj * q3 = NULL;
	q3 = gluNewQuadric();
	gluQuadricDrawStyle(q3, GLU_FILL);                   //4
	gluCylinder(q3, 0.3, 0.0, 0.8, 14, 14);
	gluDeleteQuadric(q3);
	glPopMatrix();


	glBegin(GL_QUADS);                   //5
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glVertex3f(0.2f, 0.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //6
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glVertex3f(0.2f, 0.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //7
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //8
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //9
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //10
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //11
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //12
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //13
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //14
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //15
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //16
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //17
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //18
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //19
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //20
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //21
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //22
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //23
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //24
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glEnd();
	//--------------------------------------------2nd
	glBegin(GL_QUADS);                   //25
	glVertex3f(0.2f, 0.1f, 0.4f);
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glVertex3f(0.2f, 0.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //26
	glVertex3f(0.2f, 0.1f, -0.4f);
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glVertex3f(0.2f, 0.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //27
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //28
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //29
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //30
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //31
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //32
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //33
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //34
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //35
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //36
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //37
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //38
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //39
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //40
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //41
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //42
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glEnd();
	//--------------------------------3rd
	glBegin(GL_QUADS);                   //43
	glVertex3f(0.2f, 0.1f, 0.4f);
	glVertex3f(0.4f, 0.0f, 0.4f);
	glVertex3f(0.4f, -0.0f, 0.5f);
	glVertex3f(0.2f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //44
	glVertex3f(0.2f, 0.1f, -0.4f);
	glVertex3f(0.4f, 0.0f, -0.4f);
	glVertex3f(0.4f, -0.0f, -0.5f);
	glVertex3f(0.2f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //45
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glVertex3f(0.2f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //46
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glVertex3f(0.2f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //47
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //48
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //49
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //50
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //51
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //52
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //53
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //54
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //55
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //56
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //57
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //58
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glEnd();
	//----------------------------4th
	glBegin(GL_QUADS);                   //59
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //60
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //61
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //62
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //63
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //64
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //65
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //66
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //67
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //68
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //69
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //70
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //71
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);                   //72
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //73
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //74
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glEnd();
	//--------------------------------------close face
	glBegin(GL_QUADS);                   //75
	glVertex3f(0.4f, 0.0f, 0.4f);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.4f, 0.0f, -0.4f);
	glEnd();

	glBegin(GL_QUADS);                   //76
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glEnd();

	glBegin(GL_QUADS);                   //77
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glEnd();

	glBegin(GL_QUADS);                   //78
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glEnd();

	glBegin(GL_QUADS);                   //79
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glEnd();

	glBegin(GL_QUADS);                   //80
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glEnd();

	glBegin(GL_QUADS);                   //81
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //82
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glEnd();

	glPopMatrix();

}
void lefthandUpper(float size) {
	glPushMatrix();
	//TOTAL 86 POLYGONS
	glScalef(size, size, size);
	glTranslatef(0.0f, -0.25f, 0.0f);
	//-----------------------------------1st
	glColor3f(1.0f, 0.8078f, 0.3490f);
	glBegin(GL_QUADS);                   //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, 2.1f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //2
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, 2.1f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //3
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.7f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //4
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.7f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //5
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.7f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //6
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.7f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //7
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //8
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //9
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //10
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();
	//----------------------------------------2nd
	glBegin(GL_QUADS);                   //11
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.6f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //12
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.6f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //13
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //14
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //15
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //16
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.6f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //17
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //18
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 1.4f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //19
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //20
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.5f, 0.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();
	//---------------------------------3rd
	glBegin(GL_QUADS);                   //21
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.6f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //22
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 1.6f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //23
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //24
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //25
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //26
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.2f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //27
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.1f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //28
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 1.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.1f, -0.6f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //29
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.5f, 0.1f, 0.6f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //30
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.5f, -0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.5f, 0.1f, -0.6f);
	glEnd();
	//---------------------------------4th
	glBegin(GL_QUADS);                   //31
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.5f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //32
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.5f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //33
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //34
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //35
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //36
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.8f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //37
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, 0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.1f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //38
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.6f, -0.6f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.5f, 0.1f, -0.6f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //39
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.5f, 0.1f, 0.6f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //40
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, -0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.5f, 0.1f, -0.6f);
	glEnd();
	//---------------------------------5th
	glBegin(GL_QUADS);                   //41
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.5f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.2f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.2f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //42
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.5f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.2f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.2f, -1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //43
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.2f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //44
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.2f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.0f, -1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //45
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //46
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 0.4f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 0.0f, -1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //47
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, 0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, 0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.0f, -0.7f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //48
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.5f, 0.0f, -0.8f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.5f, 0.2f, -0.8f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.0f, -0.7f, -1.0f);
	glEnd();
	//------------------------------left hand eyes
	glColor3f(0.2392f, 0.2392f, 0.2392f);
	glPushMatrix();
	glBegin(GL_QUADS);                   //49
	glVertex3f(2.0f, 1.0f, 0.4f);
	glVertex3f(2.5f, 0.8f, 0.5f);
	glVertex3f(2.5f, 0.6f, 0.6f);
	glVertex3f(2.0f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //50
	glVertex3f(2.0f, 1.0f, -0.4f);
	glVertex3f(2.5f, 0.8f, -0.5f);
	glVertex3f(2.5f, 0.6f, -0.6f);
	glVertex3f(2.0f, 0.8f, -0.6f);
	glEnd();
	glPopMatrix();
	//-------------------------left hand eyes end
	//------------------------- yellow part
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.4f);
	glTranslatef(-0.3f, -0.15f, 0.0f);
	glBegin(GL_QUADS);                   //51
	glVertex3f(-0.5f, 2.1f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //52
	glVertex3f(-0.5f, 2.1f, 0.0f);
	glVertex3f(0.5f, 1.9f, 0.0f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //53
	glVertex3f(-0.2f, 1.6f, 0.4f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //54
	glVertex3f(-0.2f, 1.6f, -0.4f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //55
	glVertex3f(-0.2f, 1.6f, 0.4f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glVertex3f(0.0f, 1.0f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //56
	glVertex3f(-0.2f, 1.6f, -0.4f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glVertex3f(0.0f, 1.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //57
	glVertex3f(0.3f, 0.5f, 0.8f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glVertex3f(0.0f, 1.0f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //58
	glVertex3f(0.3f, 0.5f, -0.8f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glVertex3f(0.0f, 1.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //59
	glVertex3f(1.5f, 0.4f, 0.8f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glVertex3f(0.5f, 1.0f, 0.6f);
	glVertex3f(1.5f, 0.8f, 0.6f);
	glEnd();
	glBegin(GL_QUADS);                   //60
	glVertex3f(1.5f, 0.4f, -0.8f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glVertex3f(1.5f, 0.8f, -0.6f);
	glEnd();
	glPopMatrix();
	//----------------------yellow part end
	//------------------------------------------close face
	glColor3f(1.0f, 0.8078f, 0.3490f);
	glBegin(GL_TRIANGLES);                   //61
	glVertex3f(0.5f, 1.9f, 0.0f);
	glVertex3f(0.5f, 1.8f, 0.2f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //62
	glVertex3f(0.5f, 1.8f, 0.2f);
	glVertex3f(0.5f, 1.8f, -0.2f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //63
	glVertex3f(0.5f, 1.0f, 0.6f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glVertex3f(0.5f, 1.4f, -0.4f);
	glVertex3f(0.5f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //64
	glVertex3f(0.5f, 1.0f, 0.6f);
	glVertex3f(0.5f, 1.0f, -0.6f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glEnd();
	glBegin(GL_QUADS);                   //65
	glVertex3f(0.5f, 0.2f, 1.0f);
	glVertex3f(0.5f, 0.2f, -1.0f);
	glVertex3f(0.5f, 0.6f, -0.8f);
	glVertex3f(0.5f, 0.6f, 0.8f);
	glEnd();

	glBegin(GL_QUADS);                   //66
	glVertex3f(0.5f, 0.2f, 1.0f);
	glVertex3f(0.5f, 0.2f, -1.0f);
	glVertex3f(1.5f, 0.0f, -1.0f);
	glVertex3f(1.5f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //67
	glVertex3f(2.5f, 0.0f, 1.0f);
	glVertex3f(2.5f, 0.0f, -1.0f);
	glVertex3f(1.5f, 0.0f, -1.0f);
	glVertex3f(1.5f, 0.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);                   //68
	glVertex3f(2.5f, 0.0f, 1.0f);
	glVertex3f(2.5f, 0.0f, -1.0f);
	glVertex3f(3.5f, 0.0f, -0.8f);
	glVertex3f(3.5f, 0.0f, 0.8f);
	glEnd();
	glBegin(GL_QUADS);                   //69
	glVertex3f(4.0f, 0.0f, 0.4f);
	glVertex3f(4.0f, 0.0f, -0.4f);
	glVertex3f(3.5f, 0.0f, -0.8f);
	glVertex3f(3.5f, 0.0f, 0.8f);
	glEnd();
	glBegin(GL_QUADS);                   //70
	glVertex3f(4.0f, 0.0f, 0.4f);
	glVertex3f(4.0f, 0.0f, -0.4f);
	glVertex3f(4.5f, 0.0f, -0.2f);
	glVertex3f(4.5f, 0.0f, 0.2f);
	glEnd();
	//----------------------close face end
	//---------------------------------------horn start
	glPushMatrix();
	glColor3f(0.9058f, 0.8588f, 0.7961f);
	glTranslatef(1.0f, 1.0f, 0.4f);
	GLUquadricObj *h1 = NULL;
	h1 = gluNewQuadric();
	gluQuadricDrawStyle(h1, GLU_FILL);                   //71
	gluCylinder(h1, 0.2, 0.2, 0.4, 14, 14);
	gluDeleteQuadric(h1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0f, 1.0f, -0.8f);
	GLUquadricObj *h2 = NULL;
	h2 = gluNewQuadric();
	gluQuadricDrawStyle(h2, GLU_FILL);                   //72
	gluCylinder(h2, 0.2, 0.2, 0.4, 14, 14);
	gluDeleteQuadric(h2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, 0.95f, 0.7f);
	glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *hh1 = NULL;
	hh1 = gluNewQuadric();
	gluQuadricDrawStyle(hh1, GLU_FILL);                   //73
	gluCylinder(hh1, 0.2, 0.15, 0.4, 14, 14);
	gluDeleteQuadric(hh1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0f, 0.95f, -0.7f);
	glRotatef(-135.0f, 1.0f, 0.0f, 0.0f);

	GLUquadricObj *hh2 = NULL;
	hh2 = gluNewQuadric();
	gluQuadricDrawStyle(hh2, GLU_FILL);                   //74
	gluCylinder(hh2, 0.2, 0.15, 0.4, 14, 14);
	gluDeleteQuadric(hh2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0f, 1.2f, 1.0f);
	glBegin(GL_QUADS);                   //75
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //76
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //77
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //78
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.15f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, 1.2f, -1.0f);
	glBegin(GL_QUADS);                   //79
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //80
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //81
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.15f);
	glEnd();
	glBegin(GL_QUADS);                   //82
	glVertex3f(0.0f, 0.6f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.15f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.15f);
	glEnd();
	glPopMatrix();

	glBegin(GL_TRIANGLES);                   //83
	glVertex3f(3.25f, 2.5f, 0.0f);
	glVertex3f(3.0f, 0.9f, 0.2f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //84
	glVertex3f(3.25f, 2.5f, 0.0f);
	glVertex3f(3.0f, 0.9f, -0.2f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //85
	glVertex3f(3.25f, 2.5f, 0.0f);
	glVertex3f(3.0f, 0.9f, -0.2f);
	glVertex3f(3.0f, 0.9f, 0.2f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //86
	glVertex3f(3.25f, 2.5f, 0.0f);
	glVertex3f(3.5f, 0.9f, 0.2f);
	glVertex3f(3.5f, 0.9f, -0.2f);
	glEnd();

	glPopMatrix();
}
void righthandChin(float size) {
	glPushMatrix();
	//total 82 polygon
	glScalef(size, size, size);
	glScalef(1.1f, 1.0f, 1.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glColor3f(0.8313f, 0.8313f, 0.8313f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.2f, 0.0f); //(z,y,x)
	GLUquadricObj * q1 = NULL;
	q1 = gluNewQuadric();
	gluQuadricDrawStyle(q1, GLU_FILL);
	gluCylinder(q1, 0.2, 0.2, 0.8, 14, 14);
	gluDeleteQuadric(q1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25f, 0.2f, -0.4f);
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, 0.3, 0.3, 0.8, 14, 14);
	gluDeleteQuadric(cylinder);

	GLUquadricObj * q2 = NULL;
	q2 = gluNewQuadric();
	gluQuadricDrawStyle(q2, GLU_FILL);
	gluCylinder(q2, 0.0, 0.3, 0.8, 14, 14);
	gluDeleteQuadric(q2);

	GLUquadricObj * q3 = NULL;
	q3 = gluNewQuadric();
	gluQuadricDrawStyle(q3, GLU_FILL);
	gluCylinder(q3, 0.3, 0.0, 0.8, 14, 14);
	gluDeleteQuadric(q3);
	glPopMatrix();


	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glVertex3f(0.2f, 0.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glVertex3f(0.2f, 0.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -1.1f, 0.0f);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -1.0f, 0.0f);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -0.8f, 0.0f);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(3.2f, -0.5f, 0.0f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glEnd();
	//--------------------------------------------2nd
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.1f, 0.4f);
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glVertex3f(0.2f, 0.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.1f, -0.4f);
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glVertex3f(0.2f, 0.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glVertex3f(0.2f, -0.2f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glVertex3f(0.2f, -0.2f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glVertex3f(0.4f, -0.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glVertex3f(0.4f, -0.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glVertex3f(0.7f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glVertex3f(0.7f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glVertex3f(1.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glVertex3f(1.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glVertex3f(2.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glVertex3f(2.0f, -0.8f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glVertex3f(2.5f, -1.1f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glVertex3f(2.5f, -1.1f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glVertex3f(2.9f, -1.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glVertex3f(2.9f, -1.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.0f, -0.8f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.0f, -0.8f, -0.2f);
	glEnd();
	//--------------------------------3rd
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.1f, 0.4f);
	glVertex3f(0.4f, 0.0f, 0.4f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glVertex3f(0.2f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.1f, -0.4f);
	glVertex3f(0.4f, 0.0f, -0.4f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glVertex3f(0.2f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, 0.4f);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glVertex3f(0.2f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 0.0f, -0.4f);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glVertex3f(0.2f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.3f, 0.4f);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.4f, -0.3f, -0.4f);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -0.8f, 0.4f);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.7f, -0.8f, -0.4f);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.6f, 0.4f);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, -0.6f, -0.4f);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.9f, 0.4f);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.9f, -0.4f);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, 0.4f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.8f, -0.4f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glEnd();
	//----------------------------4th
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.0f, -0.6f, 0.4f);
	glVertex3f(3.2f, -0.5f, 0.2f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.0f, -0.6f, -0.4f);
	glVertex3f(3.2f, -0.5f, -0.2f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(3.0f, -0.3f, 0.5f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(3.0f, -0.3f, -0.5f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.9f, -0.5f, 0.5f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.9f, -0.5f, -0.5f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.6f, 0.5f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.5f, -0.6f, -0.5f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glVertex3f(2.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glVertex3f(2.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glVertex3f(1.0f, -0.3f, 0.5f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glVertex3f(1.0f, -0.3f, -0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.4f, 0.0f, 0.5f);
	glVertex3f(0.7f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.4f, 0.0f, -0.5f);
	glVertex3f(0.7f, -0.5f, -0.5f);
	glEnd();
	//--------------------------------------close face
	glBegin(GL_QUADS);
	glVertex3f(0.4f, 0.0f, 0.4f);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glVertex3f(0.4f, 0.0f, -0.4f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.7f, 0.0f, 0.6f);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glVertex3f(0.7f, 0.0f, -0.6f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(1.0f, 0.0f, 0.65f);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glVertex3f(1.0f, 0.0f, -0.65f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2.0f, 0.0f, 0.7f);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glVertex3f(2.0f, 0.0f, -0.7f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2.5f, 0.0f, 0.7f);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glVertex3f(2.5f, 0.0f, -0.7f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(2.9f, 0.0f, 0.65f);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glVertex3f(2.9f, 0.0f, -0.65f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(3.0f, 0.0f, 0.6f);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glVertex3f(3.0f, 0.0f, -0.6f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(3.2f, 0.0f, 0.4f);
	glVertex3f(3.7f, 0.0f, 0.0f);
	glVertex3f(3.2f, 0.0f, -0.4f);
	glEnd();

	glPopMatrix();

}
void righthandUpper(float size) {
	glPushMatrix();
	glScalef(size, size, size);
	glTranslatef(0.0f, -0.5f, 0.0f);
	//total 196 polygon
	//-------------------------------1st
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //2
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //3
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //4
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //5
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //6
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //7
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //8
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //9
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //10
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //11
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //12
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.2f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //13
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //14
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.2f);
	glEnd();
	//-------------------------------2nd
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //15
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //16
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //17
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //18
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //19
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //20
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //21
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //22
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //23
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //24
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //25
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //26
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.2f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //27
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //28
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.2f);
	glEnd();
	//-------------------------------3rd
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //29
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //30
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //31
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //32
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //33
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //34
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //35
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //36
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.1f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //37
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //38
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //39
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //40
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.4f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //41
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, 0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //42
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.5f, -0.3f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.4f);
	glEnd();
	//-------------------------------4th
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //43
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //44
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //45
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //46
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //47
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //48
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //49
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //50
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //51
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //52
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //53
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //54
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.4f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //55
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, 0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //56
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, -0.4f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.4f);
	glEnd();

	//-------------------------------5th
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //57
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.3f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //58
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.5f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.3f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //59
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //60
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //61
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //62
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.3f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //63
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //64
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //65
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //66
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //67
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //68
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, -0.65f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);                   //69
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, 0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //70
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.4f, -0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.4f, -0.45f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, -0.65f);
	glEnd();

	//-------------------------------6th
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //71
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.2f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.3f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //72
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.2f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 2.3f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //73
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //74
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //75
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //76
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 2.0f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 2.1f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //77
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //78
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.8f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //79
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //80
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //81
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //82
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //83
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.2f, 0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //84
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 1.2f, -0.65f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 1.2f, -0.65f);
	glEnd();

	//-------------------------------7th
	glBegin(GL_QUADS);                   //85
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.2f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.9f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //86
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 2.2f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.9f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //87
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.6f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, 0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //88
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.6f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //89
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.6f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, 0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //90
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.6f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //91
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //92
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.7f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //93
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //94
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //95
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //96
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //97
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, 0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.1f, 0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //98
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 1.1f, -0.7f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 1.1f, -0.7f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, -0.71f);
	glEnd();

	//-------------------------------8th
	glBegin(GL_QUADS);                   //99
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 1.5f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.9f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //100
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 1.5f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.9f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //101
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, 0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //102
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //103
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, 0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //104
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.3f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //105
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //106
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 1.4f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //107
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //108
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //109
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //110
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //111
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.8f, 0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //112
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.8f, -0.71f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.8f, -0.71f);
	glEnd();

	//-------------------------------9th
	glBegin(GL_QUADS);                   //113
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 1.5f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.1f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //114
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 1.5f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.1f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //115
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //116
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //117
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //118
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.9f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //119
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //120
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //121
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.5f, 0.78f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //122
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.5f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //123
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.5f, 0.78f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //124
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 0.5f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //125
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 0.7f, 0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.5f, 0.7f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //126
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 0.7f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 0.7f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-4.5f, 0.5f, -0.7f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, 0.5f, -0.78f);
	glEnd();

	//-------------------------------10th
	glBegin(GL_QUADS);                   //127
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.6f, 0.60f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.1f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //128
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.6f, -0.60f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 1.1f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //129
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //130
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //131
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //132
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.8f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.7f, -0.85f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //133
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, 0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 0.8f, 0.85f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //134
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.8f, 0.5f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 0.8f, -0.85f);
	glEnd();

	//-------------------------------11th

	glBegin(GL_QUADS);                   //135
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.4f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.4f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //136
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.4f, 0.4f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.4f, -0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //137
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, 0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.6f, 0.4f, 0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.4f, 0.85f);
	glEnd();
	glBegin(GL_QUADS);                   //138
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.6f, -0.90f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.6f, 0.4f, -0.85f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 0.4f, -0.85f);
	glEnd();

	//---------------------------close face
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_QUADS);                   //139
	glVertex3f(0.0f, 2.2f, 0.45f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glEnd();

	glBegin(GL_QUADS);                   //140
	glVertex3f(0.0f, 1.9f, 0.7f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //141
	glVertex3f(0.0f, 1.9f, 0.7f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //142
	glVertex3f(0.0f, 1.2f, 0.78f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //143
	glVertex3f(0.0f, 1.2f, 0.78f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //144
	glVertex3f(0.0f, 0.6f, 0.90f);
	glVertex3f(0.0f, 1.0f, 0.85f);
	glVertex3f(0.0f, 1.0f, -0.85f);
	glVertex3f(0.0f, 0.6f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //145
	glVertex3f(0.0f, 0.6f, 0.90f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glVertex3f(0.0f, 0.6f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //146
	glVertex3f(-2.0f, 0.6f, 0.90f);
	glVertex3f(-1.0f, 0.6f, 0.90f);
	glVertex3f(-1.0f, 0.6f, -0.90f);
	glVertex3f(-2.0f, 0.6f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //147
	glVertex3f(-2.0f, 0.6f, 0.90f);
	glVertex3f(-2.8f, 0.5f, 0.78f);
	glVertex3f(-2.8f, 0.5f, -0.78f);
	glVertex3f(-2.0f, 0.6f, -0.90f);
	glEnd();
	glBegin(GL_QUADS);                   //148
	glVertex3f(-3.0f, 0.5f, 0.78f);
	glVertex3f(-2.8f, 0.5f, 0.78f);
	glVertex3f(-2.8f, 0.5f, -0.78f);
	glVertex3f(-3.0f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //149
	glVertex3f(-3.0f, 0.5f, 0.78f);
	glVertex3f(-4.0f, 0.5f, 0.78f);
	glVertex3f(-4.0f, 0.5f, -0.78f);
	glVertex3f(-3.0f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_QUADS);                   //150
	glVertex3f(-4.5f, 0.5f, 0.7f);
	glVertex3f(-4.0f, 0.5f, 0.78f);
	glVertex3f(-4.0f, 0.5f, -0.78f);
	glVertex3f(-4.5f, 0.5f, -0.7f);
	glEnd();

	glBegin(GL_QUADS);                   //151
	glVertex3f(-4.5f, 0.5f, 0.7f);
	glVertex3f(-4.5f, 0.7f, 0.78f);
	glVertex3f(-4.5f, 0.7f, -0.78f);
	glVertex3f(-4.5f, 0.5f, -0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //152
	glVertex3f(-4.5f, 0.8f, 0.71f);
	glVertex3f(-4.5f, 0.7f, 0.78f);
	glVertex3f(-4.5f, 0.7f, -0.78f);
	glVertex3f(-4.5f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //153
	glVertex3f(-4.5f, 0.8f, 0.71f);
	glVertex3f(-4.5f, 1.1f, 0.7f);
	glVertex3f(-4.5f, 1.1f, -0.7f);
	glVertex3f(-4.5f, 0.8f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //154
	glVertex3f(-4.5f, 1.2f, 0.65f);
	glVertex3f(-4.5f, 1.1f, 0.7f);
	glVertex3f(-4.5f, 1.1f, -0.7f);
	glVertex3f(-4.5f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //155
	glVertex3f(-4.5f, 1.2f, 0.65f);
	glVertex3f(-4.5f, 1.4f, 0.4f);
	glVertex3f(-4.5f, 1.4f, -0.4f);
	glVertex3f(-4.5f, 1.2f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //156
	glVertex3f(-4.5f, 1.5f, 0.3f);
	glVertex3f(-4.5f, 1.4f, 0.4f);
	glVertex3f(-4.5f, 1.4f, -0.4f);
	glVertex3f(-4.5f, 1.5f, -0.3f);
	glEnd();


	// ------------------------------add on
	glPushMatrix();
	glTranslatef(0.25f, -0.1f, 0.0f);
	glColor3f(0.6274f, 0.7882f, 1.0f);
	glBegin(GL_QUADS);                   //157
	glVertex3f(0.5f, 2.5f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glVertex3f(0.5f, 2.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //158
	glVertex3f(0.5f, 2.5f, 0.0f);
	glVertex3f(0.0f, 2.2f, 0.0f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glVertex3f(0.5f, 2.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //159
	glVertex3f(0.5f, 2.5f, 0.3f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glVertex3f(0.0f, 2.2f, 0.2f);
	glVertex3f(0.5f, 2.5f, 0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //160
	glVertex3f(0.5f, 2.5f, -0.3f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glVertex3f(0.0f, 2.2f, -0.2f);
	glVertex3f(0.5f, 2.5f, -0.2f);
	glEnd();
	glBegin(GL_QUADS);                   //161
	glVertex3f(0.5f, 2.5f, 0.3f);
	glVertex3f(0.0f, 2.2f, 0.3f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glVertex3f(0.5f, 2.5f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);                   //162
	glVertex3f(0.5f, 2.5f, -0.3f);
	glVertex3f(0.0f, 2.2f, -0.3f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glVertex3f(0.5f, 2.5f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //163
	glVertex3f(0.5f, 2.5f, 0.45f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glVertex3f(0.0f, 2.2f, 0.4f);
	glVertex3f(0.5f, 2.5f, 0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //164
	glVertex3f(0.5f, 2.5f, -0.45f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glVertex3f(0.0f, 2.2f, -0.4f);
	glVertex3f(0.5f, 2.5f, -0.4f);
	glEnd();
	glBegin(GL_QUADS);                   //165
	glVertex3f(0.5f, 2.5f, 0.45f);
	glVertex3f(0.0f, 2.2f, 0.45f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glVertex3f(0.5f, 2.3f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //166
	glVertex3f(0.5f, 2.5f, -0.45f);
	glVertex3f(0.0f, 2.2f, -0.45f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glVertex3f(0.5f, 2.3f, -0.65f);
	glEnd();

	glBegin(GL_QUADS);                   //167
	glVertex3f(0.5f, 2.2f, 0.7f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glVertex3f(0.0f, 2.0f, 0.65f);
	glVertex3f(0.5f, 2.3f, 0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //168
	glVertex3f(0.5f, 2.2f, -0.7f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glVertex3f(0.0f, 2.0f, -0.65f);
	glVertex3f(0.5f, 2.3f, -0.65f);
	glEnd();
	glBegin(GL_QUADS);                   //169
	glVertex3f(0.5f, 2.2f, 0.7f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glVertex3f(0.5f, 1.9f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //170
	glVertex3f(0.5f, 2.2f, -0.7f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glVertex3f(0.5f, 1.9f, -0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //171
	glVertex3f(0.5f, 1.5f, 0.78f);
	glVertex3f(0.0f, 1.2f, 0.78f);
	glVertex3f(0.0f, 1.6f, 0.71f);
	glVertex3f(0.5f, 1.9f, 0.71f);
	glEnd();
	glBegin(GL_QUADS);                   //172
	glVertex3f(0.5f, 1.5f, -0.78f);
	glVertex3f(0.0f, 1.2f, -0.78f);
	glVertex3f(0.0f, 1.6f, -0.71f);
	glVertex3f(0.5f, 1.9f, -0.71f);
	glEnd();

	glPopMatrix();
	//---------------------------ear
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_TRIANGLES);                   //173
	glVertex3f(-1.0f, 2.0f, 0.1f);
	glVertex3f(-0.5f, 2.0f, 0.35f);
	glVertex3f(0.0f, 2.8f, 0.7f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //174
	glVertex3f(-1.0f, 2.0f, -0.1f);
	glVertex3f(-0.5f, 2.0f, -0.35f);
	glVertex3f(0.0f, 2.8f, -0.7f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //175
	glVertex3f(-1.0f, 2.0f, 0.1f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glVertex3f(0.0f, 2.8f, 0.7f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //176
	glVertex3f(-1.0f, 2.0f, -0.1f);
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glVertex3f(0.0f, 2.8f, -0.7f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //177
	glColor3f(0.5804f, 0.5804f, 0.5804f);
	glVertex3f(-0.5f, 2.0f, 0.2f);
	glVertex3f(-0.5f, 2.0f, 0.35f);
	glVertex3f(0.0f, 2.8f, 0.7f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //178
	glVertex3f(-0.5f, 2.0f, -0.2f);
	glVertex3f(-0.5f, 2.0f, -0.35f);
	glVertex3f(0.0f, 2.8f, -0.7f);
	glEnd();

	// eyes
	glColor3f(0.2392f, 0.2392f, 0.2392f);
	glBegin(GL_QUADS);                   //179
	glVertex3f(-2.0f, 1.7f, 0.7f);
	glVertex3f(-1.0f, 1.6f, 0.7f);
	glVertex3f(0.0f, 1.9f, 0.7f);
	glVertex3f(-1.0f, 2.0f, 0.7f);
	glEnd();
	glBegin(GL_QUADS);                   //180
	glVertex3f(-2.0f, 1.7f, -0.7f);
	glVertex3f(-1.0f, 1.6f, -0.7f);
	glVertex3f(0.0f, 1.9f, -0.7f);
	glVertex3f(-1.0f, 2.0f, -0.7f);
	glEnd();

	//tooth
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	glBegin(GL_TRIANGLES);                   //181
	glVertex3f(-3.9f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.6f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //182
	glVertex3f(-3.8f, 0.6f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.7f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //183
	glVertex3f(-3.9f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.2f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //184
	glVertex3f(-3.7f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.2f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //185
	glVertex3f(-3.9f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.6f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //186
	glVertex3f(-3.8f, 0.6f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.7f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //187
	glVertex3f(-3.9f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.2f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //188
	glVertex3f(-3.7f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.2f, -0.78f);
	glEnd();
	glPushMatrix();
	glTranslatef(0.3f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);                   //189
	glVertex3f(-3.9f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.6f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //190
	glVertex3f(-3.8f, 0.6f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.7f, 0.5f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //191
	glVertex3f(-3.9f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.2f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //192
	glVertex3f(-3.7f, 0.5f, 0.78f);
	glVertex3f(-3.8f, 0.5f, 0.85f);
	glVertex3f(-3.8f, 0.2f, 0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //193
	glVertex3f(-3.9f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.6f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //194
	glVertex3f(-3.8f, 0.6f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.7f, 0.5f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //195
	glVertex3f(-3.9f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.2f, -0.78f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //196
	glVertex3f(-3.7f, 0.5f, -0.78f);
	glVertex3f(-3.8f, 0.5f, -0.85f);
	glVertex3f(-3.8f, 0.2f, -0.78f);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
void leftlowerArm(float size) {

	glPushMatrix();
	if (LisOpen) {
		if (LmouthU < 5.0f) {
			LmouthU += 0.05f;
		}
	}
	else
	{
		if (LmouthU > 0.0f)
		{
			LmouthU -= 0.05f;
		}
	}
	glRotatef(LmouthU, 0.0, 0.0, 1.0);
	GLuint textures[4];
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		lefthandUpper(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		lefthandUpper(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		lefthandUpper(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		lefthandUpper(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}

	glPopMatrix();
	glPushMatrix();
	if (LisOpen) {
		if (LmouthC > -5.0f) {
			LmouthC -= 0.1f;
		}
	}
	else
	{
		if (LmouthC < 0.0f)
		{
			LmouthC += 0.1f;
		}
	}
	glRotatef(LmouthC, 0.0f, 0.0f, 1.0f);
	lefthandChin(size);
	glPopMatrix();

	lefthandWeapon(size);

}
void rightlowerArm(float size) {
	glPushMatrix();
	if (RisOpen) {
		if (RmouthU < 7.0f) {
			RmouthU += 0.05f;
		}
	}
	else
	{
		if (RmouthU > 0.0f)
		{
			RmouthU -= 0.05f;
		}

	}
	glRotatef(RmouthU, 0.0f, 0.0f, 1.0f);

	righthandChin(size);
	glPopMatrix();
	glPushMatrix();
	if (RisOpen) {
		if (RmouthC > -8.0f) {
			RmouthC -= 0.05f;
		}
	}
	else
	{
		if (RmouthC < 0.0f)
		{
			RmouthC += 0.05f;
		}
	}
	glRotatef(RmouthC, 0.0f, 0.0f, 1.0f);
	GLuint textures[4];
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		righthandUpper(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		righthandUpper(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		righthandUpper(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		righthandUpper(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}


	glPopMatrix();
	righthandWeapon(size);
}
void leftArm(float size) {
	//total 9 polygon
	glPushMatrix();
	glScalef(size, size, size);
	lefthandArmor(1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	if (LHRotateUP) {
		if (currentLHRotateUD > targetLHRotateUD) {
			currentLHRotateUD -= 1.0f;
		}
		else {
			LHRotateUP = false;
		}
	}
	else {
		if (currentLHRotateUD < targetLHRotateUD) {
			currentLHRotateUD += 1.0f;
		}
		else {
			LHRotateUP = true;
		}
	}
	if (LHRotateFront) {
		if (currentLHRotateFB > targetLHRotateFB) {
			currentLHRotateFB -= 1.0f;
		}
		else {
			LHRotateFront = false;
		}
	}
	else {
		if (currentLHRotateFB < targetLHRotateFB) {
			currentLHRotateFB += 1.0f;
		}
		else {
			LHRotateFront = true;
		}
	}
	if (animation1) {
		if (a1) {
			if (currentLHRotateFB < targetLHRotateFB) {
				currentLHRotateFB += 1.0f;
			}
			else if (currentLHRotateUD > targetLHRotateUD) {
				currentLHRotateUD -= 1.0f;
			}
			else {
				a1 = false;
			}

		}
		else {
			targetCannonS = 3.0f;
			targetCannonT = -100.0f;

		}

	}
	glRotatef(currentLHRotateUD, 1.0f, 0.0f, 0.0f);
	glRotatef(currentLHRotateFB, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 0.8078f, 0.3490f);
	GLUquadricObj *up1 = NULL;
	up1 = gluNewQuadric();
	gluQuadricDrawStyle(up1, GLU_FILL);                   //1
	gluQuadricTexture(up1, true);
	gluSphere(up1, 0.30, 60, 60);
	gluDeleteQuadric(up1);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.29f);
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	GLUquadricObj *uc1 = NULL;
	uc1 = gluNewQuadric();
	gluQuadricDrawStyle(uc1, GLU_FILL);                   //2
	gluCylinder(uc1, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc1);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc2 = NULL;
	uc2 = gluNewQuadric();
	gluQuadricDrawStyle(uc2, GLU_FILL);                   //3
	gluCylinder(uc2, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc2);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc3 = NULL;
	uc3 = gluNewQuadric();
	gluQuadricDrawStyle(uc3, GLU_FILL);                   //4
	gluCylinder(uc3, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc3);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc4 = NULL;
	uc4 = gluNewQuadric();
	gluQuadricDrawStyle(uc4, GLU_FILL);                   //5
	gluCylinder(uc4, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc4);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc5 = NULL;
	uc5 = gluNewQuadric();
	gluQuadricDrawStyle(uc5, GLU_FILL);                   //6
	gluCylinder(uc5, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc5);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc6 = NULL;
	uc6 = gluNewQuadric();
	gluQuadricDrawStyle(uc6, GLU_FILL);                   //7
	gluCylinder(uc6, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc6);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc7 = NULL;
	uc7 = gluNewQuadric();
	gluQuadricDrawStyle(uc7, GLU_FILL);                   //8
	gluCylinder(uc7, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc7);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc8 = NULL;
	uc8 = gluNewQuadric();
	gluQuadricDrawStyle(uc8, GLU_FILL);                   //9
	gluCylinder(uc8, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc8);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.15f, -0.07f, 0.0f);
	glPushMatrix();
	if (LHLMoveUP) {
		if (currentLHLRotateUP < targetLHLRotateUP) {
			currentLHLRotateUP += 1.0f;
		}
		else {
			LHLMoveUP = false;
		}
	}
	else {
		if (currentLHLRotateUP > targetLHLRotateUP) {
			currentLHLRotateUP -= 1.0f;
		}
		else {
			LHLMoveUP = true;
		}
	}
	glRotatef(currentLHLRotateUP, -1.0f, 1.0f, 0.0f);

	leftlowerArm(size*0.24f);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void rightArm(float size) {
	//total 10 polygon
	glPushMatrix();
	glScalef(size, size, size);
	righthandArmor(1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	if (RHRotateUP) {
		if (currentRHRotateUD > targetRHRotateUD) {
			currentRHRotateUD -= 1.0f;
		}
		else {
			RHRotateUP = false;
		}
	}
	else {
		if (currentRHRotateUD < targetRHRotateUD) {
			currentRHRotateUD += 1.0f;
		}
		else {
			RHRotateUP = true;
		}
	}
	if (RHRotateFront) {
		if (currentRHRotateFB > targetRHRotateFB) {
			currentRHRotateFB -= 1.0f;
		}
		else {
			RHRotateFront = false;
		}
	}
	else {
		if (currentRHRotateFB < targetRHRotateFB) {
			currentRHRotateFB += 1.0f;
		}
		else {
			RHRotateFront = true;
		}
	}
	glRotatef(currentRHRotateUD, 1.0f, 0.0f, 0.0f);
	glRotatef(currentRHRotateFB, 0.0f, 1.0f, 0.0f);
	glColor3f(0.5137f, 0.6667f, 0.9333f);
	GLUquadricObj *up1 = NULL;
	up1 = gluNewQuadric();
	gluQuadricDrawStyle(up1, GLU_FILL);                   //1
	gluSphere(up1, 0.30, 60, 60);
	gluDeleteQuadric(up1);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.29f);
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	GLUquadricObj *uc1 = NULL;
	uc1 = gluNewQuadric();
	gluQuadricDrawStyle(uc1, GLU_FILL);                   //2
	gluCylinder(uc1, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc1);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc2 = NULL;
	uc2 = gluNewQuadric();
	gluQuadricDrawStyle(uc2, GLU_FILL);                   //3
	gluCylinder(uc2, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc2);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc3 = NULL;
	uc3 = gluNewQuadric();
	gluQuadricDrawStyle(uc3, GLU_FILL);                   //4
	gluCylinder(uc3, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc3);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc4 = NULL;
	uc4 = gluNewQuadric();
	gluQuadricDrawStyle(uc4, GLU_FILL);                   //5
	gluCylinder(uc4, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc4);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc5 = NULL;
	uc5 = gluNewQuadric();
	gluQuadricDrawStyle(uc5, GLU_FILL);                   //6
	gluCylinder(uc5, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc5);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc6 = NULL;
	uc6 = gluNewQuadric();
	gluQuadricDrawStyle(uc6, GLU_FILL);                   //7
	gluCylinder(uc6, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc6);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc7 = NULL;
	uc7 = gluNewQuadric();
	gluQuadricDrawStyle(uc7, GLU_FILL);                   //9
	gluCylinder(uc7, 0.05, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(uc7);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *uc8 = NULL;
	uc8 = gluNewQuadric();
	gluQuadricDrawStyle(uc8, GLU_FILL);                   //10
	gluCylinder(uc8, 0.1, 0.05, 0.1, 40, 40);
	gluDeleteQuadric(uc8);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(-1.15f, -0.07f, 0.0f);
	glPushMatrix();
	if (RHLMoveUP) {
		if (currentRHLRotateUP > targetRHLRotateUP) {
			currentRHLRotateUP -= 1.0f;
		}
		else {
			RHLMoveUP = false;
		}
	}
	else {
		if (currentRHLRotateUP < targetRHLRotateUP) {
			currentRHLRotateUP += 1.0f;
		}
		else {
			RHLMoveUP = true;
		}
	}
	glRotatef(currentRHLRotateUP, 1.0f, 1.0f, 0.0f);
	rightlowerArm(0.24f);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void eyes(float size) {
	//total 3 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glColor3f(1.0f, 1.0f, 1.0f);
	GLUquadricObj *e1 = NULL;
	e1 = gluNewQuadric();
	gluQuadricDrawStyle(e1, GLU_FILL);
	gluSphere(e1, 1.0, 60, 60);
	gluDeleteQuadric(e1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.1f);
	glColor3f(0.1882f, 0.7254f, 0.6470f);
	GLUquadricObj *e2 = NULL;
	e2 = gluNewQuadric();
	gluQuadricDrawStyle(e2, GLU_FILL);
	gluSphere(e2, 0.95, 60, 60);
	gluDeleteQuadric(e2);

	glTranslatef(0.0f, 0.0f, 0.06f);
	glColor3f(0.1764f, 0.1764f, 0.1764f);
	GLUquadricObj *e3 = NULL;
	e3 = gluNewQuadric();
	gluQuadricDrawStyle(e3, GLU_FILL);
	gluSphere(e3, 0.9, 60, 60);
	gluDeleteQuadric(e3);
	glPopMatrix();
	glPopMatrix();
}
void head(float size) {
	//total 28 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	GLUquadricObj *h1 = NULL;
	h1 = gluNewQuadric();
	gluQuadricDrawStyle(h1, GLU_FILL);                   //1
	gluSphere(h1, 0.3, 60, 60);
	gluDeleteQuadric(h1);
	glTranslatef(0.0f, 0.0f, 0.05f);
	GLUquadricObj *h2 = NULL;
	h2 = gluNewQuadric();
	gluQuadricDrawStyle(h2, GLU_FILL);                   //2
	gluSphere(h2, 0.3, 60, 60);
	gluDeleteQuadric(h2);
	glTranslatef(0.0f, 0.0f, 0.05f);
	GLUquadricObj *h3 = NULL;
	h3 = gluNewQuadric();
	gluQuadricDrawStyle(h3, GLU_FILL);                   //3
	gluSphere(h3, 0.3, 60, 60);
	gluDeleteQuadric(h3);
	glPopMatrix();

	//-----------------------eyes
	glPushMatrix();
	glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.32f);
	eyes(0.08f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.32f);
	eyes(0.08f);
	glPopMatrix();

	glBegin(GL_TRIANGLES);                   //4
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.25f, 0.30f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.55f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //5
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.25f, 0.30f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.55f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //6
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.1f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.05f, 0.30f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.55f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //7
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.3f, 0.1f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.05f, 0.30f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.55f);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0f, -0.15f, -0.02f);
	glBegin(GL_TRIANGLES);                   //8
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.25f, 0.30f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //9
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.25f, 0.30f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.25f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //10
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.3f, 0.1f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.1f, 0.30f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();
	glBegin(GL_TRIANGLES);                   //11
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.3f, 0.1f, 0.25f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.1f, 0.30f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	for (float i = 0.0f; i < 2.1f; i += 0.02f) {
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);                   //12
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.3f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -0.3f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.05f, -0.3f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.05f, -0.0f, -0.3f);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	for (float i = 0.0f; i > -2.1f; i -= 0.02f) {
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);                   //13
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.3f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -0.3f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.05f, -0.3f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.05f, -0.0f, -0.3f);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	for (float i = 0.0f; i < 2.2f; i += 0.02f) {
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);                   //14
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.3f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.05f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.05f, 0.0f, -0.3f);
		glEnd();

		glBegin(GL_QUADS);                   //15
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.23f, -0.20f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.05f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.05f, 0.23f, -0.20f);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	for (float i = 0.0f; i > -2.2f; i -= 0.02f) {
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);                   //16
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.3f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.05f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.05f, 0.0f, -0.3f);
		glEnd();

		glBegin(GL_QUADS);                   //17
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.23f, -0.2f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.05f, 0.15f, -0.32f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-0.05f, 0.23f, -0.2f);
		glEnd();
	}
	glPopMatrix();

	//mouth
	glPushMatrix();
	glTranslatef(0.0f, -0.15f, 0.0f);
	glBegin(GL_QUADS);                   //18
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.20f, 0.0f, 0.29f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.15f, -0.15f, 0.30f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, -0.3f, 0.5f);
	glEnd();

	glBegin(GL_QUADS);                   //19
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.4f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.29f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.15f, -0.15f, 0.30f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, -0.3f, 0.5f);
	glEnd();
	glPopMatrix();

	//horns
	glPushMatrix();
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.2f, 0.15f);
	glBegin(GL_TRIANGLES);                   //20
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 1.0f, 0.05f);

	glVertex3f(0.05f, 0.0f, 0.0f);                   //21
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.7f, 0.05f);

	glVertex3f(-0.05f, 0.0f, 0.0f);                   //22
	glVertex3f(0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.7f, 0.05f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.10f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);                   //23
	glColor3f(0.9725f, 0.7647f, 0.3412f);
	glVertex3f(-0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.7f, 0.05f);

	glVertex3f(0.05f, 0.0f, 0.0f);                   //24
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.7f, 0.05f);

	glVertex3f(-0.05f, 0.0f, 0.0f);                   //25
	glVertex3f(0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.7f, 0.05f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.10f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);                   //26
	glColor3f(0.9725f, 0.7647f, 0.3412f);
	glVertex3f(-0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.7f, 0.05f);

	glVertex3f(0.05f, 0.0f, 0.0f);                   //27
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.7f, 0.05f);

	glVertex3f(-0.05f, 0.0f, 0.0f);                   //28
	glVertex3f(0.05f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.7f, 0.05f);
	glEnd();
	glPopMatrix();



	glPopMatrix();
}
void body(float size) {
	//total 26 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.25f);
	GLUquadricObj *b1 = NULL;
	b1 = gluNewQuadric();
	gluQuadricDrawStyle(b1, GLU_FILL);                   //1
	gluQuadricTexture(b1, true);
	gluCylinder(b1, 0.4, 0.15, 0.45, 60, 60);
	gluDeleteQuadric(b1);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.25f);
	GLUquadricObj *b2 = NULL;
	b2 = gluNewQuadric();
	gluQuadricDrawStyle(b2, GLU_FILL);                   //2
	gluQuadricTexture(b2, true);
	gluCylinder(b2, 0.4, 0.15, 0.45, 60, 60);
	gluDeleteQuadric(b2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *b3 = NULL;
	b3 = gluNewQuadric();
	gluQuadricDrawStyle(b3, GLU_FILL);                   //3
	gluQuadricTexture(b3, true);
	gluCylinder(b3, 0.35, 0.4, 0.15, 60, 60);
	gluDeleteQuadric(b3);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *b4 = NULL;
	b4 = gluNewQuadric();
	gluQuadricDrawStyle(b4, GLU_FILL);                   //4
	gluQuadricTexture(b4, true);
	gluCylinder(b4, 0.35, 0.4, 0.15, 60, 60);
	gluDeleteQuadric(b4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	GLUquadricObj *b5 = NULL;
	b5 = gluNewQuadric();
	gluQuadricDrawStyle(b5, GLU_FILL);                   //5
	gluQuadricTexture(b5, true);
	gluCylinder(b5, 0.35, 0.25, 0.6, 60, 60);
	gluDeleteQuadric(b5);
	glPopMatrix();

	glPushMatrix();
	for (float i = 0.0f; i < 1.2f; i += 0.02f) {
		glColor3f(0.9f, 0.9f, 0.9f);
		glRotatef(i, 1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);                   //6
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.02f, 0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, 0.45f);
		glEnd();
		glBegin(GL_QUADS);                   //7
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.02f, 0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, 0.45f);
		glEnd();

		glBegin(GL_QUADS);                   //8
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.02f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, -0.45f);
		glEnd();
		glBegin(GL_QUADS);                   //9
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.02f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, -0.45f);
		glEnd();

		//cover
		glBegin(GL_QUADS);                   //10
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();

		glBegin(GL_QUADS);                   //11
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	for (float i = 0.0f; i > -1.3f; i -= 0.02f) {
		glColor3f(0.9f, 0.9f, 0.9f);
		glRotatef(i, 1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);                   //12
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.02f, 0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, 0.45f);
		glEnd();
		glBegin(GL_QUADS);                   //13
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.02f, 0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, 0.45f);
		glEnd();

		glBegin(GL_QUADS);                   //14
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.02f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, -0.45f);
		glEnd();
		glBegin(GL_QUADS);                   //15
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.02f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.02f, -0.45f);
		glEnd();

		//cover
		glBegin(GL_QUADS);                   //16
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();

		glBegin(GL_QUADS);                   //17
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.45f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, 0.4f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-0.25f, 0.0f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();
	}
	glPopMatrix();

	glBegin(GL_QUADS);                   //18
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.40f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.15f, -0.8f, 0.35f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.25f, 0.0f, 0.4f);
	glEnd();

	glBegin(GL_QUADS);                   //19
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.45f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.4f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.15f, -0.8f, 0.35f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.25f, 0.0f, 0.4f);
	glEnd();


	//neck
	glPushMatrix();
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
	GLUquadricObj *neck = NULL;
	neck = gluNewQuadric();
	gluQuadricDrawStyle(neck, GLU_FILL);                   //20
	gluCylinder(neck, 0.3, 0.15, 0.5, 60, 60);
	gluDeleteQuadric(neck);
	glPopMatrix();

	//connect to lower body
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.50f);
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	GLUquadricObj *lb1 = NULL;
	lb1 = gluNewQuadric();
	gluQuadricDrawStyle(lb1, GLU_FILL);                   //21
	gluCylinder(lb1, 0.1, 0.2, 0.1, 40, 40);
	gluDeleteQuadric(lb1);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *lb2 = NULL;
	lb2 = gluNewQuadric();
	gluQuadricDrawStyle(lb2, GLU_FILL);                   //22
	gluCylinder(lb2, 0.2, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(lb2);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *lb3 = NULL;
	lb3 = gluNewQuadric();
	gluQuadricDrawStyle(lb3, GLU_FILL);                   //23
	gluCylinder(lb3, 0.1, 0.15, 0.1, 40, 40);
	gluDeleteQuadric(lb3);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *lb4 = NULL;
	lb4 = gluNewQuadric();
	gluQuadricDrawStyle(lb4, GLU_FILL);                   //24
	gluCylinder(lb4, 0.15, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(lb4);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *lb5 = NULL;
	lb5 = gluNewQuadric();
	gluQuadricDrawStyle(lb5, GLU_FILL);                   //25
	gluCylinder(lb5, 0.1, 0.2, 0.1, 40, 40);
	gluDeleteQuadric(lb5);

	glTranslatef(0.0f, 0.0f, 0.1f);
	GLUquadricObj *lb6 = NULL;
	lb6 = gluNewQuadric();
	gluQuadricDrawStyle(lb6, GLU_FILL);                   //26
	gluCylinder(lb6, 0.2, 0.1, 0.1, 40, 40);
	gluDeleteQuadric(lb6);


	glPopMatrix();


	glPopMatrix();
}
void foot(float size) {
	//total 15 polygon
	glPushMatrix();
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glScalef(size, size, size);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);                   //1
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0.22f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, 0.22f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);                   //2
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.22f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.22f, 0.5f);

	glTexCoord2f(0.0f, 1.0f);                   //3
	glVertex3f(-0.2f, 0.22f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.3f, 0.22f, 0.5f);

	glTexCoord2f(0.0f, 1.0f);                   //4
	glVertex3f(0.2f, 0.22f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.3f, 0.22f, 0.5f);

	glTexCoord2f(0.0f, 1.0f);                   //5
	glVertex3f(-0.2f, 0.22f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 0.22f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.22f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 0.22f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);                   //6
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);                   //7
	glVertex3f(-0.2f, 0.0f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0.22f, -0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.2f, 0.22f, -0.1f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //8
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -0.6f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, 0.22f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //9
	glVertex3f(0.0f, 0.0f, -0.6f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0.22f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //10
	glVertex3f(0.0f, 0.0f, -0.6f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //11
	glVertex3f(0.0f, 0.0f, -0.6f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, 0.22f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, 0.22f, -0.1f);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0f, 0.25f, 0.0f);
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);                   //12
	gluSphere(sphere, 0.2, 60, 60);
	gluDeleteQuadric(sphere);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.1f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(1.0f, 0.8941f, 0.4f);
	GLUquadricObj *toe1 = NULL;
	toe1 = gluNewQuadric();
	gluQuadricDrawStyle(toe1, GLU_FILL);                   //13
	gluCylinder(toe1, 0.1, 0.0, 0.25, 40, 40);
	gluDeleteQuadric(toe1);

	glTranslatef(0.2f, 0.0f, 0.0f);
	GLUquadricObj *toe2 = NULL;
	toe2 = gluNewQuadric();
	gluQuadricDrawStyle(toe2, GLU_FILL);                   //14
	gluCylinder(toe2, 0.1, 0.0, 0.25, 40, 40);
	gluDeleteQuadric(toe2);

	glTranslatef(-0.4f, 0.0f, 0.0f);
	GLUquadricObj *toe3 = NULL;
	toe3 = gluNewQuadric();
	gluQuadricDrawStyle(toe3, GLU_FILL);                   //15
	gluCylinder(toe3, 0.1, 0.0, 0.25, 40, 40);
	gluDeleteQuadric(toe3);
	glPopMatrix();

	glPopMatrix();
}
void lowerleg(float size) {
	//total 10 polygon
	glPushMatrix();
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glScalef(size, size, size);

	glPushMatrix();
	glColor3f(0.4196f, 0.4196f, 0.4196f);
	GLUquadricObj *ll1 = NULL;
	ll1 = gluNewQuadric();
	gluQuadricDrawStyle(ll1, GLU_FILL);                   //1
	gluSphere(ll1, 0.37, 60, 60);
	gluDeleteQuadric(ll1);
	glPopMatrix();

	glBegin(GL_QUADS);                   //2
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -2.0f, 0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -1.9f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //3
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -2.0f, 0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, -1.9f, 0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //4
	glVertex3f(0.0f, 0.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -2.0f, -0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -1.9f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //5
	glVertex3f(0.0f, 0.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -2.0f, -0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, -1.9f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //6
	glVertex3f(0.5f, 0.0f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, -1.9f, 0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -1.9f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //7
	glVertex3f(-0.5f, 0.0f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.2f, -1.9f, 0.2f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.2f, -1.9f, -0.2f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //8
	glVertex3f(-0.5f, 0.0f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, -0.1f);
	glEnd();




	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_TRIANGLES);                   //9
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(5.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, 0.4f);

	glTexCoord2f(1.0f, 0.0f);                   //10
	glVertex3f(-0.5f, 0.0f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(5.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, 0.4f);
	glEnd();


	glPopMatrix();
}
void upperleg(float size) {
	//total 14 polygon
	glPushMatrix();
	glScalef(size, size, size);
	glBegin(GL_QUADS);                   //1
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, -0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, -1.5f, 0.15f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -1.5f, 0.3f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.0f, 1.0f);                   //2
	glVertex3f(0.0f, -0.5f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, -1.5f, -0.15f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -1.5f, -0.3f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glTexCoord2f(0.0f, 1.0f);                   //3
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -1.5f, 0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.8f, -1.5f, 0.15f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.9f, -0.5f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //4
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -1.5f, -0.3f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.8f, -1.5f, -0.15f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.9f, -0.5f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //5
	glVertex3f(0.0f, -0.5f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.2f, -1.5f, -0.15f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.2f, -1.5f, 0.15f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, -0.5f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //6
	glVertex3f(0.9f, -0.5f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.8f, -1.5f, -0.15f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.8f, -1.5f, 0.15f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.9f, -0.5f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //7
	glVertex3f(0.5f, 0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.5f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //8
	glVertex3f(0.5f, 0.5f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.9f, -0.5f, -0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.5f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //9
	glVertex3f(0.5f, 0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 0.5f, -0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.5f, 0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //10
	glVertex3f(1.0f, 0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.9f, -0.5f, -0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.5f, -0.1f);

	glTexCoord2f(0.0f, 1.0f);                   //11
	glVertex3f(0.5f, 0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.1f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, -0.1f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, -0.1f);
	glEnd();

	glBegin(GL_TRIANGLES);                   //12
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.1f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, -0.5f, 0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glTexCoord2f(1.0f, 0.0f);                   //13
	glVertex3f(0.5f, 0.5f, -0.1f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, -0.5f, -0.1f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);

	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(0.4196f, 0.4196f, 0.4196f);

	GLUquadricObj *lcylinder = NULL;
	lcylinder = gluNewQuadric();
	gluQuadricDrawStyle(lcylinder, GLU_FILL);                   //14
	gluCylinder(lcylinder, 0.2, 0.05, 0.5, 60, 60);
	gluDeleteQuadric(lcylinder);
	glPopMatrix();
	glPopMatrix();
}
void leftLeg(float size) {
	GLuint textures[4];
	glPushMatrix();
	glScalef(1.0f, 1.2f, 1.0f);
	glTranslatef(0.2f*size, 0.0f, 0.0f);

	glPushMatrix();
	if (walkFront) {
		if (rightlegMovementDone) {
			if (LfirstMovement == false) {
				if (LupperlegRotate < 30.0f) {
					LupperlegRotate += 1.0f;
					LlowerlegRotate -= 1.0f;
				}
				else {
					LfirstMovement = true;
				}
			}
			else if (LfirstMovement == true) {
				if (LupperlegRotate > 0.0f) {
					LupperlegRotate -= 1.0f;
					LlowerlegRotate += 1.0f;
				}
				else {
					leftlegMovementDone = true;
					targetLlegTranslate = targetallTranslate;
				}

			}
		}


	}
	glTranslatef(0.0f, 0.0f, targetLlegTranslate);
	glRotatef(LupperlegRotate, 1.0f, 0.0f, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}

	glPushMatrix();
	glTranslatef(0.5f*size, -1.7f*size, 0.0f);
	glRotatef(LlowerlegRotate, 1.0f, 0.0f, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}

	glTranslatef(0.0f, -2.2f*size, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		foot(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		foot(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		foot(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		foot(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void rightLeg(float size) {
	GLuint textures[4];
	glPushMatrix();
	glScalef(1.0f, 1.2f, 1.0f);
	glTranslatef(-0.2f*size, 0.0f, 0.0f);

	glPushMatrix();
	if (walkFront) {
		if (allMovement) {
			if (RfirstMovement == false) {
				if (RupperlegRotate < 30.0f) {
					RupperlegRotate += 1.0f;
					RlowerlegRotate -= 1.0f;
				}
				else {
					RfirstMovement = true;
				}
			}
			else if (RfirstMovement == true) {
				if (RupperlegRotate > 0.0f) {
					RupperlegRotate -= 1.0f;
					RlowerlegRotate += 1.0f;
				}
				else {
					rightlegMovementDone = true;
					targetRlegTranslate = targetallTranslate;
				}

			}
		}


	}
	glTranslatef(0.0f, 0.0f, targetRlegTranslate);
	glRotatef(RupperlegRotate, 1.0f, 0.0f, 0.0f);


	glPushMatrix();
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		upperleg(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5f*size, -1.7f*size, 0.0f);
	glRotatef(RlowerlegRotate, 1.0f, 0.0f, 0.0f);

	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		lowerleg(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}
	glTranslatef(0.0f, -2.2f*size, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		foot(size);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		foot(size);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		foot(size);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		foot(size);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void combine(float size) {
	GLuint textures[4];
	glPushMatrix();
	glScalef(size, size, size);

	glTranslatef(0.0f, 1.5f, 0.0f);
	glPushMatrix();
	if (walkFront) {
		if (rightlegMovementDone) {
			if (leftlegMovementDone) {
				if (currentallTranslate > targetallTranslate) {
					currentallTranslate -= 0.05f;
				}
			}
			else if (currentallTranslate > targetallTranslate + 0.1f) {
				currentallTranslate -= 0.05f;
			}

		}
		else {
			rightlegMovementDone = false;
			leftlegMovementDone = false;
		}


	}
	else {
		if (currentallTranslate < targetallTranslate) {
			currentallTranslate += 0.01f;
		}
	}
	glTranslatef(0.0f, 0.0f, -currentallTranslate);
	glPushMatrix();
	if (currentHeadRotate > targetHeadRotate) {
		currentHeadRotate -= 0.5f;
	}
	else if (currentHeadRotate < targetHeadRotate) {
		currentHeadRotate += 0.5f;
	}
	glRotatef(currentHeadRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.6f, 0.0f);
	head(0.7f);
	glPopMatrix();
	body(1.0f);
	glPushMatrix();

	glTranslatef(0.95f, 0.0f, 0.0f);
	leftArm(1.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.95f, 0.0f, 0.0f);
	rightArm(1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.5f, 0.0f);
	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		lowerbody(0.55f);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		lowerbody(0.55f);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		lowerbody(0.55f);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		lowerbody(0.55f);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}

	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, -1.35f, 0.0f);
	leftLeg(0.5f);
	rightLeg(0.5f);
	glPopMatrix();



	glPopMatrix();
}
void world() {

	glColor3f(1.0f, 1.0f, 1.0f);
	GLUquadricObj *world = NULL;
	world = gluNewQuadric();
	gluQuadricDrawStyle(world, GLU_FILL);
	gluQuadricTexture(world, true);
	gluSphere(world, 10.0, 200, 200);
	gluDeleteQuadric(world);
}
void projection() {
	GLuint textures;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(tx, 0.0f, 0.0f);

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, -2.0, 20.0);
		//only rotate the view in ortho
		glRotatef(ry, 0.0f, 1.0f, 0.0f);

		textures = loadTexture("World.bmp");
		world();
		glDeleteTextures(1, &textures);

	}
	else {

		gluPerspective(5.0, 1.0, -1.0, 20.0);
		glFrustum(-10, 10, -10, 10, 1.0, 41.0);
		glDisable(GL_DEPTH_TEST);


		textures = loadTexture("World.bmp");
		world();
		glDeleteTextures(1, &textures);


		glEnable(GL_DEPTH_TEST);
	}
}
void display() {
	GLuint textures[5];


	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting();
	projection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glTranslatef(0.0f, 0.0f, tz);
	if (!isOrtho) {
		glRotatef(ry, 0.0f, 1.0f, 0.0f);
	}

	switch (textureChoice) {
	case 1:
		textures[0] = loadTexture("armor1.bmp");
		combine(1.0f);
		glDeleteTextures(1, &textures[0]);
		break;
	case 2:
		textures[1] = loadTexture("armor2.bmp");
		combine(1.0f);
		glDeleteTextures(1, &textures[1]);
		break;
	case 3:
		textures[2] = loadTexture("armor3.bmp");
		combine(1.0f);
		glDeleteTextures(1, &textures[2]);
		break;
	case 4:
		textures[3] = loadTexture("armor4.bmp");
		combine(1.0f);
		glDeleteTextures(1, &textures[3]);
		break;
	default:
		break;
	}


	glDisable(GL_TEXTURE_2D);

}

//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------