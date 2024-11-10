#include <stdio.h>
#include <ctype.h>
#include <kbd.h>
#include <console.h>
#include <string.h>

char buffer[128] = {0};
char* bufptr = buffer;
psf2_t* font = (psf2_t*) &_binary_font_psf_start;

// screen dimensions
int width;
int height;

// cursor position
int current_x;
int current_y;

void console_init(void)
{
    width = bootboot.fb_width;
    height = bootboot.fb_height;

    current_x = 0;
    current_y = 0;
}

void consputc(char c)
{
    //psf2_t* font = (psf2_t*) &_binary_font_psf_start;
    int glyph_x, glyph_y;
    int line, mask, offset;
    int bpl = (font->width + 7) / 8;
    int newline = 0;

    if(current_x >= width / font->width - 24) {
        // reset x and go into the next line
        current_x = 0;
        current_y += width;
        newline = 1;
    }

    if(current_y + width * 2 >= ((height / font->height) * width) && (c == '\n' || newline)) {
        // scroll everything up by 1 line
        for(int k = 0; k < font->height; k++) {
        
        for(int j = 0; j < height - 1; j++)
            for(int i = 0; i < width; i++)
                fb[j * width + i] = fb[(j+1) * width + i];
        }
        
        current_x = 0;
        current_y -= width;
        newline = 0;
    }

    if(c == '\n') {
        current_x = 0;
        current_y += width;
        return;
    }

    if(c == '\b') {
        return;
    }

    if(isgraph(c) || c == ' ') {
        unsigned char* glyph = (unsigned char*) &_binary_font_psf_start +
            font->headersize + (c < font->numglyph ? c : 0) * font->bytesperglyph;
        offset = (current_x * (font->width + 1) * 4);

        for(glyph_y = 0; glyph_y < font->height; glyph_y++) {
            line = offset;
            mask = 1 << (font->width - 1);

            for(glyph_x = 0; glyph_x < font->width; glyph_x++) {
                fb[current_y * font->height + line/4] = ((int) *glyph) & (mask) ? 0xFFFFFF : 0;
                mask >>= 1;
                line += 4;
            }

            glyph += bpl;
            offset += bootboot.fb_scanline;
        }

        current_x++;
    }
}

void exec(char* cmd)
{
    if(!(*cmd)) return;

    if(strncmp(cmd, "echo ", 5) == 0) printf("%s\n", cmd + 5);
    else if(strncmp(cmd, "console x", 9) == 0) 
        printf("console x axis: %d/%d\n", current_x, width / font->width);
    else if(strncmp(cmd, "console y", 9) == 0) 
        printf("console y axis: %d/%d\n", current_x, height / font->height);
    else if(strncmp(cmd, "font x", 6) == 0)
        printf("font x axis: %d\n", font->width);
    else if(strncmp(cmd, "font y", 6) == 0)
        printf("font y axis: %d\n", font->height);
    else if(strcmp(cmd, "ls") == 0) printf("no filesystem yet lol\n");
    else printf("exec: unknown command\n");
}

void consoleint(char c)
{
    consputc(c);
    
    if(isgraph(c) || c == ' ') {
        *bufptr = c;
        if(bufptr == buffer + 128) bufptr = buffer;
        else bufptr++;
    }

    if(c == '\n') {
        exec(buffer);
        memset(buffer, 0, 128);
        bufptr = buffer;
    }

    if(c == '\b') {
        if(bufptr > buffer) bufptr--;
        *bufptr = 0;
        if(current_x) current_x--;
        putchar(' ');
        current_x--;
    }
}

/*void console_debug(void)
{
    psf2_t* font = (psf2_t*) &_binary_font_psf_start;
    int saved_x = current_x;
    int saved_y = current_y;
    current_x = 10;
    current_y = 0;
    puts("current_y: ");
    printud(saved_y);
    puts("\nwidth: ");
    printud(width);
    puts("\nfont->width: ");
    printud(font->width);
    puts("\n(height/font->height - 1) * width: ");
    printud((height/font->height - 1) * width);
}*/
