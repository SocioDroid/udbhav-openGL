/* Header Files */
#pragma once
#include <windows.h>
#include "OGLMain.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include "./includes/OpenAL.h"

// ======================================= SCENES/
#include "./scenes/MainScene/MainScene.h"
// ======================================= SCENES END

// ======================================= CAMERA
#include "./utils/camera/Camera.h"
#include "./utils/camera/BezierCamera.h"
// ======================================= CAMERA END

#include "./utils/common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./includes/stb_image.h"
#include "./includes/helper_timer.h"

#define WINWIDTH 800
#define WINHEIGHT 600

GLuint giWindowWidth = 0;
GLuint giWindowHeight = 0;

vec3 eye = vec3(0.0f, 0.0f, 6.0f);
vec3 center = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

mat4 perspectiveProjectionMatrix;

mat4 modelMatrix = mat4::identity();
mat4 viewMatrix = mat4::identity();

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;

// DOF
extern float focalDistance;
extern float focalDepth;

BOOL isDOFTrue = FALSE;
StopWatchInterface *timer = NULL;
// ================================= Camera
Camera camera;
BezierCamera *globalBezierCamera;

// ================================= Camera End

// ================================= HELPING VARIABLES
CommonShaders *commonShaders;
CommonModels *commonModels;

// Object
float objX = 0.0f;
float objY = 0.0f;
float objZ = 0.0f;
float objIncrement = 70.0f;

// Scale
float scaleX = 0.0;
float scaleY = 0.0;
float scaleZ = 0.0;
float scaleIncrement = 2.0f;

float globalTime = 0.0f;
float ELAPSED_TIME;

float light_objX = 0.0f;
float light_objY = 0.0f;
float light_objZ = 0.0f;
float lightObjIncrement = 0.1f;

float objAngle = 0.0f;
float objAngleIncrement = 1.0f;

// =============================== GLOBAL CONTROLS
int PHASE_CURRENT = PHASE_MAIN;
BOOL USE_FPV_CAM = FALSE;
BOOL playMusic = TRUE;
BOOL enableBezierCameraControl = TRUE;
BOOL spaceBarIsPressed = FALSE;
float VOLUME_LEVEL = 0.1f;
// ==============================================//

BOOL start_fade_out_opening = FALSE;

std::vector<std::vector<float>> bezierPoints = {
	{-1400.000000f, 2018.000000f, 14210.000000f},
	{-1050.000000f, 2018.000000f, 11550.000000f},
	{-630.000000f, 2018.000000f, 5600.000000f},
	{-2590.000000f, 2018.000000f, 2870.000000f},
	{-3010.000000f, 2578.000000f, -210.000000f},
	{-2730.000000f, 3488.000000f, -4060.000000f},
	{-2940.000000f, 4468.000000f, -5880.000000f},
	{-1680.000000f, 4538.000000f, -7560.000000f},
	{-350.000000f, 4538.000000f, -10570.000000f},
	{1190.000000f, 4958.000000f, -11340.000000f},
	{2590.000000f, 5448.000000f, -14910.000000f},
	{350.000000f, 5448.000000f, -15890.000000f},
	{-1540.000000f, 5448.000000f, -17360.000000f},
	{-5950.000000f, 5448.000000f, -25270.000000f},
	{-10290.000000f, 5448.000000f, -30030.000000f},
	{-6930.000000f, 5448.000000f, -33810.000000f},
	{7280.000000f, 5588.000000f, -25270.000000f},
	{9170.000000f, 5588.000000f, -25270.000000f},
	{11410.000000f, 5588.000000f, -26180.000000f},
	{12460.000000f, 5448.000000f, -26180.000000f},
	{18410.000000f, 5098.000000f, -26390.000000f},
	{20160.000000f, 5098.000000f, -26390.000000f},
	{24500.000000f, 4958.000000f, -26460.000000f},
	{28070.000000f, 4328.000000f, -26460.000000f},
	{30030.000000f, 2648.000000f, -26460.000000f},
	{35700.000000f, 2158.000000f, -26460.000000f},
	{35420.000000f, 2158.000000f, -24500.000000f},
	{35420.000000f, 2158.000000f, -19390.000000f},
	{35630.000000f, 2158.000000f, -18340.000000f},
	{35840.000000f, 1458.000000f, -15610.000000f},
	{34720.000000f, 1318.000000f, -14700.000000f},
	{30730.000000f, 1318.000000f, -13440.000000f},
	{28210.000000f, 1318.000000f, -13370.000000f},
	{27300.000000f, 1318.000000f, -13370.000000f},
	{26040.000000f, 1038.000000f, -13370.000000f},
	{24290.000000f, 1038.000000f, -13370.000000f},
	{21070.000000f, 828.000000f, -13370.000000f},
	{20930.000000f, 828.000000f, -13370.000000f},
	{20860.000000f, 828.000000f, -13370.000000f},
	{20790.000000f, 828.000000f, -13370.000000f},
	{20790.000000f, 828.000000f, -13370.000000f},
};

