#include "../libc/mem.h"
#include "../cpu/ports.h"

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define VIDMEM 0xb8000

const int DISPLAYWIDTH = 80;
const int DISPLAYHEIGHT = 25;

int vidMemRow = 0;
int vidMemCol = 0;

void setCharAt(int x, int y, char c, int colour);
char getCharAt(int x, int y);
int getScreenOffset(int x, int y);
void scrollUp();
int getCursor();
void setCursor(int offset);

void setCharAt(int x, int y, char c, int colour) {
    char * video_memory = (char*)VIDMEM;
    int index = (y * DISPLAYWIDTH + x * 2);
    *(video_memory + index) = c;
    *(video_memory + index + 1) = colour;
}

char getCharAt(int x, int y) {
    char * video_memory = (char*)VIDMEM;
    int index = (y * DISPLAYWIDTH + x * 2);
    return *(video_memory + index);
}

int getScreenOffset(int x, int y) {
    return (y * DISPLAYWIDTH + x);
}

void scrollUp() {
    char * video_memory = (char*)VIDMEM;
    for (int i = 1; i < DISPLAYHEIGHT; i++) {
        memcpy(getScreenOffset(0, i) * 2 + video_memory,
               getScreenOffset(0, i - 1) * 2 + video_memory,
               DISPLAYWIDTH * 2);
    }
    for (int i = 0; i < DISPLAYWIDTH * 2; i++) {
        *(video_memory + getScreenOffset(i, 24) * 2) = 0;
    }
    setCursor(getScreenOffset(0, 24));
}

int getCursor() {
    port_byte_out(0x3d4, 14); //request high byte
    int position = port_byte_in(0x3d5);
    position = position << 8; // high byte
    port_byte_out(0x3d4, 15); //requesting low byte 
    position += port_byte_in(0x3d5); //add low byte
    return position;
}

void setCursor(int offset) {
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void print_backspace() {
    char * video_memory = (char*)VIDMEM;
    *(video_memory + getScreenOffset(vidMemCol, vidMemRow) * 2 - 2) = 0x0;
    *(video_memory + getScreenOffset(vidMemCol, vidMemRow) * 2 - 1) = 0x0f;
    vidMemCol -= 1;
    if (vidMemCol < 0) {
        vidMemCol = 80;
        vidMemRow -= 1;
    }
    int c = *(video_memory + getScreenOffset(vidMemCol, vidMemRow) * 2 - 2);
    while (c == 0) {
        vidMemCol -= 1;
        if (vidMemCol < 0) {
            vidMemCol = 80;
            vidMemRow -= 1;
        }
        c = *(video_memory + getScreenOffset(vidMemCol, vidMemRow) * 2 - 2);
    }
    setCursor(getScreenOffset(vidMemCol, vidMemRow));
}

extern void print(char * str) {
    char * video_memory = (char*)VIDMEM;
    char c = *str;
    int i = 1;
    int row = vidMemRow;
    int col = vidMemCol;
    while (c != '\0') {
        if (c == '\n') {
            row += 1;
            col = 0;
        } else {
            int vidpos = (row * DISPLAYWIDTH + col) * 2;
            *(video_memory + vidpos) = c;
            *(video_memory + vidpos + 1) = 0x0f;
            col += 1;
            if (col > DISPLAYWIDTH) {
                col = 1;
                row += 1;
            }
            if (row >= DISPLAYHEIGHT - 1) {
                scrollUp();
                row -= 1;
            }
        }
            c = *(str + i);
            i += 1;
    }
    //keep track of where we are
    vidMemRow = row;
    vidMemCol = col;
    setCursor(getScreenOffset(vidMemCol, vidMemRow));
}

void println(char * str) {
    print(str);
    print("\n");
}

extern void clearScreen() {
    char * video_memory = (char*)VIDMEM;
    for (int i = 0; i < DISPLAYWIDTH * DISPLAYHEIGHT * 2; i+= 2) {
        *(video_memory + i) = 0x0;
        *(video_memory + i + 1) = 0x0f;
    }
    setCursor(0);
}