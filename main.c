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

        if(IsKeyPressed(KEY_RIGHT_SHIFT)){
            session = (typing_session_state){0};
            init_text();
            find_end_of_lines();
        }
        
        is_text_limit_reached();
        if (session.text_ended == false){
            draw_test_line();
            input_buffer();
            register_backspace();
            draw_typed_text();
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
