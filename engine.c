#include "engine.h"

#include "stdio.h"
#include "stdbool.h"

#include "windows.h"
#include "wingdi.h"
#include <GL/gl.h>

HWND hwnd;

int screenWidth = 160;
int screenHeight = 90;

Engine_Pixel *screenPixels;

void Engine_setWindowSize(int width, int height){
	
	screenWidth = width;
	screenHeight = height;
	
	screenPixels = malloc(sizeof(Engine_Pixel) * screenWidth * screenHeight);
	
	SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
	
}

Engine_Pixel *Engine_getScreenPixelPointer(int x, int y){
	
	return screenPixels + x + y * screenWidth;
	
}
	
bool QUIT_PROGRAM = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	const char CLASS_NAME[] = "working program";
	
	WNDCLASS wc = {};
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"working program",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if(hwnd == NULL){
		printf("Could not create Window");
		return 0;
	}
	
	HDC hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	
	int pf = ChoosePixelFormat(hdc, &pfd);
	
	if(pf == 0){
		printf("Could not choose pixel format\n");
		return 0;
	}
	
	SetPixelFormat(hdc, pf, &pfd);
	
	DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	
	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	
	Engine_init();
	
	ShowWindow(hwnd, nCmdShow);
	
	int currentTime = GetTickCount();
	int lastTime = currentTime;
	int deltaTime = 0;
	int currentFrameTime = 0;
	/*
	pixels = malloc(3 * sizeof(unsigned char) * WIDTH * HEIGHT);
	
	for(int i = 0; i < WIDTH * HEIGHT; i++){
		
		pixels[i * 3] = 255 * ((float)i / (float)WIDTH * (float)HEIGHT);
		
	}*/
	
	while(!QUIT_PROGRAM){
		
		lastTime = currentTime;
		currentTime = GetTickCount();
		deltaTime = currentTime - lastTime;
		currentFrameTime += deltaTime;
	
		MSG msg = {};
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
		}
		
		if(currentFrameTime >= 1000 / 60){
			
			currentFrameTime -= 1000 / 60;
			
			//update
			
			Engine_update();
			
		}
		
		//draw
		
		Engine_draw();
		
		glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, screenPixels);
		
		SwapBuffers(hdc);
		
	}
	
	return 0;
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_CLOSE
	|| uMsg == WM_DESTROY
	|| uMsg == WM_QUIT){
		PostQuitMessage(0);
		QUIT_PROGRAM = true;
		return 0;
	}
	
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}