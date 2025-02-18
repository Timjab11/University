#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef unsigned char byte;
typedef struct {
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;
typedef struct {
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    uint16_t width;
    uint16_t height;
    uint32_t pixels;
    byte depth;
    byte descriptor;
} Header;
typedef struct {
    byte blue;
    byte green;
    byte red;
} Pixel;
typedef struct {
    Header header;
    Pixel* pixels;
} Image;
Header toHeader(TGAHeader header) {
    Header result = {
        .id_length = header.id_length,
        .color_map_type = header.color_map_type,
        .image_type = header.image_type,
        .color_map = {
            header.color_map[0],
            header.color_map[1],
            header.color_map[2],
            header.color_map[3],
            header.color_map[4],
        },
        .x_origin = { header.x_origin[0], header.x_origin[1] },
        .y_origin = { header.y_origin[0], header.y_origin[1] },
        .width = *(uint16_t*)header.width,
        .height = (uint16_t)header.height[0] | ((uint16_t)header.height[1] << 8),
        .pixels = 0,
        .depth = header.depth,
        .descriptor = header.descriptor,
    };
    result.pixels = result.width * result.height;
    return result;
}
TGAHeader toTgaHeader(Header header) {
    return (TGAHeader) {
        .id_length = header.id_length,
            .color_map_type = header.color_map_type,
            .image_type = header.image_type,
            .color_map = {
                header.color_map[0],
                header.color_map[1],
                header.color_map[2],
                header.color_map[3],
                header.color_map[4],
        },
        .x_origin = { header.x_origin[0], header.x_origin[1] },
        .y_origin = { header.y_origin[0], header.y_origin[1] },
        // Either we can cast an uint16_t* to an uint8_t*
            .width = {
                ((uint8_t*)&header.width)[0],
                ((uint8_t*)&header.width)[1],
        },
        // Or we can perform bitwise operations
            .height = {
                (uint8_t)(header.height & 0xFF),
                (uint8_t)(header.height >> 8)
        },
            .depth = header.depth,
            .descriptor = header.descriptor,
    };
}
Image loadImage(const char* image) {
    FILE* file = fopen(image, "rb");
    TGAHeader tgaHeader;
    if (file == NULL) {
        return (Image) { NULL, NULL};
    }
    fread(&tgaHeader, sizeof(TGAHeader), 1, file);

    Header header = toHeader(tgaHeader);

    Pixel* pixels = (Pixel*)malloc(sizeof(Pixel) * header.pixels);

    fread(pixels, sizeof(Pixel), header.pixels, file);

    fclose(file);

    return (Image) { header, pixels };
}
void writeImage(Image image, const char* filename) {
    FILE* file = fopen(filename, "wb");

    TGAHeader header = toTgaHeader(image.header);

    fwrite(&header, sizeof(TGAHeader), 1, file);

    fwrite(image.pixels, sizeof(Pixel), image.header.pixels, file);

    fclose(file);
}
//vrchní část jsme dělali na cvičení

bool loadFonts(char* fontDir, Image* font) {
    char path[101];
    for (char c = 'A'; c <= 'Z'; c++) {
        snprintf(path, sizeof(path), "%s/%c.tga", fontDir, c);
        font[c - 'A'] = loadImage(path);
        if (font[c - 'A'].pixels == NULL) {
            printf( "Failed to load font: %c\n", c);
            return 1;
        }
    }
    return 0;
} 
void toUpperCase(char* line) {
    for (int i = 0; line[i]; i++) {
        line[i] = toupper(line[i]);
    }
}
int totalWidth(Image* font,char* line) {
    int totalWidth = 0;
    for (int i = 0; i < line[i] && line[i] != '\0'; i++)
    {
        if (line[i] == ' ') {
            totalWidth += 10;
        }
        else if (line[i] >= 'A' && line[i] <= 'Z') {
            int charWidth = font[line[i] - 'A'].header.width;
            totalWidth += charWidth ;
        }
    }
    return totalWidth;
}
void drawline(Image* image, int y, char* line, Image* font) {
    int offset = image->header.width / 2 - totalWidth(font, line) / 2;
    for (int i = 0; line[i]; i++) {
        if (line[i] == ' ') {
            offset += 10; 
            continue;
        }
        if (line[i] >= 'A' && line[i] <= 'Z') {
            Image character = font[line[i] - 'A'];
            for (int row = 0; row < character.header.height; row++) {
                for (int col = 0; col < character.header.width; col++) {
                    Pixel pixel = character.pixels[row * character.header.width + col];
                    if (pixel.red || pixel.green || pixel.blue) {
                        int X = offset + col;
                        int Y = y + row;
                        if (X < image->header.width && Y < image->header.height) {
                            image->pixels[Y * image->header.width + X] = pixel;
                        }
                    }
                }
            }
            offset += character.header.width;
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Wrong parameters\n");
        return 1;
    }
    Image image = loadImage(argv[1]);
    if (image.pixels == NULL) {
        printf("Could not load image\n");
        return 1;
    }
    Image font[27];
    if (loadFonts(argv[3],font)) {
        return 1;
    }
    int top;
    int bottom;
    scanf("%d %d\n", &top, &bottom);
    char line[101] = "";
    for (int i = 0; i < top; i++) {
        fgets(line, sizeof(line), stdin);
        toUpperCase(line);
        drawline(&image,10 + i * 40, line,font); 
    }
    for (int i = 0; i < bottom; i++) {
        fgets(line, sizeof(line), stdin);
        toUpperCase(line);
        drawline(&image,image.header.height - (bottom - i) * 40, line, font); 
    }
    writeImage(image, argv[2]);

    free(image.pixels);
    for (int i = 0; i < 26; i++) {
        free(font[i].pixels);
    }
    return 0;
}
