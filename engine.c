#include "engine.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#ifdef __linux__
#include "time.h"
#include "unistd.h"
#include "X11/X.h"
#include "X11/Xlib.h"
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glu.h"
#endif

#ifdef _WIN32
#include "windows.h"
#include "wingdi.h"
#include <GL/gl.h>
#endif

#ifdef __linux__
Display *dpy;
Window root;
GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;
#endif

#ifdef _WIN32
HWND hwnd;
#endif

//COMMON INITS

void initPixelDrawing(){

	//init screen pixels
	screenPixels = malloc(sizeof(Engine_Pixel) * screenWidth * screenHeight);

	//init color buffers
	for(int i = 0; i < ENGINE_COLORS_LENGTH; i++){
		for(int j = 0; j < COLOR_BUFFER_SIZE; j++){
			colorBuffers[i][j] = ENGINE_COLORS[i];
		}
	}

}

//ENGINE ENTRY POINT

#ifdef __linux__
int main(){

	//setup window
	dpy = XOpenDisplay(NULL);

	if(dpy == NULL){
		printf("Cannot open X display!\n");
		return 0;
	}

	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL){
		printf("Could not Choose X Visual\n");
		return 0;
	}

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | StructureNotifyMask;

	win = XCreateWindow(dpy, root, 0, 0, screenWidth, screenHeight, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);

	XStoreName(dpy, win, "engine-program");

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	//common inits
	initPixelDrawing();

	Engine_init();

	//game loop
	size_t startTicks = 0;
	size_t endTicks = 0;

	int fps = 60;

	size_t ticksPerFrame = CLOCKS_PER_SEC / fps;

	bool quit = false;

	while(!quit){

		startTicks = clock();

		//handle events
		while(XPending(dpy) > 0){

			XNextEvent(dpy, &xev);

			if(xev.type == ClientMessage
			|| xev.type == DestroyNotify){
				quit = true;
			}

			if(xev.type == KeyPress){

				if(xev.xkey.keycode == XKeysymToKeycode(dpy, XK_Q)){
					quit = true;
				}
			}
		
		}

		//update

		Engine_update();

		//draw

		Engine_draw();

		glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, screenPixels);

		glXSwapBuffers(dpy, win);

		endTicks = clock();

		int lag = (endTicks - startTicks);

		if(lag < 0){
			lag = 0;
		}

		float lagMilliseconds = (float)lag / (float)(CLOCKS_PER_SEC / 1000);

		usleep(lagMilliseconds * 1000);

	
	}

	Engine_finnish();

	return 0;

}
#endif

#ifdef _WIN32
bool QUIT_PROGRAM = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	//setup window
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

	//common inits
	initPixelDrawing();
	
	Engine_init();
	
	ShowWindow(hwnd, nCmdShow);
	
	//game loop
	int currentTime = GetTickCount();
	int lastTime = currentTime;
	int deltaTime = 0;
	int currentFrameTime = 0;
	
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

	Engine_finnish();
	
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
#endif

//ENGINE FUNCTIONS

//WINDOW FUNCTIONS

void Engine_setWindowTitle(char *title){

#ifdef __linux__
	XSetWMName(dpy, win, title);
#endif

}

void Engine_setWindowSize(int width, int height){

#ifdef __linux__
	XResizeWindow(dpy, win, width, height);
#endif

#ifdef _WIN32
	SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_SHOWWINDOW);
#endif

	screenWidth = width;
	screenHeight = height;

	screenPixels = malloc(sizeof(Engine_Pixel) * width * height);

}

//DRAWING FUNCTIONS
Engine_Pixel *Engine_getScreenPixelPointer(int x, int y){
	return screenPixels + x + (screenHeight - y - 1) * screenWidth;
}

void Engine_fillRect(int x, int y, int w, int h, enum Engine_ColorEnum color){

	if(x > screenWidth
	|| y > screenHeight
	|| x + w < 0
	|| y + h < 0){
		return;
	}

	if(x + w > screenWidth){
		w = screenWidth - x;
	}
	if(y + h > screenHeight){
		h = screenHeight - y;
	}
	if(x < 0){
		w = w + x;
		x = 0;
	}
	if(y < 0){
		h = h + y;
		y = 0;
	}

	for(int i = 0; i < h; i++){

		Engine_Pixel *pixel_p = Engine_getScreenPixelPointer(x, y + i);

		memcpy(pixel_p, colorBuffers[color], w * sizeof(Engine_Pixel));

	}
}

void Engine_drawLine(Vec2f p1, Vec2f p2, Engine_Pixel color){
	
	Vec2f diff = p1;
	Vec2f_sub(&diff, &p2);
	
	int length = Vec2f_getMag(diff);

	Vec2f_normalize(&diff);
		
	Vec2f pos = p2;

	for(int i = 0; i < length; i++){

		Engine_Pixel *pixel_p = Engine_getScreenPixelPointer(round(pos.x), round(pos.y));

		*pixel_p = color;
		
		pos.x += diff.x;
		pos.y += diff.y;

	}


}

void Engine_drawTriangle(Vec2f *verts, Engine_Pixel color){

	for(int i = 0; i < 3; i++){

		Vec2f p1 = verts[(i + 0) % 3];
		Vec2f p2 = verts[(i + 1) % 3];

		Engine_drawLine(p1, p2, color);
	
	}

}

void Engine_fillTriangle(Vec2f *verts, Engine_Pixel color){

	

}
