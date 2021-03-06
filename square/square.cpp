
//    Draw a Square

#include "stdafx.h"
#include "square.h"
#include "shader.h"
#include "matrix.h"
#include "model.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CShader shader;
CMatrix matrix;
CModel model;


// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC *hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(int width, int height);

void OnFileExit(HWND hWnd);


// main window
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SQUARE, szWindowClass, MAX_LOADSTRING);

    //  Registers the window class.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SQUARE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Store instance handle in our global variable.
    hInst = hInstance; 

    int x, y, width, height, cx, cy;

    // 480p: 854x480

    cx = 854;
    cy = 480;

    width  = cx + 16;
    height = cy + 58;

    x = (GetSystemMetrics(SM_CXSCREEN) - width)/2;
    y = (GetSystemMetrics(SM_CYSCREEN) - height)/2;

    // Create the main program window.
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		x, y, width, height, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    // Display the main program window.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SQUARE));

    MSG msg;

    // Main message loop
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;

    switch (message)
    {
	case WM_PAINT:   OnPaint(hDC);									break;
	case WM_CREATE:  OnCreate(hWnd, &hDC);							break;
	case WM_DESTROY: OnDestroy(hWnd, hDC);							break;
	case WM_SIZE:    OnSize(LOWORD (lParam), HIWORD (lParam));break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
void OnPaint(HDC hDC)
{
     glClear(GL_COLOR_BUFFER_BIT);

	 model.Render(matrix);

     SwapBuffers(hDC);
}

//
void OnCreate(HWND hWnd, HDC *hDC)
{
    int iPixelFormat;
    HGLRC hglRC;                // rendering context

    // create a pixel format
    static PIXELFORMATDESCRIPTOR pfd = {
         sizeof(PIXELFORMATDESCRIPTOR),   // size of structure.
         1,                               // version number
         PFD_DRAW_TO_WINDOW |             // support window
         PFD_SUPPORT_OPENGL |             // support OpenGl
         PFD_DOUBLEBUFFER,                // double buffered
         PFD_TYPE_RGBA,                   // RGBA type
         24,                              // 24-bit color depth
         0, 0, 0, 0, 0, 0,                // color bits ignored
         0,                               // no alpha buffer
         0,                               // shift bit ignored
         0,                               // no accumulation buffer
         0, 0, 0, 0,                      // accum bits ignored
         32,                              // 32-bit z-buffer
         0,                               // no stencil buffer
         0,                               // no auxiliary buffer
         PFD_MAIN_PLANE,                  // main layer
         0,                               // reserved
         0, 0, 0 };                       // layer masks ignored.

     *hDC = GetDC(hWnd);                                 // get the device context for our window
     iPixelFormat = ChoosePixelFormat(*hDC, &pfd);       // get the best available match of pixel format for the device context
     SetPixelFormat(*hDC, iPixelFormat, &pfd);           // make that the pixel format of the device context
     hglRC = wglCreateContext(*hDC);                     // create an OpenGL rendering context
     wglMakeCurrent(*hDC, hglRC);                        // make it the current rendering context

     // check the extensions
     char szText[100];
     if(!LoadFunctions())
     {
	      MessageBox(NULL, L"shader not supported!", L"OpenGL Extensions", MB_ICONWARNING | MB_OK);
	      PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		  return;
     }

	 OutputDebugStringA("-----------------------------------------------------------------------------\n");
     sprintf_s(szText,100,"OpenGL Version :%s\n",glGetString(GL_VERSION));   OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLES Version   :%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLU Version    :%s\n",glGetString(GLU_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"Vendor         :%s\n",glGetString(GL_VENDOR));    OutputDebugStringA(szText);
     sprintf_s(szText,100,"Renderer       :%s\n",glGetString(GL_RENDERER));  OutputDebugStringA(szText);
     OutputDebugStringA("-----------------------------------------------------------------------------\n");

	wchar_t szPath[MAX_PATH], szVertFile[MAX_PATH], szFragFile[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, szPath);

	wcscpy_s(szVertFile, MAX_PATH, szPath);
	wcscat_s(szVertFile, MAX_PATH, L"\\data\\vertex.txt");

	wcscpy_s(szFragFile, MAX_PATH, szPath);
	wcscat_s(szFragFile, MAX_PATH, L"\\data\\fragment.txt");

	OutputDebugString(szVertFile);
	OutputDebugString(L"\n");

	OutputDebugString(szFragFile);
	OutputDebugString(L"\n");

     // create shader
     if(!shader.Create( szVertFile, szFragFile))
     {
		 MessageBoxA(NULL, shader.infolog, shader.caption, MB_ICONWARNING | MB_OK);
	     PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		 return;
     }

	 // create model
	 model.Create();
	 model.SetShader(shader.GetHandle());

	 // clear window to black
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

//
void OnDestroy(HWND hWnd, HDC hDC)
{
     shader.Destroy();
	 model.Destroy();

	HGLRC hglRC;                // rendering context

	hglRC = wglGetCurrentContext(); // get current OpenGL rendering context
	wglMakeCurrent(hDC, NULL);      // make the rendering context not current
	wglDeleteContext(hglRC);        // delete the rendering context
	ReleaseDC(hWnd, hDC);           // releases a device context

	PostQuitMessage(0);
}

//
void OnSize(int width, int height)
{
     if(width==0) width = 1;
     if(height==0) height = 1;

     // set up the screen coordinates
	 //
	 //    28.103  +-------------------------+
	 //            |                         |
	 //            |                         |
	 //            |                         |
	 //   -28.103  +-------------------------+
	 //          -50                         50   

     glViewport(0, 0, width, height);
	 matrix.Orthographic(-50.0f, 50.0f, -28.103f, 28.103f, 0.0f, 1.0f);
}

//
void OnFileExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}
