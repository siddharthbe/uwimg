#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image resized_image = make_image(w, h, im.c);
    float a_w = (float) im.w / w;
    float b_w = 0.5 * a_w - 0.5;
    float a_h = (float) im.h / h;
    float b_h = 0.5 * a_h - 0.5;
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < h; j++) {
	    for (int k = 0; k < w; k++) {
	        resized_image.data[i * w * h + j * w + k] = nn_interpolate(im, a_w * k + b_w, a_h * j + b_h, i);
	    }
	}
    }
    return resized_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float v1 = get_pixel(im, floorf(x), floorf(y), c);
    float v2 = get_pixel(im, ceilf(x), floorf(y), c);
    float v3 = get_pixel(im, floorf(x), ceilf(y), c);
    float v4 = get_pixel(im, ceilf(x), ceilf(y), c);
    float d1 = x - floorf(x);
    float d2 = ceilf(x) - x;
    float d3 = y - floorf(y);
    float d4 = ceilf(y) - y;
    float q1 = v1 * d2 + v2 * d1;
    float q2 = v3 * d2 + v4 * d1;
    return q1 * d4 + q2 * d3;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image resized_image = make_image(w, h, im.c);
    float a_w = (float) im.w / w;
    float b_w = 0.5 * a_w - 0.5;
    float a_h = (float) im.h / h;
    float b_h = 0.5 * a_h - 0.5;
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < h; j++) {
	    for (int k = 0; k < w; k++) {
	        resized_image.data[i * w * h + j * w + k] = bilinear_interpolate(im, a_w * k + b_w, a_h * j + b_h, i);
	    }
	}
    }
    return resized_image;
}

