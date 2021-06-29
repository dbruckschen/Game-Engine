#include "window.h"

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    
    switch (msg) 
    {
	case WM_SIZE: 
	{
	}break;
        
	case WM_PAINT: 
	{
	    PAINTSTRUCT ps = {0};
	    BeginPaint(window, &ps);
	    EndPaint(window, &ps);
	}break;
		
	case WM_DESTROY: 
	{
	    PostQuitMessage(0);
	}break;
	
	default:
	{
	    result = DefWindowProc(window, msg, wparam, lparam);
	}
    }
    return result;
}

Window OpenWindow(int w, int h, char *title)
{
    HWND window_handle = {0};
    WNDCLASSEX wc = {0};
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style  = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
    //wc.hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON), IMAGE_ICON, 32, 32, 0);
    //wc.hCursor = (HCURSOR)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_CURSOR), IMAGE_CURSOR, 32, 32, 0);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "WindowClass";
    //wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON), IMAGE_ICON, 32, 32, 0);
    
    RegisterClassEx(&wc);
    window_handle = CreateWindowEx(0, wc.lpszClassName, title, WS_CAPTION|
                                   WS_SYSMENU|
                                   WS_THICKFRAME|
                                   WS_MINIMIZEBOX|
                                   WS_MAXIMIZEBOX,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   h, w,
                                   0, 0,
                                   wc.hInstance, 0);

    RECT window_rect = {0};
    window_rect.left = 0;
    window_rect.top = 0;
    window_rect.right = w;
    window_rect.bottom = h;

    AdjustWindowRect(&window_rect, WS_CAPTION|WS_SYSMENU, GetMenu(window_handle) != 0);
    MoveWindow(window_handle, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 1);
    ShowWindow(window_handle, SW_SHOW);
	
    Window new_window;

    new_window.wnd_h = window_handle;
    new_window.wc = wc;
	
    return new_window;
}

bool MessageLoop(Input *input)
{
    bool running = true;
    MSG msg;
	
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
    {
	switch(msg.message)
	{
	    case WM_QUIT:
	    {
		running = false;
	    }break;

	    case WM_SYSKEYDOWN:
	    case WM_KEYDOWN:
	    {
		input->keyboard[msg.wParam].down = true;
		//printf("0x%x pressed\n", msg.wParam);
		
		bool was_down_last_frame = (msg.lParam >> 30) & 1;
		if(!was_down_last_frame)
		{
		    input->keyboard[msg.wParam].down_previous_frame = true;
		    //printf("0x%x pressed this frame\n", msg.wParam);
		}
		else
		{
		    input->keyboard[msg.wParam].down_previous_frame = false;
		}

		if(input->keyboard[msg.wParam].toggle == false && was_down_last_frame == false)
		{
		    printf("toggle an\n");
		    input->keyboard[msg.wParam].toggle = true;
		}
		else if(input->keyboard[msg.wParam].toggle == true && was_down_last_frame == false)
		{
		    printf("toggle aus\n");
		    input->keyboard[msg.wParam].toggle = false;
		}
		
	    }break;

	    case WM_SYSKEYUP:
	    case WM_KEYUP:
	    {
		input->keyboard[msg.wParam].down = false;
		//printf("0x%x released\n", msg.wParam);

	    }break;
	}
	
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return running;
}