// YAW GLOBAL
std::vector<float> yawGlobal = {
	-90.000000f,
	-90.000000f,
	-90.000000f,
	-82.000000f,
	-82.000000f,
	-72.000000f,
	-64.000000f,
	-60.000000f,
	-58.000000f,
	-54.000000f,
	-52.000000f,
	-42.000000f,
	-34.000000f,
	0.000000f,
	24.000000f,
	54.000000f,
	76.000000f,
	94.000000f,
	94.000000f,
	94.000000f,
	94.000000f,
	106.000000f,
	106.000000f,
	106.000000f,
	106.000000f,
	122.000000f,
	126.000000f,
	128.000000f,
	158.000000f,
	170.000000f,
	170.000000f,
	174.000000f,
	174.000000f,
	170.000000f,
	160.000000f,
	140.000000f,
	130.000000f,
	130.000000f,
	130.000000f,
	130.000000f,
	130.000000f,
};

// PITCH GLOBAL
std::vector<float> pitchGlobal = {
	-4.000000f,
	-4.000000f,
	-2.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	-4.000000f,
	-4.000000f,
	52.000000f,
	-4.000000f,
	-4.000000f,
	-12.000000f,
	-12.000000f,
	-14.000000f,
	-14.000000f,
	-14.000000f,
	-14.000000f,
	-12.000000f,
	-12.000000f,
	-12.000000f,
	-12.000000f,
	-6.000000f,
	-4.000000f,
	-6.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	4.000000f,
	4.000000f,
	4.000000f,
	4.000000f,
	4.000000f,
};

// FOV GLOBAL
std::vector<float> fovGlobal = {
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
	0.000000f,
};

int vectorIndex = bezierPoints.size() - 1;

// ================================= HELPING VARIABLES END
MainScene *mainScene;

