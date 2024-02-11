# PaletteGen

PaletteGen is a C application using GTK to generate a harmonious color palette. It displays a window with four buttons, each representing a color. A fifth button changes all colors simultaneously.

## How it works

Colors are generated using the HSL (Hue, Saturation, Lightness) model. When a color is changed, a new hue, saturation, and lightness are randomly generated within specific ranges to ensure the colors are not too bright or too dark.

When a single color is changed, the new color is analogous to the existing colors, creating a harmonious palette. Clicking the "Change All Colors" button generates a new palette with all colors being analogous.

## Running the program

Ensure you have GTK+ 3.0 or later installed on your system. Compile the program using the following:

`gcc 'pkg-config --cflags gtk+-3.0' -o color color.c 'pkg-config --libs gtk+-3.0' -lm`
