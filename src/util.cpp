#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <panel.h>
#include "map.h"
#include "util.h"
#include "player.h"

using namespace std;

std::string get_path(void) {
	char buf[1024];
#ifdef WINDOWS
    GetModuleFileName(NULL, buf, 1024);
    std::string::size_type pos = std::string(buf).find_last_of("\\/");
    return std::string(buf).substr(0, pos);
#elif __GNUC__
    ssize_t len = ::readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if (len != -1) {
	buf[len] = '\0';
    std::string s = std::string(buf);
    return s.erase(s.rfind("/"));
    } else {
		error("Error: Could not determine working directory\n");
		return "";
    }
#endif
}

void _error(const char* file, unsigned line, std::string mess) {
    //attron(COLOR_PAIR(1));
    //mvprintw(0, 0, ("Error in file " + string(file) + " at line  " + to_string(line) + ": " + mess).c_str());
    //attroff(COLOR_PAIR(1));
    player->add_message(message("Error in file " + string(file) + " at line  " + to_string(line) + ": " + mess, 1));
}

void display_help(void) {
    WINDOW* help_win = newwin(20, 80, 0, 0);
    PANEL*  help_panel = new_panel(help_win);
    top_panel(help_panel);
    uint16_t scroll = 0;
    while(true) {
        wclear(help_win);
        box(help_win, 0, 0);
        for(uint16_t i = scroll; i < scroll + 18 && help_lines[i] != NULL; ++i) {
            mvwprintw(help_win, i - scroll + 1, 1, help_lines[i]);
        }
        update_panels();
        doupdate();
        int input = wgetch(help_win);
        switch(input) {
            case 'j':
            case '2':
            case KEY_DOWN:
                if(help_lines[scroll + 18] != NULL)
                    ++scroll;
                break;
            case 'k':
            case '8':
            case KEY_UP:
                if(scroll > 0)
                    --scroll;
                break;
            case 'q':
                return;
        }
    }
    del_panel(help_panel);
    delwin(help_win);
}

char* load_file(const char* path) {
    FILE* file = fopen((get_path() + path).c_str(), "rb");
    if(!file) {
        error("File " + get_path() + path + " not found.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* filedata = new char[filesize];
    fread(filedata, 1, filesize, file);
    filedata[filesize - 1] = '\0';

    fclose(file);
    return filedata;
}

vec2 step(vec2 delta) {
    if(delta.x == 0) {
        if(delta.y == 0)
            return delta;
        return vec2(0, -delta.y / abs(delta.y));
    } else if(delta.y == 0)
        return vec2(-delta.x / abs(delta.x), 0);

    if((float)delta.y / (float)delta.x < 0.6)
        return vec2(-delta.x / abs(delta.x), 0);
    if((float)delta.x / (float)delta.y < 0.6)
        return vec2(0, -delta.y / abs(delta.y));
    return vec2(-delta.x / abs(delta.x), -delta.y / abs(delta.y));
}

vec2 select_target() {
    vec2 target = player->getPosition();
    uint16_t corner = clamp((int)player->getPosition().x - 35, 0, 922);
    wmove(map->getWindow(), target.y + 1, target.x - corner + 1);
    wrefresh(map->getWindow());
    while(char input = getch()) {
    switch(input) {
        case 'h':
        case '4':
            if(target.x - corner > 0)
                target.x--;
            break;
        case 'j':
        case '2':
            if(target.y < 17)
                target.y++;
            break;
        case 'k':
        case '8':
            if(target.y > 0)
                target.y--;
            break;
        case 'l':
        case '6':
            if(target.x - corner < 77)
                target.x++;
            break;
        case 'y':
        case '7':
            if(target.x - corner > 0 && target.y > 0) {
                target.x--;
                target.y--;
            }
            break;
        case 'u':
        case '9':
            if(target.x - corner < 77 && target.y > 0) {
                target.x++;
                target.y--;
            }
            break;
        case 'b':
        case '1':
            if(target.x - corner > 0 && target.y < 17) {
                target.x--;
                target.y++;
            }
            break;
        case 'n':
        case '3':
            if(target.x - corner < 77 && target.y < 17) {
                target.x++;
                target.y++;
            }
            break;
        case ' ':
        case '\n':
        case '5':
            return target;
            break;
        case 'q':
            return vec2(-1, -1);
            break;
        }
        wmove(map->getWindow(), target.y + 1, target.x - corner + 1);
        wrefresh(map->getWindow());
    }
    return target;
}
