#ifndef TYPING_SESSION_STATE_H
#define TYPING_SESSION_STATE_H
typedef struct{
    char buffer[256];
    unsigned char len, max_buffer_length;
    double first_delete_time, last_delete_time, first_keystroke_time, elapsed_time, wpm;
    int key_strokes,num_of_mistakes;
    bool text_ended;
} typing_session_state;

extern typing_session_state session;
#endif