// ======================================= OBJECTS
OpenAL myMusic;
float globalSpeedAdjust = 0.0f;
/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	/* variable declarations */
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	int iHeightOfWindow, iWidthOfWindow;

	// Code
	CreateLogFile();

	// Initializaion of wndclassex structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	/* Register Above wndclass */
	RegisterClassEx(&wndclass);

	iHeightOfWindow = GetSystemMetrics(SM_CYSCREEN); // Height of Window Screen
	iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN);	 // Width Of Window Screen

	/* Create Window */
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
						  TEXT("RTR5 - Aishwarya Kendle | GLEA"),
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						  (iWidthOfWindow - WINWIDTH) / 2,
						  (iHeightOfWindow - WINHEIGHT) / 2,
						  WINWIDTH,
						  WINHEIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);
	ghwnd = hwnd;

	// initizalize
	iRetVal = initialize();

	if (iRetVal == -1)
	{
		PrintLog("Choose Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		PrintLog("Set Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		PrintLog("Crete OpenGl Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -4)
	{
		PrintLog("Makeing OpenGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		PrintLog("GLEW Initialization Failed...\n");
		uninitialize();
	}
	else
	{
		PrintLog("Initialize Successful...\n");
	}

	ShowWindow(hwnd, iCmdShow);

	/* fore grounding and focusing window */
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	/* Game Loop */
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				/* Render the seen */
				display();
				// updatetheseen
				update();
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);
}

/* CALLBACK Function */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	/* fucntion declarations */

	// void ToggleFullScreen();
	void resize(int, int);

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		if (enableBezierCameraControl)
		{
			objX = bezierPoints[bezierPoints.size() - 1][0];
			objY = bezierPoints[bezierPoints.size() - 1][1];
			objZ = bezierPoints[bezierPoints.size() - 1][2];
			scaleX = yawGlobal[yawGlobal.size() - 1];
			scaleY = pitchGlobal[pitchGlobal.size() - 1];
			scaleZ = fovGlobal[fovGlobal.size() - 1];
		}
		break;
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_ERASEBKGND:
		return (0);

	case WM_CHAR:
		camera.keyboardInputs(wParam);
		switch (wParam)
		{
		case 'f':
		case 'F':
			ToggleFullScreen();
			break;
		case '+':
			scaleZ += scaleIncrement * 20.0f;
			// scaleZ += scaleIncrement;
			break;
		case '-':
			scaleZ -= scaleIncrement * 20.0f;
			// scaleZ -= scaleIncrement;
			break;
		case 'C':
		case 'c':
			USE_FPV_CAM = !USE_FPV_CAM;
			break;
		case 'p':
		case 'P':
			if (enableBezierCameraControl)
			{
				bezierPoints.push_back({objX, objY, objZ});
				yawGlobal.push_back(scaleX);
				pitchGlobal.push_back(scaleY);
				fovGlobal.push_back(scaleZ);
				vectorIndex = bezierPoints.size() - 1;
			}
			break;

		case 'G':
		case 'g':
			if (enableBezierCameraControl)
			{
				if (bezierPoints.size() > vectorIndex + 1)
				{
					vectorIndex++;
					objX = bezierPoints[vectorIndex][0];
					objY = bezierPoints[vectorIndex][1];
					objZ = bezierPoints[vectorIndex][2];
					scaleX = yawGlobal[vectorIndex];
					scaleY = pitchGlobal[vectorIndex];
					scaleZ = fovGlobal[vectorIndex];
				}
			}
			break;

		case 'B':
		case 'b':
			if (enableBezierCameraControl)
			{
				if (vectorIndex > 0)
				{
					vectorIndex--;
					objX = bezierPoints[vectorIndex][0];
					objY = bezierPoints[vectorIndex][1];
					objZ = bezierPoints[vectorIndex][2];
					scaleX = yawGlobal[vectorIndex];
					scaleY = pitchGlobal[vectorIndex];
					scaleZ = fovGlobal[vectorIndex];
				}
			}
			break;
		case 'X':
			light_objX = light_objX + lightObjIncrement;
			break;
		case 'x':
			light_objX = light_objX - lightObjIncrement;
			break;
		case 'Y':
			light_objY = light_objY + lightObjIncrement;
			break;
		case 'y':
			light_objY = light_objY - lightObjIncrement;
			break;
		case 'Z':
			light_objZ = light_objZ + lightObjIncrement;
			break;
		case 'z':
			light_objZ = light_objZ - lightObjIncrement;
			break;
		case 'r':
		case 'R':
			globalTime = 0.0f;
			break;
		case 'q':
		case 'Q':
			if (objIncrement == 1.0f)
				objIncrement = 0.1f;
			else
				objIncrement = 1.0f;

			if (scaleIncrement == 1.0f)
				scaleIncrement = 0.1f;
			else
				scaleIncrement = 1.0f;
			break;

		case 'i':
		case 'I':
			globalSpeedAdjust -= 0.0001f;
			break;
		case 'k':
		case 'K':
			globalSpeedAdjust += 0.0001f;
			break;

		case 27:
			PrintLog("\n\nVariables  | OBJ %ff, %ff, %ff | OBJ Angle %ff | SCALE %ff, %ff, %ff\n\n\n\n", objX, objY, objZ, objAngle, scaleX, scaleY, scaleZ);
			PrintLog("\nlight_objX : %f , light_objY : %f , light_objZ : %f\n ", light_objX, light_objY, light_objZ);

			if (enableBezierCameraControl)
			{
				// Required  for camera
				PrintLog("std::vector<std::vector<float>> bezierPoints = {\n");
				for (int i = 0; i < bezierPoints.size(); i++)
				{
					PrintLog("{%ff, %ff, %ff},\n", bezierPoints[i][0], bezierPoints[i][1], bezierPoints[i][2]);
				}
				PrintLog("};\n");
				PrintLog("\n\n// YAW GLOBAL\n");
				PrintLog("std::vector<float> yawGlobal = {\n");
				for (int i = 0; i < yawGlobal.size(); i++)
				{
					PrintLog("%ff,\n", yawGlobal[i]);
				}
				PrintLog("};\n");
				PrintLog("\n\n// PITCH GLOBAL\n");
				PrintLog("std::vector<float> pitchGlobal = {\n");
				for (int i = 0; i < pitchGlobal.size(); i++)
				{
					PrintLog("%ff,\n\t", pitchGlobal[i]);
				}
				PrintLog("};\n");
				PrintLog("\n\n// FOV GLOBAL\n");
				PrintLog("std::vector<float> fovGlobal = {\n");
				for (int i = 0; i < fovGlobal.size(); i++)
				{
					PrintLog("%ff,\n\t", fovGlobal[i]);
				}
				PrintLog("};\n");
			}
			PostQuitMessage(0);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			scaleX -= scaleIncrement;
			break;
		case VK_RIGHT:
			scaleX += scaleIncrement;
			break;
		case VK_UP:
			scaleY += scaleIncrement;

			break;
		case VK_DOWN:
			scaleY -= scaleIncrement;

			break;
		case VK_NUMPAD8:
			objY += objIncrement;
			break;
		case VK_NUMPAD5:
			objY -= objIncrement;
			break;
		case VK_NUMPAD4:
			objX -= objIncrement;
			break;
		case VK_NUMPAD6:
			objX += objIncrement;
			break;
		case VK_NUMPAD7:
			objZ -= objIncrement;
			break;
		case VK_NUMPAD1:
			objZ += objIncrement;
			break;

		case VK_NUMPAD9:
			objAngle += objAngleIncrement;
			break;

		case VK_NUMPAD3:
			objAngle -= objAngleIncrement;
			break;
		default:
			break;
		}
		break;
	case WM_SIZE:
		resize(WORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		PrintLog("\n\nVariables  | OBJ %ff, %ff, %ff | SCALE %ff, %ff, %ff\n\n\n\n", objX, objY, objZ, scaleX, scaleY, scaleZ);
		PrintLog("light_objX : %f , light_objY : %f , light_objZ : %f\n ", light_objX, light_objY, light_objZ);

		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
	{
		float mouseX = LOWORD(lParam);
		float mouseY = HIWORD(lParam);

		// Send this to camera class
		camera.mouseInputs(mouseX, mouseY);
	}
	break;

	case WM_MOUSEWHEEL:
	{
		short scrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// Use scrollValue and scrollDelta here
		camera.mouseScroll(scrollDelta);
	}
	break;

	default:
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	// variable declartions
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	//	code
	wp.length = sizeof(WINDOWPLACEMENT);
	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // nccalksize
			}

			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

int initialize(void)
{
	/* fucntion delcations */
	void resize(int, int);
	void uninitialize(void);
	void ToggleFullScreen();
	/* variable declartions */
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	/* code */
	/* initialization of pixelformatdesciptor structure */
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // memset((void*)&pfd , NULL, sizeof(OIXELFORAMTEDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	/* GetDC */
	ghdc = GetDC(ghwnd);

	/* Choose Pixel Format */
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
		return -1;

	/* Set The choosen Puxel Format */
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return -2;

	/* binding API */
	/* Create OpenGL Rendering Context */
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return -3;

	/* make the rendering as current cintext */
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return -4;

	/* Here start OpeGL Code */
	// GLEW INITIALIZATION
	if (glewInit() != GLEW_OK)
		return -5;

	// ======================================================= START INITIALIZATION HERE
	commonShaders = new CommonShaders();
	commonShaders->initialize();

	mainScene = new MainScene();

	commonModels = new CommonModels();
	commonModels->initialize(mainScene->selected_scene, mainScene->START_E2E_DEMO);

	if (!mainScene->initialize())
	{
		PrintLog("Failed to initialize mainScene");
	}

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	// Enabaling the texture
	glEnable(GL_TEXTURE_2D);

	/* Clear the  screen using blue color */
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	// Setting up camera
	float _position[] = {0.0, 0.0, 10.0};
	camera = Camera(WINWIDTH, WINHEIGHT, _position);

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);
	ToggleFullScreen();

	if (playMusic)
	{
		myMusic.InitializeAudio(MAKEINTRESOURCE(MYMUSIC));

		if (mainScene->START_E2E_DEMO == true)
		{
			myMusic.Play();
		}
		else
		{
			switch (mainScene->selected_scene)
			{
			case SCENE_01_EARTH_BIRTH:
				myMusic.playFromSeconds(START_TIME_SCENE_01_01_INTRO);
				break;
			case SCENE_02_EARTH_COOLDOWN:
				myMusic.playFromSeconds(START_TIME_SCENE_02_01_EARTH_COOLDOWN);
				break;
			case SCENE_03_TERRAIN_WITH_HEAVY_RAIN:
				myMusic.playFromSeconds(START_TIME_SCENE_03_01_TERRAIN_FIRST_RAIN);
				break;

			default:
				myMusic.playFromSeconds(0.0f);
				break;
			}
		}
		myMusic.setAudio(VOLUME_LEVEL);
	}

	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	return (0);
}

void resize(int width, int height)
{
	/* code */
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	// updating camera's resolution
	camera.updateResolution(width, height);

	giWindowWidth = width;
	giWindowHeight = height;

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		10000000.0f);
}

