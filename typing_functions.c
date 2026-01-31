#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SPACE_BETWEEN_CHARS 10
#define LINE_X 450
#define LINE_Y 500
#define CURSOR_LENGTH 40
#define CURSOR_OFFSET 5
#define LINE_NUM 6
#define NXT_LINE_OFFSET_Y 50
#define CHARS_PER_LINE 45
#define USER_IS_TYPING 1
#define USER_ISNT_TYPING 0

typedef struct{
    char buffer[256];
    unsigned char len, max_buffer_length;
    double first_delete_time, last_delete_time, first_keystroke_time, elapsed_time, wpm;
    int key_strokes,num_of_mistakes;
    bool text_ended;
    bool text_locked;
    char * end_of_lines [LINE_NUM];

} typing_session_state;

typedef struct{
    int x;
    int y;
    int length;
} cursor;

typing_session_state session = {0};
cursor cursor_1 = {(LINE_X - CURSOR_OFFSET),LINE_Y,CURSOR_LENGTH};
char test_line [256] = {"hello world"};
char num_of_words;
double user_typed_time = 0;
bool is_user_typing = false;


static const char *word_list[] = {
    "the","be","of","and","a","to","in","he","have","it",
    "that","for","they","I","with","as","not","on","she","at",
    "by","this","we","you","do","but","from","or","which","one",
    "would","all","will","there","say","who","make","when","can","more",
    "if","no","man","out","other","so","what","time","up","go",
    "about","than","into","could","state","only","new","year","some","take",
    "come","these","know","see","use","get","like","then","first","any",
    "work","now","may","such","give","over","think","most","even","find",
    "day","also","after","way","many","must","look","before","great","back",
    "through","long","where","much","should","well","people","down","own","just",
    "because","good","each","those","feel","seem","how","high","too","place"
    "little","world","very","still","nation","hand","old","life","tell","write",
    "become","here","show","house","both","between","need","mean","call","develop",
    "under","last","right","move","thing","general","school","never","same","another",
    "begin","while","number","part","turn","real","leave","might","want","point",
    "form","off","child","few","small","since","against","ask","late","home",
    "interest","large","person","end","open","public","follow","during","present","without",
    "again","hold","govern","around","possible","head","consider","word","program","problem",
    "however","lead","system","set","order","eye","plan","run","keep","face",
    "fact","group","play","stand","increase","early","course","change","help","line"
};


#define WORD_LIST_COUNT (sizeof(word_list) / sizeof(word_list[0]))

#define LINE_LENGTH 10
#define LINE_X_PIXELS 1000
#define DRAW_SIZE 40

void choose_num_of_words()
{
    if(IsKeyPressed(KEY_KP_1)){
        num_of_words = 10;
    }
    // to be continued
}

void init_text()
{
    int len,random_index;
    test_line[0] = '\0';
    for(int i = 0;i < LINE_LENGTH;i++){
        random_index = rand() % WORD_LIST_COUNT;
        strcat(test_line,word_list[random_index]);
        len = strlen(test_line);
        test_line[len++] = ' ';
        test_line[len] = '\0';
    }
    test_line[--len] = '\0';
    session.max_buffer_length = len;
    printf("%s\n",test_line); 

}

void find_end_of_lines()
{
    int len = strlen(test_line),space_index,i = 0;
    char * ptr = test_line;
    session.end_of_lines[i++] = test_line;

    while(len > CHARS_PER_LINE && i < LINE_NUM){
        space_index = CHARS_PER_LINE;
        while(space_index > 0 && ptr[space_index] != ' '){
            space_index--;
        }
        ptr[space_index] = '\0';
        ptr = ptr + space_index + 1;
        len = len - space_index - 1;
        session.end_of_lines[i++] = ptr;
    }    
}

void draw_test_line()
{
    Vector2 line_pos = {LINE_X,LINE_Y};
    for (int i = 0;i < LINE_NUM;i++){
        if(session.end_of_lines[i] == 0)    break;                                      // break loop if no address was assigned to the pointer
        DrawTextEx(GetFontDefault(),session.end_of_lines[i],line_pos,DRAW_SIZE,SPACE_BETWEEN_CHARS,Fade(WHITE,.2f));
        line_pos.y += NXT_LINE_OFFSET_Y;

    }
}

void get_text_length()
{
    session.max_buffer_length = strlen(test_line);                                     
}

void input_buffer()
{
    char my_key;
    while((my_key = GetCharPressed()) != 0 && session.len < session.max_buffer_length){
            if(my_key >= 32 && my_key <= 126){                                      // ascii printable range
                if(session.key_strokes == 0)
                    session.first_keystroke_time = GetTime();

                for(int i = 1;i < LINE_NUM && session.end_of_lines[i] != 0;i++){
                    if(test_line + session.len == session.end_of_lines[i] - 1){            // compare address of test line index at len to address of \0
                        if(my_key == ' ')   break;                                         // if equal,break the for loop and continue the function
                        else                return;                                        // else break and don't accept any input until the user types space
                    }
                }

                session.buffer[session.len++] = my_key;
                session.key_strokes++;
                is_user_typing = true;
                user_typed_time = GetTime();
            }
    }

    session.buffer[session.len] = '\0';                                                     // len always poits to '/0'
}

