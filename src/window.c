#include "window.h"

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
    LRESULT result = 0;

    switch (msg) {
	case WM_SIZE:
		struct Framebuffer *fb = (struct Framebuffer *)GetWindowLongPtr(window, GWLP_USERDATA);
		if(fb) {
			DestroyFramebuffer(fb);
			
			struct Framebuffer *new_fbuff;
			new_fbuff = CreateFramebuffer(window);

			fb->buffer = new_fbuff->buffer;
			fb->bpp = new_fbuff->bpp;
			fb->width = new_fbuff->width;
			fb->height = new_fbuff->height;
			fb->size = new_fbuff->size;
			fb->bitmap_handle = new_fbuff->bitmap_handle;
			fb->info = new_fbuff->info;
			fb->bitmap_hdc = new_fbuff->bitmap_hdc;
			
			VirtualFree(new_fbuff, 0, MEM_RELEASE);
		}
		break;
		
    case WM_PAINT:
        PAINTSTRUCT ps = {0};
        BeginPaint(window, &ps);
        EndPaint(window, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_CHAR:
		/* int ascii_code = (int)wparam; */
		/* int repeat_count = lparam & 0x7FFF; */
		/* bool is_special_key = lparam & 0x400000; */
			
		//printf("ascii: %c, repeat count: %d, is special key: %d\n", ascii_code, repeat_count, is_special_key);
			
		break;
	
    default:
        result = DefWindowProc(window, msg, wparam, lparam);
    }
    return result;
}

struct Window OpenWindow(int w, int h, char *title) {
    HWND window_handle = {0};
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
    window_handle = CreateWindowEx(0, wc.lpszClassName, title, WS_CAPTION |
                                   WS_SYSMENU |
                                   WS_THICKFRAME |
                                   WS_MINIMIZEBOX |
                                   WS_MAXIMIZEBOX,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   h, w, 0, 0,
                                   wc.hInstance, 0);
	
    RECT window_rect = {0};
    window_rect.left = 0;
    window_rect.top = 0;
    window_rect.right = w;
    window_rect.bottom = h;

    AdjustWindowRect(&window_rect, WS_CAPTION | WS_SYSMENU, GetMenu(window_handle) != 0);

    int window_width = window_rect.right - window_rect.left;
    int window_height = window_rect.bottom - window_rect.top;

    MoveWindow(window_handle, 0, 0, window_width, window_height, 1);
    ShowWindow(window_handle, SW_SHOW);

    struct Window new_window;
    new_window.wnd_h = window_handle;
    new_window.wc = wc;
    new_window.width = window_width;
    new_window.height = window_height;

    return new_window;
}

bool MessageLoop(struct Input *input) {
    bool running = true;
    MSG msg;

    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case WM_QUIT:
            running = false;
            break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
			GetKeyboardInput(input, msg);
            break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
            input->keyboard[msg.wParam].down = false;
			input->keyboard[msg.wParam].pressed_this_frame = false;
			input->keyboard[msg.wParam].down_previous_frame = false;
            break;
			
		case WM_LBUTTONDOWN:
			input->left_click_down = true;
			break;

		case WM_LBUTTONUP:
			input->left_click_down = false;
			break;
			
		case WM_RBUTTONDOWN:
			input->right_click_down = true;
			break;
			
		case WM_RBUTTONUP:
			input->right_click_down = false;
			break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return running;
}