void display(void)
{
	ELAPSED_TIME = sdkGetTimerValue(&timer);
	ELAPSED_TIME = ELAPSED_TIME / 1000.0f;

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (enableBezierCameraControl)
	{
		bezierPoints[vectorIndex][0] = objX;
		bezierPoints[vectorIndex][1] = objY;
		bezierPoints[vectorIndex][2] = objZ;
		yawGlobal[vectorIndex] = scaleX;
		pitchGlobal[vectorIndex] = scaleY;
		// fovGlobal[vectorIndex] = scaleZ;
	}

	updateGlobalViewMatrix();

	// ==================================== SCENE
	mainScene->display();
	// ==================================== DISPLAY TEXT IN TITLE BAR
	char titleText[255];
	sprintf(titleText, "CameTime = %f Time = %f Index = %d Variables | OBJ %f :: %f :: %f | SCALE %f :: %f :: %f", globalTime, ELAPSED_TIME, vectorIndex, objX, objY, objZ, scaleX, scaleY, scaleZ);
	SetWindowTextA(ghwnd, (LPCSTR)titleText);
	// =============================================================

	SwapBuffers(ghdc);
}

float camSpeed = 0.0001f;
void update(void)
{
	mainScene->update();
	if (globalTime <= 1.0f)
		globalTime += (0.000015f + globalSpeedAdjust);
	// globalTime += (0.000015f + camSpeed);

	// if (globalTime > 0.17f)
	// {
	// 	if (camSpeed < 0.0008f)
	// 		camSpeed += 0.00003f;
	// }
	// globalTime = 0.8f + globalSpeedAdjust;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	mainScene->uninitialize();
	commonShaders->uninitialize();

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghwnd = NULL;
		ghdc = NULL;
	}

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
	}

	CloseLogFile();
}

void setGlobalBezierCamera(BezierCamera *bezierCamera)
{
	globalBezierCamera = bezierCamera;
}

void setSelectedScene(int selScene)
{
	// if (mainScene->START_E2E_DEMO == true)
	mainScene->selected_scene = selScene;
}
void setCurrentPhase(int phase)
{
	PHASE_CURRENT = phase;
}
void updateGlobalViewMatrix()
{
	if (USE_FPV_CAM)
	{
		viewMatrix = camera.getViewMatrix();
	}
	else
	{
		if (globalBezierCamera)
			viewMatrix = globalBezierCamera->getViewMatrix();
	}
}