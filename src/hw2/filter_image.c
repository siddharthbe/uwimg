#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;
    for (int i = 0; i < im.c * im.h * im.w; i++) {
        sum += im.data[i];
    }
    for (int i = 0; i < im.c * im.h * im.w; i++) {
        im.data[i] /= sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    image box_filter = make_image(w,w,1);
    for (int i = 0; i < w * w; i++) {
        box_filter.data[i] = 1;
    }
    l1_normalize(box_filter);
    return box_filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == im.c || filter.c == 1);
    image output = make_image(im.w, im.h, preserve ? im.c : 1);
    int h_shift = filter.h / 2;
    int w_shift = filter.w / 2;
    for (int i = 0 - w_shift; i < im.w - w_shift; i++) {
        for (int j = 0 - h_shift; j < im.h - h_shift; j++) {
	    for (int k = 0; k < im.c; k++) {
		float v = preserve ? 0.0 : get_pixel(output, i + w_shift, j + h_shift, 0);
		for (int i_filter = 0; i_filter < filter.w; i_filter++) {
		    for (int j_filter = 0; j_filter < filter.h; j_filter++) {
		        v += get_pixel(im, i + i_filter, j + j_filter, k) * get_pixel(filter, i_filter, j_filter, filter.c == 1 ? 0 : k);
		    }
		}
		set_pixel(output, i + w_shift, j + h_shift, preserve ? k : 0, v);
	    }
	}
    }
    return output;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    float highpass[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    memcpy(filter.data, highpass, sizeof(highpass));
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    float sharpen[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    memcpy(filter.data, sharpen, sizeof(sharpen));
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    float emboss[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
    memcpy(filter.data, emboss, sizeof(emboss));
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO
// Since highpass finds edges no matter the color we will set preserve to 0. We will set preserve to 1 for emboss and sharpen
// if we want to preserve the color.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO
// We should post process all filters to be sure that the colors don't go outside the range. We can do this using strategies
// such as clamping.

image make_gaussian_filter(float sigma)
{
    // TODO
    float filter_size = 6 * sigma;
    filter_size = ((int) filter_size % 2 == 0) ? (filter_size += 1.0) : (filter_size);
    image filter = make_image(filter_size, filter_size, 1);
    int shift = filter_size / 2;
    for (int i = 0 - shift; i < filter_size - shift; i++) {
        for (int j = 0 - shift; j < filter_size - shift; j++) {
	    float v = 1 / (TWOPI * sigma * sigma) * expf(-(i * i + j * j) / (2 * sigma * sigma));
	    set_pixel(filter, i + shift, j + shift, 0, v);
	}
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image sum_image = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        sum_image.data[i] = a.data[i] + b.data[i];
    }
    return sum_image;
}

image sub_image(image a, image b)
{
    // TODO 
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image diff_image = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        diff_image.data[i] = a.data[i] - b.data[i];
    }
    return diff_image;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    float gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    memcpy(filter.data, gx, sizeof(gx));
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    float gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    memcpy(filter.data, gy, sizeof(gy));
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min = INFINITY;
    float max = INFINITY;
    for (int i = 0; i < im.c * im.h * im.w; i++) {
        max = im.data[i] > max ? im.data[i] : max;
        min = im.data[i] < min ? im.data[i] : min;
    }
    float range = max - min;
    for (int i = 0; i < im.c * im.h * im.w; i++) {
        im.data[i] = (range == 0) ? ((im.data[i] - min) / range) : 0; 
    }
}

image *sobel_image(image im)
{
    // TODO
    image *output = calloc(2, sizeof(image));
    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image gx_img = convolve_image(im, gx, 0);
    image gy_img = convolve_image(im, gy, 0);
    image magnitude = make_image(gx_img.w, gx_img.h, 1);
    image theta = make_image(gx_img.w, gx_img.h, 1);
    for (int i = 0; i < im.h * im.w; i++) {
        magnitude.data[i] = sqrtf(gx_img.data[i] * gx_img.data[i] + gy_img.data[i] * gy_img.data[i]);
       	theta.data[i] = atan2f(gy_img.data[i],gx_img.data[i]);
    }
    free_image(gx);
    free_image(gy);
    free_image(gx_img);
    free_image(gy_img);
    output[0] = magnitude;
    output[1] = theta;
    return output;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel = sobel_image(im);
    image magnitude = sobel[0];
    image theta = sobel[1];
    feature_normalize(magnitude);
    feature_normalize(theta);
    image color_sobel = make_image(im.w, im.h, 3);
    memcpy(color_sobel.data, magnitude.data, sizeof(float) * im.h * im.w);
    memcpy(color_sobel.data + im.h * im.w, theta.data, sizeof(float) * im.h * im.w);
    memcpy(color_sobel.data + 2 * im.h * im.w, theta.data, sizeof(float) * im.h * im.w);
    hsv_to_rgb(color_sobel);
    free_image(magnitude);
    free_image(theta);
    return color_sobel;
}
