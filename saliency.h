#ifndef SALIENCY_H
#define SALIENCY_H

#include <cstdlib>
#include <cstdio>
#include "image.h"
#include "misc.h"
#include "filter.h"


// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b, int x1, int y1, int x2, int y2) 
{
  return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
          square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
          square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}


image<rgb>* saliency(image<rgb>* im)
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
    
    for(int i = 0;i < total; i++)
    {
        int xfix = i / height;
        int yfix = i - xfix * height;
        for(int j = 0; j < total; j++)
        {
            int xcursor = j / height;
            int ycursor = j - xcursor * height;
            imRef(weight, xfix, yfix) += diff(r,g,b,xfix,yfix,xcursor,ycursor);
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