void register_backspace()
{
    if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
        if(IsKeyPressed(KEY_BACKSPACE) && session.len > 0){
            while(session.len > 0 && session.buffer[session.len - 1] == ' ')
                session.buffer[--session.len] = '\0';
        
            while(session.len > 0 && session.buffer[session.len - 1] != ' ')
                session.buffer[--session.len] = '\0';
        }
    }

   else{
        if(IsKeyPressed(KEY_BACKSPACE) && session.len > 0){
                session.buffer[--session.len] = '\0';
                session.first_delete_time = GetTime();
                session.last_delete_time = session.first_delete_time;
        }

        if(IsKeyDown(KEY_BACKSPACE) && session.len > 0){
            if (GetTime() - session.first_delete_time >= .5){
                if(GetTime() - session.last_delete_time >= 0.05){
                session.buffer[--session.len] = '\0';
                session.last_delete_time = GetTime();
                }   
            }          
        }
    } 
}

void draw_cursor(int cursor_x,int cursor_y)
{
    static double last_cursor_blink_time = 0;
    static bool render_cursor = true;

    if(GetTime() - last_cursor_blink_time >= .5){
        last_cursor_blink_time = GetTime();
        render_cursor = !render_cursor;
    }

    if(render_cursor == true){
        cursor_x -= 5;
        DrawLine(cursor_x, cursor_y, cursor_x, (cursor_y + CURSOR_LENGTH),WHITE);
    }
}

void draw_typed_text()
{
    char key_to_send[2] = {'\0','\0'};
    Color char_color;
    short char_width;
    Vector2 char_position = {LINE_X,LINE_Y};

    for(int i = 0;i < session.len;i++){
        char_color = GREEN;
        key_to_send[0] = test_line[i];
       // char_width = MeasureText(key_to_send,30);
        char_width = MeasureTextEx(GetFontDefault(),key_to_send,DRAW_SIZE,0).x;

        if (session.buffer[i] != test_line[i]){
            char_color = RED;
            if(test_line[i] == ' ')   key_to_send[0] = '_';
            
            else if(test_line[i] == '\0' && session.buffer[i] == ' '){                  // end of line --> move cursor to next line
                 
                char_position.x = LINE_X - SPACE_BETWEEN_CHARS;                         // cancel space between chars for the cursor for the new line
                char_position.y += NXT_LINE_OFFSET_Y;
            }
        }
        DrawTextEx(GetFontDefault(),key_to_send,char_position,DRAW_SIZE,0,char_color);
        //DrawText(key_to_send, char_pos_x, char_pos_y, 30, char_color);
        char_position.x = char_position.x + char_width + SPACE_BETWEEN_CHARS;
    }
    //draw_cursor(char_position.x,char_position.y);

    if(GetTime() - user_typed_time >= .5){
        is_user_typing = false;
    }

    if(is_user_typing == false){
        draw_cursor(char_position.x,char_position.y);
    }
    else if(is_user_typing == true){
        char_position.x -= 5;
        DrawLine(char_position.x, char_position.y, char_position.x, (char_position.y + CURSOR_LENGTH),WHITE);
    }
}


/*void draw_typed_text()
{
    for (int i = 1;i < LINE_NUM;i++){
        if(current_session.end_of_lines[i] == 0)    break;          // break loop if no address was assigned to the pointer
        current_session.end_of_lines[i]--;                          // point to \0 in the end of line
        *current_session.end_of_lines[i] = ' ';                      // return it to the original space
    }
}
    */




void calculate_wpm()
{
    double word_count,minutes;

    for (int i = 1;i < LINE_NUM;i++){
        if(session.end_of_lines[i] == 0)    break;          // break loop if no address was assigned to the pointer
        session.end_of_lines[i]--;                          // point to \0 in the end of line
        *session.end_of_lines[i] = ' ';                      // return it to the original space
    }

    for(int i = 0;i < session.max_buffer_length;i++){
        if(session.buffer[i] != test_line[i] && test_line[i] != '\0')
            session.num_of_mistakes++;
    }
   // printf("number of mistakes = %d  number of characters = %d\n",session.num_of_mistakes,session.max_length - session.num_of_mistakes);
   // fflush(stdout);
    word_count = (session.max_buffer_length - session.num_of_mistakes) / 5.0;
    minutes = session.elapsed_time /60.0f;        //convert to minutes
    session.wpm = word_count / minutes;
}

void is_text_limit_reached()
{
    if(session.text_ended == false && session.len == session.max_buffer_length){
        printf("%i %i\n",session.len,session.max_buffer_length);
        session.text_ended = true;
        session.elapsed_time = GetTime() - session.first_keystroke_time;
       // printf("elapsed time = %f   first key time = %f",current_session.elapsed_time, current_session.first_keystroke_time);
        calculate_wpm();
    }  
}

void draw_lock()
{
    DrawText("Text locked",840,200,30,YELLOW);
}



