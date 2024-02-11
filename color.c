#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    GtkWidget *primary_widget;
    GtkWidget *secondary_widget;
    GtkWidget *tertiary_widget;
    GtkWidget *quaternary_widget;
} ColorWidgets;

// Function to convert HSL to RGB
void hsl_to_rgb(int h, float s, float l, float* r, float* g, float* b) {
    float c = (1 - fabs(2 * l - 1)) * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = l - c / 2;
    float r1, g1, b1;

    if (h >= 0 && h < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if (h >= 60 && h < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if (h >= 120 && h < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if (h >= 180 && h < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if (h >= 240 && h < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    *r = (r1 + m);
    *g = (g1 + m);
    *b = (b1 + m);
}

void apply_color_to_widget(GtkWidget *widget, int hue, float s, float l) {
    float r, g, b;
    // Convert HSL to RGB
    hsl_to_rgb(hue, s, l, &r, &g, &b);
    // Format the RGB color into a CSS string, targeting buttons specifically
    char *css = g_strdup_printf("button { background-color: rgb(%d,%d,%d); }", (int)(r * 255), (int)(g * 255), (int)(b * 255));
    
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_free(css);
    g_object_unref(provider);

    // Force the widget to redraw to apply the new style
    gtk_widget_queue_draw(widget);

    // Update the button label with the RGB values
    char *label = g_strdup_printf("RGB(%d, %d, %d)", (int)(r * 255), (int)(g * 255), (int)(b * 255));
    gtk_button_set_label(GTK_BUTTON(widget), label);
    g_free(label);
}

void set_related_colors_on_widget(ColorWidgets *widgets, int hue) {
    int primary_hue = hue;
    int secondary_hue = (primary_hue + 30) % 360;
    int tertiary_hue = (primary_hue + 60) % 360;
    int quaternary_hue = (primary_hue + 90) % 360;
    
    float s = (rand() % 100) / 100.0; // Random saturation between 0.0 and 1.0
    float l = 0.2 + (rand() % 60) / 100.0; // Random lightness between 0.2 and 0.8

    apply_color_to_widget(widgets->primary_widget, primary_hue, s, l);
    apply_color_to_widget(widgets->secondary_widget, secondary_hue, s, l);
    apply_color_to_widget(widgets->tertiary_widget, tertiary_hue, s, l);
    apply_color_to_widget(widgets->quaternary_widget, quaternary_hue, s, l);
}

void on_all_colors_button_clicked(GtkWidget *widget, ColorWidgets *widgets) {
    int hue = rand() % 360;
    set_related_colors_on_widget(widgets, hue);
}

void on_widget_clicked(GtkWidget *widget, ColorWidgets *widgets) {
    float s = (rand() % 100) / 100.0; // Random saturation between 0.0 and 1.0
    float l = 0.2 + (rand() % 60) / 100.0; // Random lightness between 0.2 and 0.8
    int hue = rand() % 360;

    apply_color_to_widget(widget, hue, s, l);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    srand(time(NULL)); // Seed the random number generator
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Color Palette Maker");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);
    
    GtkWidget *primary_widget = gtk_button_new_with_label("Primary Color");
    GtkWidget *secondary_widget = gtk_button_new_with_label("Secondary Color");
    GtkWidget *tertiary_widget = gtk_button_new_with_label("Tertiary Color");
    GtkWidget *quaternary_widget = gtk_button_new_with_label("Quaternary Color");
    GtkWidget *all_colors_button = gtk_button_new_with_label("Change All Colors");
    
    gtk_box_pack_start(GTK_BOX(box), primary_widget, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), secondary_widget, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), tertiary_widget, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), quaternary_widget, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), all_colors_button, TRUE, TRUE, 0);
    
    ColorWidgets *widgets = g_new0(ColorWidgets, 1);
    widgets->primary_widget = primary_widget;
    widgets->secondary_widget = secondary_widget;
    widgets->tertiary_widget = tertiary_widget;
    widgets->quaternary_widget = quaternary_widget;

    g_signal_connect(primary_widget, "clicked", G_CALLBACK(on_widget_clicked), widgets);
    g_signal_connect(secondary_widget, "clicked", G_CALLBACK(on_widget_clicked), widgets);
    g_signal_connect(tertiary_widget, "clicked", G_CALLBACK(on_widget_clicked), widgets);
    g_signal_connect(quaternary_widget, "clicked", G_CALLBACK(on_widget_clicked), widgets);
    g_signal_connect(all_colors_button, "clicked", G_CALLBACK(on_all_colors_button_clicked), widgets);

    gtk_widget_show_all(window);
    gtk_main();

    g_free(widgets);
    return 0;
}