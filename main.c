#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "typing_functions.h"
#include "typing_session_state.h"

#define SPACE_BETWEEN_CHARS 10
#define LINE_X 450
#define LINE_Y 500

Vector2 line_pos = {LINE_X,LINE_Y};
Vector2 ff;
extern const char test_line [256];




int main(void)
{
    const int screenWidth  = GetMonitorWidth(0);
    const int screenHeight = GetMonitorHeight(0);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Fullscreen Windowed");
    MaximizeWindow();
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_NONE);

    srand(time(NULL));
    load_my_font();
    init_text();
    find_end_of_lines();
    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if(IsKeyPressed(KEY_LEFT_SHIFT)){
            session.text_locked = !session.text_locked;
            printf(" text_locked = %i\n",session.text_locked);
        }

        if(IsKeyPressed(KEY_RIGHT_SHIFT)){
            if(session.text_locked == false){
            session = (typing_session_state){0};
            init_text();
            find_end_of_lines();
            }

            else{
                int temp = session.max_buffer_length;
                session = (typing_session_state){0};
                session.max_buffer_length = temp;
                session.text_locked = true;
                find_end_of_lines();
            }
        }

        if (session.text_ended == false && session.len == session.max_buffer_length){
            printf("%i %i\n",session.len,session.max_buffer_length);
            session.text_ended = true;
            session.elapsed_time = GetTime() - session.first_keystroke_time;
            // printf("elapsed time = %f   first key time = %f",current_session.elapsed_time, current_session.first_keystroke_time);
            calculate_wpm();
        }

        if (session.text_ended == false){
            draw_test_line();
            input_buffer();
            register_backspace();
            draw_typed_text();
            if(session.text_locked == true)     draw_lock();
        }

        else{
            DrawText("WPM = ",500,500,30,BLUE);
            DrawText(TextFormat("%.2f",session.wpm),600,500,30,BLUE);
            DrawText("Elapsed time: ",500,570,30,ORANGE);
            DrawText(TextFormat("%.2f",session.elapsed_time),710,570,30,ORANGE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
