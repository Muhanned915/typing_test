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

typedef struct{
    char buffer[256];
    unsigned char len, max_buffer_length;
    double first_delete_time, last_delete_time, first_keystroke_time, elapsed_time, wpm;
    int key_strokes,num_of_mistakes;
    bool text_ended;
    char * end_of_lines [LINE_NUM];

} typing_session_state;

typedef struct{
    int x;
    int y;
    int length;
} cursor;

Font typing_font;

void load_my_font()
{
typing_font = LoadFontEx(
    "JetBrainsMono-Bold.ttf",
    30,     // base font size (same as DrawTextEx)
    NULL,   // load all glyphs
    0
);
}


typing_session_state session = {0};
cursor cursor_1 = {(LINE_X - CURSOR_OFFSET),LINE_Y,CURSOR_LENGTH};
char test_line [256] = {"hello world"};
char num_of_words;


static const char *word_list[] = {
"the","and","to","of","a","in","is","it","you","that",
"he","was","for","on","are","as","with","his","they","i",
"at","be","this","have","from","or","one","had","by","word",
"but","not","what","all","were","we","when","your","can","said",
"there","use","an","each","which","she","do","how","their","if",
"will","up","other","about","out","many","then","them","these","so",
"some","her","would","make","like","him","into","time","has","look",
"two","more","write","go","see","no","way","could","people","my",
"than","first","water","been","call","who","oil","its","now","find",
"long","down","day","did","get","come","made","may","part","over",
"new","sound","take","only","little","work","know","place","year",
"live","me","back","give","most","very","after","thing","our","just",
"name","good","sentence","man","think","say","great","where","help",
"much","before","line","right","too","mean","old","any","same","tell",
"boy","follow","came","want","show","also","around","form","three","small",
"set","put","end","does","another","well","large","must","big","even",
"such","because","turn","here","why","ask","went","men","read","need",
"land","different","home","us","move","try","kind","hand","picture","again",
"change","off","play","spell","air","away","animal","house","point","page",
"letter","mother","answer","found","study","still","learn","should","america","world",
"high","every","near","add","food","between","own","below","country","plant",
"last","school","father","keep","tree","never","start","city","earth","eye",
"light","thought","head","under","story","saw","left","dont","few","while",
"along","might","close","something","seem","next","hard","open","example","begin",
"life","always","those","both","paper","together","got","group","often","run",
"important","until","children","side","feet","car","mile","night","walk","white",
"sea","began","grow","took","river","four","carry","state","once","book",
"hear","stop","without","second","later","miss","idea","enough","eat","face",
"watch","far","real","almost","let","above","girl","sometimes","mountain",
"cut","young","talk","soon","list","song","being","leave","family","body",
"music","color","stand","sun","questions","fish","area","mark","dog","horse",
"birds","problem","complete","room","knew","since","ever","piece","told","usually",
"friends","easy","heard","order","red","door","sure","become","top","ship",
"across","today","during","short","better","best","however","low","hours","black",
"whole","measure","remember","early","waves","listen","wind","rock","space","fast",
"hold","toward","five","step","morning","passed","true","hundred","against","pattern",
"north","slowly","money","map","farm","pulled","draw","voice","seen","cold",
"cried","plan","notice","south","sing","war","ground","fall","king","town",
"unit","figure","field","travel","wood","fire","upon","done","english","road",
"ten","fly","gave","box","finally","wait","correct","oh","quickly","person",
"became","shown","minutes","strong","verb","stars","front","feel","fact","street",
"decided","contain","course","surface","produce","building","ocean","class","note",
"nothing","rest","carefully","inside","wheels","stay","green","known","week","less",
"machine","base","ago","stood","plane","system","behind","ran","round","boat",
"game","force","brought","understand","warm","common","bring","explain","dry","though",
"language","shape","deep","thousands","yes","clear","equation","yet","filled","heat",
"full","hot","check","object","rule","among","noun","power","cannot","able",
"six","size","dark","ball","material","special","heavy","fine","pair","circle",
"include","built","matter","square","perhaps","felt","suddenly","test","direction",
"center","ready","anything","divided","general","energy","subject","moon","return",
"believe","dance","members","picked","simple","cells","paint","mind","love","cause",
"rain","exercise","train","blue","wish","drop","developed","window","difference",
"distance","heart","site","sum","summer","wall","forest","likely","wrote","length",
"reason","kept","interest","arms","brother","race","present","beautiful","store",
"job","edge","past","sign","record","finished","discovered","wild","happy","effect",
"scale","names","settled","consider","operation","logic","binary","value","memory",
"pointer","array","string","buffer","stack","heap","process","thread","signal",
"device","driver","kernel","firmware","code","input","output","screen","keyboard",
"mouse","timer","event","frame","render","update","state","loop","random","speed",
"score","level","start","pause","reset","clear","draw","load","save","file",
"path","open","close","read","write","error","debug","build","test","run",
"compile","link","execute","logic","flow","check","count","index","value","limit"
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

void draw_typed_text()
{
    char key_to_send[2] = {'\0','\0'};
    Color char_color;
    short char_width;
    Vector2 char_position = {LINE_X,LINE_Y};

    for(int i = 0;i < session.len;i++){
        char_color = WHITE;
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
    char_position.x -= 5;
    DrawLine(char_position.x, char_position.y, char_position.x, (char_position.y + CURSOR_LENGTH),WHITE);
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



