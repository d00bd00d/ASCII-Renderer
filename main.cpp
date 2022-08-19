#include <ncurses.h>
#include "canvas.h" 

#define FRAMERATE 45

int main(int argc, char** argv) {
    // initialise curses
    initscr(); 
    noecho(); 
    cbreak(); 
    curs_set(0); 
    timeout(1000 / FRAMERATE); 

    // colours; stick to terminal default 
    start_color();
    use_default_colors(); 
    init_pair(1, -1, -1); 

    int max_x, max_y; 
    getmaxyx(stdscr, max_y, max_x); 

    // where all the magic happens nwn 
    const char* file = "torus.vert"; 
    if(argc > 1) 
        file = argv[1]; 

    Canvas canvas(max_x, max_y, stdscr, file); 
    char ch = getch(); 

    do {
        canvas.redraw(); 
        ch = getch(); 
    } while(ch != 'q'); 

    endwin(); 
    return 0; 
} 
