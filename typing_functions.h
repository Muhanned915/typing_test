#ifndef TYPING_FUNC_H
#define TYPING_FUNC_H

void input_buffer(void);
void register_backspace(void);
void draw_typed_text(void);
void get_text_length(void);
bool is_text_limit_reached(void);
void get_first_keystroke_time(void);
double calculate_wpm(void);
void init_text(void);
void draw_test_line(void);
void find_end_of_lines();
void load_my_font();
#endif