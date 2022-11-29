#include "network.h"
#include "window.h"
#include "draw.h"
#include "timing.h"
#include "input.h"
#include "ui.h"
#include "common.h"
#include "mathlib.h"

#include <assert.h>

#define CHAT_MAX_BUFF_ROWS 1000
#define CHAT_MAX_BUFF_COLS MAX_TEXTFIELD_LEN

void copy_input_to_chat_buff(char *src, int src_len, char buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS], int buff_index) {
	if(src[0] != '\0') {
		assert(src_len <= CHAT_MAX_BUFF_COLS);
	
		for(int i = 0; i < src_len; i++) {
			buff[buff_index][i] = src[i];
		}
		buff[buff_index][src_len] = '\0';
	}
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Pass server or client option(server/client) as command line argument.");
		return 0;
	}
	
	int window_width = 800;
	int window_height = 600;
	
	struct Window window = OpenWindow(window_width, window_height, "Chat Demo");
	struct Input input = {0};
	struct Framebuffer *framebuffer = CreateFramebuffer(window.wnd_h);
	struct Timer main_timer = {0};
	InitTimer(&main_timer);
	SYSTEMTIME system_time = {0};
	
	struct Font font = {0};
	font.bmp = LoadBitmapFile("font.bmp");
	font.glyph_width = 5;
	font.glyph_height = 7;
	font.glyph_count = 98;
	font.glyph_spacing = 2;
	font.color_mask = RGB_Color(255, 255, 255); // i don't need a color mask, but 0 will mask off white

	WSADATA wsa_data;
	SOCKET connect_socket = INVALID_SOCKET;
	SOCKET listen_socket = INVALID_SOCKET;
	
	if(InitializeWinsock(2, 2, &wsa_data)) {
		printf("winsock initalized\n");
	}
	else {
		printf("error winsock initialisation\n");
	}
	
	char *client_or_server = argv[1];
	if(StringCmp(client_or_server, "client")) {
		char ip[] = "127.0.0.1";
		bool client_success = CreateClient(ip, DEFAULT_PORT, &connect_socket);
		if(client_success) {
			printf("successfuly created winsock client\n");
		}
	}
	else if(StringCmp(client_or_server, "server")) {
		bool server_success = CreateServer(DEFAULT_PORT, &listen_socket, &connect_socket);
		if(server_success) {
			printf("successfuly created winsock server\n");
		}
	}

	WSAPOLLFD connect_socketfd = {0};
	connect_socketfd.fd = connect_socket;
	//connect_socketfd.events = POLLRDNORM | POLLWRNORM;
	connect_socketfd.events = POLLRDNORM; 
	connect_socketfd.revents = 0;
		
	u32 background_color = RGB_Color(245, 245, 245);
	u32 input_bar_color = RGB_Color(255, 255, 255);
	u32 text_color = RGB_Color(0, 0, 0);
	u32 cursor_color = RGB_Color(20, 20, 20);
	int border_thickness = 4;
	u32 border_color = RGB_Color(204, 204, 206);
	int input_bar_w = window_width - (border_thickness*2);
	int input_bar_h = 25;
	int input_bar_x = border_thickness;
	int input_bar_y = window_height - (border_thickness+input_bar_h);
	float input_bar_delay = .3f;
	int input_bar_cursor_width = 4;
	int input_bar_cursor_height = 12;
	float input_bar_cursor_blinkrate = .5f;
	bool draw_placeholder_string = false;
		
	struct TextField chat_input_field = {0};
	chat_input_field = InitTextField(&font, input_bar_x, input_bar_y, input_bar_w, input_bar_h, input_bar_color, border_thickness, border_color, input_bar_delay, input_bar_cursor_width, input_bar_cursor_height, input_bar_cursor_blinkrate, cursor_color, text_color, draw_placeholder_string);
	chat_input_field.write_focus = true;

	char chat_buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS];
	int current_buff_line = 0;
	int chat_text_x = 25;
	int chat_text_y = 50;

	u32 user_color = RGB_Color(255, 0, 0);

	bool running = true;
	while(running) {
		StartTimer(&main_timer);
		
		ResetInput(&input);
		
		input.mouse_cursor_pos = GetMousePosition(window.wnd_h);

		GetSystemTime(&system_time);
			
		if(!MessageLoop(&input)) {
			running = false;
		}

		FillScreen(framebuffer, background_color);
		
		UpdateTextField(&chat_input_field, input, main_timer.elapsed_time);

		int poll_result = 0;
		poll_result = WSAPoll(&connect_socketfd, 1, 0);

		if(connect_socketfd.revents & POLLRDNORM) {
			// receive
			char receive_msg[256];
			int len = 256;
			recv(connect_socket, receive_msg, len, 0);

			printf("%s\n", receive_msg);
		}
		
		if(input.keyboard[enter_key].down) {
			char msg[] = "Hello";
			send(connect_socket, msg, ARRAY_LEN(msg), 0);
			
			// copy input to chat			
			assert(current_buff_line <= CHAT_MAX_BUFF_ROWS);
			copy_input_to_chat_buff(chat_input_field.text, chat_input_field.text_current_len, chat_buff, current_buff_line);

			current_buff_line++;
			
			// clear text field buff
			chat_input_field.text[0] = '\0';
			chat_input_field.text_current_len = 0;
			chat_input_field.cursor.pos = chat_input_field.cursor.initial_pos;
		}

		/* if(StringCmp(server_or_client, "client")) { */
		/* 	//recv(); */
		/* } */
		/* else { */
		/* 	//sendto(); */
		/* } */
		
		for(int i = 0; i < current_buff_line; i++) {
			DrawString(framebuffer, font, "Admin:", chat_text_x, chat_text_y+(i*12), user_color);
			DrawString(framebuffer, font, chat_buff[i], chat_text_x + 50, chat_text_y+(i * 12), text_color);
		}

		char time[64];
		IntToString(system_time.wHour, time, 64);
		time[2] = ':';
		IntToString(system_time.wMinute, time+3, 64);
		time[5] = ':';
		IntToString(system_time.wSecond, time+6, 64);
		
		DrawString(framebuffer, font, time, 400, 400, user_color);
		//DrawString(framebuffer, font, ":", 400, 400, user_color);
		
		DrawTextField(framebuffer, &chat_input_field);
		
		OutputFramebuffer(window.wnd_h, *framebuffer);

		EndTimer(&main_timer);
	}
	
    return 0;
}
