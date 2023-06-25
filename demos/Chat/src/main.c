#include "GameEngine.h"
#include <assert.h>

#define CHAT_MAX_BUFF_ROWS 1000
#define CHAT_MAX_BUFF_COLS MAX_TEXTFIELD_LEN
#define MAX_USER_NAME_LENGTH 64
#define MAX_MSG_LEN_TO_SEND 1024

struct Message {
	char send_name[MAX_USER_NAME_LENGTH];
	char msg[MAX_MSG_LEN_TO_SEND];
};

enum AppState {
	MENU,
	WAIT_FOR_CONNECTION,
	RUNNING
};

enum LAST_ACTION {
	NO_ACTION, //init state
	RECEIVE,
	SEND,
};

void copy_input_to_chat_buff(char *src, int src_len, char buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS], int buff_index) {
	if(src[0] != '\0') {
		assert(src_len <= CHAT_MAX_BUFF_COLS);
	
		for(int i = 0; i < src_len; i++) {
			buff[buff_index][i] = src[i];
		}
		buff[buff_index][src_len] = '\0';
	}
}

int main(void) {
	// inital state
	enum AppState state = MENU;
	
	int window_width = 800;
	int window_height = 600;
	
	struct Window window = OpenWindow(window_width, window_height, "Chat Demo");
	struct Input input = {0};
	
	struct Framebuffer *framebuffer = CreateFramebuffer(window.wnd_h);
	struct Timer main_timer = {0};
	InitTimer(&main_timer);
	SYSTEMTIME system_time = {0};
	
	struct Font font = {0};
	font = LoadBitmapFont("font.bmp");

	// Initalize winsock and create server/client
	WSADATA wsa_data;
	SOCKET connect_socket = INVALID_SOCKET;
	SOCKET listen_socket = INVALID_SOCKET;
	
	if(InitializeWinsock(2, 2, &wsa_data)) {
		printf("winsock initalized\n");
	}
	else {
		printf("error winsock initialisation\n");
	}

	char *client_or_server;
	WSAPOLLFD connect_socketfd = {0};
	//connect_socketfd.fd = connect_socket;
	// connect_socketfd.events = POLLRDNORM | POLLWRNORM;
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
	chat_input_field = InitTextField(&font, input_bar_x, input_bar_y, input_bar_w, input_bar_h,
									 input_bar_color, border_thickness, border_color, input_bar_delay,
									 input_bar_cursor_width, input_bar_cursor_height, input_bar_cursor_blinkrate,
									 cursor_color, text_color, draw_placeholder_string);
	chat_input_field.write_focus = true;

	char chat_buff[CHAT_MAX_BUFF_ROWS][CHAT_MAX_BUFF_COLS];
	int current_buff_line = 0;
	int chat_text_x = 25;
	int chat_text_y = 50;

	u32 user_color = RGB_Color(255, 0, 0);
	
	// menu buttons
	int btn_w = 100;
	int btn_h = 20;
	int btn_y = 200;
	int btn_x = window_width/2 - (btn_w/2);
	u32 btn_color = RGB_Color(34, 244, 244);
	u32 btn_border_color = RGB_Color(50, 58, 69);
	u32 btn_text_color = RGB_Color(0, 0, 0);

	struct Button btn_host = InitTextButton(&font, btn_text_color, btn_x, btn_y, btn_w, btn_h, "Host", btn_color, 2, btn_border_color, 0.3f);
	struct Button btn_join = InitTextButton(&font, btn_text_color, btn_x, btn_y+(btn_h+20), btn_w, btn_h, "Join", btn_color, 2, btn_border_color, 0.3f);
	//struct TextField tf_host_port = ;
	//struct TextField tf_client_ip = ;

	char my_name[256] = {0};
	char receive_name[256] = {0};
	DWORD nSize = 256;
	GetComputerNameA(my_name, &nSize);
	enum LAST_ACTION last_action = NO_ACTION;
		
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
		
		switch(state) {
		case MENU:
			if(btn_host.clicked) {
				bool server_success = CreateServer(DEFAULT_PORT, &listen_socket);

				if(server_success) {
					state = WAIT_FOR_CONNECTION;
					client_or_server = "server";
				}
			}

			if(btn_join.clicked) {
				char ip[] = "127.0.0.1";
				bool client_success = CreateClient(ip, DEFAULT_PORT, &connect_socket);

				if(client_success) {
					state = RUNNING;
					connect_socketfd.fd = connect_socket;
					client_or_server = "client";
				}
				else {
				    connect_socket = 0;
				}
			}
			// Update buttons.
			// Keep the buttons in the center of the window.
			btn_host.x = framebuffer->width/2 - (btn_host.width/2);
			btn_join.x = btn_host.x;
			
			UpdateButtonStatus(&btn_host, input, main_timer.elapsed_time);
			UpdateButtonStatus(&btn_join, input, main_timer.elapsed_time);
			
			DrawTextButton(framebuffer, &btn_host);
			DrawTextButton(framebuffer, &btn_join);
			break;

		case WAIT_FOR_CONNECTION:
			struct sockaddr_storage their_addr;
			int addr_size = sizeof their_addr;
			connect_socket = accept(listen_socket, (struct sockaddr*)&their_addr, &addr_size);

			if(connect_socket != INVALID_SOCKET) {
				connect_socketfd.fd = connect_socket;
				//closesocket(listen_socket);
				printf("accept() success\n");
				state = RUNNING;
			}
			break;
			
		case RUNNING:
			UpdateTextField(&chat_input_field, input, main_timer.elapsed_time);

			// check if server/client sends data and only then call recv()
			int poll_result = 0;
			poll_result = WSAPoll(&connect_socketfd, 1, 0);

			if(poll_result == SOCKET_ERROR) {
 				int error = WSAGetLastError();
				printf("WSAPoll() returned SOCKET_ERROR; error code: %d\n", error);
			}
			
			if(poll_result > 0) {
				if(connect_socketfd.revents & POLLRDNORM) {
					// receive
					char receive_msg_buffer[CHAT_MAX_BUFF_COLS];
					int received_bytes = recv(connect_socket, receive_msg_buffer, CHAT_MAX_BUFF_COLS, 0);
					printf("received %d bytes\n", received_bytes);

					assert(current_buff_line <= CHAT_MAX_BUFF_ROWS);

					struct Message *receive_msg = {0};
					receive_msg = (struct Message *)receive_msg_buffer;
					StringCpy(receive_name, receive_msg->send_name, ARRAY_LEN(receive_msg->send_name));
					
					copy_input_to_chat_buff(receive_msg_buffer, received_bytes, chat_buff, current_buff_line);

					current_buff_line++;

					// clear text field buff
					chat_input_field.text[0] = '\0';
					chat_input_field.text_current_len = 0;
					chat_input_field.cursor.pos = chat_input_field.cursor.initial_pos;

					last_action = SEND;
				}
			} 

			if(input.keyboard[enter_key].down) {
				char msg[MAX_MSG_LEN_TO_SEND];

				// send user name + message
				StringCpy(msg, my_name, ARRAY_LEN(my_name));

				int msg_len = chat_input_field.text_current_len + ARRAY_LEN(my_name);
				StringCpy(msg + ARRAY_LEN(my_name), chat_input_field.text, msg_len);
				
				if(chat_input_field.text_current_len >= MAX_MSG_LEN_TO_SEND) {
					msg_len = MAX_MSG_LEN_TO_SEND-1;
				}
			
				int bytes_send = send(connect_socket, msg, msg_len, 0);
				printf("send %d bytes\n", bytes_send);
			
				// copy input to chat			
				assert(current_buff_line <= CHAT_MAX_BUFF_ROWS);
				copy_input_to_chat_buff(chat_input_field.text, chat_input_field.text_current_len, chat_buff, current_buff_line);

				current_buff_line++;
			
				// clear text field buff
				chat_input_field.text[0] = '\0';
				chat_input_field.text_current_len = 0;
				chat_input_field.cursor.pos = chat_input_field.cursor.initial_pos;

				last_action = SEND;
			}

			// draw stuff
			for(int i = 0; i < current_buff_line; i++) {
				switch(last_action) {
				case SEND:
					DrawString(framebuffer, font, my_name, chat_text_x, chat_text_y+(i*12), user_color, 0, 0, framebuffer->width, framebuffer->height);	
					break;
					
				case RECEIVE:
					DrawString(framebuffer, font, receive_name, chat_text_x, chat_text_y+(i*12), user_color, 0, 0, framebuffer->width, framebuffer->height);
					break;
				}
				DrawString(framebuffer, font, chat_buff[i], chat_text_x + 50, chat_text_y+(i * 12), text_color, 0, 0, framebuffer->width, framebuffer->height);
			}

			/* char time[64]; */
			/* IntToString(system_time.wHour, time, 64); */
			/* time[2] = ':'; */
			/* IntToString(system_time.wMinute, time+3, 64); */
			/* time[5] = ':'; */
			/* IntToString(system_time.wSecond, time+6, 64); */
			/* DrawString(framebuffer, font, time, 400, 400, user_color); */
		
			DrawTextField(framebuffer, &chat_input_field);
			break;
		}
		
		OutputFramebuffer(window.wnd_h, *framebuffer);

		//ResetInput(&input);
		EndTimer(&main_timer);
	}
	
    return 0;
}
