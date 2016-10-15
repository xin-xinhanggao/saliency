#ifndef SALIENCY_H
#define SALIENCY_H

#include <cstdlib>
#include <cstdio>
#include <set>
#include "image.h"
#include "misc.h"
#include "hiscolor.h"
#include "filter.h"


// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b, int x1, int y1, int x2, int y2) 
{
  return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
          square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
          square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}

static inline float diff(float r1, float g1, float b1, float r2, float g2, float b2)
{
  return sqrt(square(r1 - r2) + square(g1 - g2) + square(b1 - b2));
}

image<rgb>* naive_saliency(image<rgb>* im)
{
    int width = im->width();
    int height = im->height();
    int total = width * height;


    image<float> *r = new image<float>(width, height);
    image<float> *g = new image<float>(width, height);
    image<float> *b = new image<float>(width, height);

    for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(r, x, y) = imRef(im, x, y).r;
      imRef(g, x, y) = imRef(im, x, y).g;
      imRef(b, x, y) = imRef(im, x, y).b;
        }
    }

    image<rgb> *output = new image<rgb>(width, height);
    image<float> *weight = new image<float>(width, height);
    for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        imRef(weight, x, y) = 0.0;
    }
    }

    float maximum = 0.0;

    printf("width %d\n",width);
    printf("height %d\n",height);
    
    std::set<hiscolor> histogram;
    get_histogram(histogram, im);

    for(int i = 0;i < total; i++)
    {
        int xfix = i / height;
        int yfix = i - xfix * height;
        for(std::set<hiscolor>::iterator it = histogram.begin();it != histogram.end();it++)
        {
            imRef(weight, xfix, yfix) += it->get() * diff((float)it->getcolor().r, (float)it->getcolor().g, (float)it->getcolor().b, imRef(r,xfix,yfix), imRef(g,xfix,yfix), imRef(b,xfix,yfix));
        }

        if(maximum < imRef(weight, xfix, yfix))
            maximum = imRef(weight, xfix, yfix);
    }
    
    for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
    {
        imRef(output,x,y).r = (uchar)(int)(imRef(weight,x,y) / maximum * 255);
        imRef(output,x,y).g = (uchar)(int)(imRef(weight,x,y) / maximum * 255);
        imRef(output,x,y).b = (uchar)(int)(imRef(weight,x,y) / maximum * 255);
    }

    return output;
}

#endif