#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include "..\include\mathlib.h"

enum key_code
{
    space_key = 0x20,

    key = 0x30,
    row_1_key,
    row_2_key,
    row_3_key,
    row_4_key,
    row_5_key,
    row_6_key,
    row_7_key,
    row_8_key,
    row_9_key,
    
    a_key = 0x41,
    b_key, 
    c_key,
    d_key, 
    e_key, 
    f_key, 
    g_key,
    h_key,
    i_key,
    j_key,
    k_key,
    l_key,
    m_key,
    n_key,
    o_key,
    p_key,
    q_key,
    r_key,
    s_key,
    t_key,
    u_key,
    v_key,
    w_key,
    x_key,
    y_key,
    z_key,
    arrow_left = 0x25,
    arrow_up,
    arrow_right,
    arrow_down,
    tilde_key = 0xC0,
};

struct KeyState
{
    bool down;
    bool down_previous_frame;
    bool pressed_this_frame;
    bool toggle;
};

struct Input
{
    int mouse_x;
    int mouse_y;
	
    bool left_click;
    bool left_click_locked;
    bool right_click;

    struct KeyState keyboard[256];
};

__declspec(dllexport) v2 GetMousePosition(HWND window);
__declspec(dllexport) void GetInput(struct Input *input);

#endif // INPUT_H
