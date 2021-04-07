#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int new_x = x;
    int new_y = y;
    int new_c = c;
    if (x < 0) {
    	new_x = 0;
    }
    if (y < 0) {
    	new_y = 0;
    }
    if (c < 0) {
    	new_c = 0;
    }
    if (x >= im.w) {
    	new_x = (im.w - 1);
    }
    if (y >= im.h) {
    	new_y = (im.h - 1);
    }
    if (c >= im.c) {
    	new_c = (im.c - 1);
    }
    return im.data[new_c * im.w * im.h + new_y * im.w + new_x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x >= 0 && y >= 0 && c >= 0 && x < im.w && y < im.h && c < im.c) {
        im.data[c * im.w * im.h + y * im.w + x] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w; i++) {
        gray.data[i] = 0.299 * im.data[i] + 0.587 * im.data[im.h * im.w + i] + 0.114 * im.data[2 * im.h * im.w + i];
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w; i++) {
        im.data[c * im.h * im.w + i] += v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w * im.c; i++) {
        if (im.data[i] < 0) {
	    im.data[i] = 0;
	}
	if (im.data[i] > 1) {
            im.data[i] = 1;
	}
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w; i++) {
        float r = im.data[i];
	float g = im.data[i + im.h * im.w];
	float b = im.data[i + 2 * im.h * im.w];
        float v = three_way_max(r, g, b);
        float m = three_way_min(r, g, b);
        float c = v - m;
        float s;
	if (v == 0) {
	    s = 0;
	} else {
	    s = c / v;
	}
	float h = 0;
	if (c != 0) {
	    if (v == r) {
	        h = (g - b) / c;
	    }
	    if (v == g) {
	        h = ((b - r) / c) + 2;
	    }
	    if (v == b) {
	        h = ((r - g) / c) + 4;
	    }
	}
	if (h < 0) {
	    h = (h / 6) + 1;
	} else {
	    h /= 6;
	}	
        im.data[i] = h;
        im.data[i + im.h * im.w] = s;
        im.data[i + 2 * im.h * im.w] = v;	
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w; i++) {
        float h = im.data[i];
	float s = im.data[i + im.w * im.h];
	float v = im.data[i + 2 * im.w * im.h];
	float c = s * v;
	float m = v - c;
	float h_prime = h * 6;
	float r, b, g;
	if (c == 0) {
	    r = v;
	    g = v;
	    b = v;
	}
	else if (h_prime > 5 && h_prime < 6) {
	    r = v;
	    g = m;
	    b = (((((h_prime / 6) - 1) * 6) * c) - g) * -1;
	}
	else if (h_prime == 5) {
	    r = v;
	    g = m;
	    b = v;
	}
	else if (h_prime < 5 && h_prime > 4) {
	    g = m;
	    r = (h_prime - 4) * c + g;
	    b = v;
	}
	else if (h_prime == 4) {
	    r = m;
	    g = m;
	    b = v;
	}
	else if (h_prime < 4 && h_prime > 3) {
	    r = m;
	    g = (((h_prime - 4) * c) - r) * -1;
            b = v;
	}
	else if (h_prime == 3) {
	    r = m;
	    g = v;
	    b = v;
	}
	else if (h_prime < 3 && h_prime > 2) {
	    r = m;
	    g = v;
	    b = ((h_prime - 2) * c) + r;
	}
	else if (h_prime == 2) {
	    r = m;
	    g = v;
	    b = m;
	}
	else if (h_prime < 2 && h_prime > 1) {
	    g = v;
	    b = m;
	    r = (((h_prime - 2) * c) - b) * -1;
	}
	else if (h_prime == 1) {
	    r = v;
	    g = v;
	    b = m;
	}
	else if (h_prime < 1 && h_prime > 0) {
	    r = v;
	    b = m;
	    g = (h_prime * c) + b;
	}
	else {
	    r = v;
	    g = m;
	    b = m;
	}
	im.data[i] = r;
	im.data[i + im.w * im.h] = g;
	im.data[i + 2 * im.w * im.h] = b;

    }
}
