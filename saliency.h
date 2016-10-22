#ifndef SALIENCY_H
#define SALIENCY_H

#include <cstdlib>
#include <cstdio>
#include <set>
#include "image.h"
#include "misc.h"
#include "hiscolor.h"
#include "segment-image.h"
#include "filter.h"



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

    printf("%d\n", total);
    printf("%d\n", histogram.size());

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

image<rgb>* gc_saliency(image<rgb>* im)
{
    universe* u = segment_image(im, 0.8, 440, 40);
    printf("complete segment_image piece %d\n", u->num_sets());
    int width = im->width();
    int height = im->height();
    int total = width * height;
    float *weight = new float[total];
    float maximum = 0.0;

    std::set<hiscolor> *histogram = new std::set<hiscolor> [u->num_sets()];
    uni_elt *elt[u->num_sets()];

    int iternum = 0;
    for(std::set<int>::iterator iter = u->kset.begin();iter != u->kset.end(); iter++)
    {
        elt[iternum] = u->findelt(*iter);
        get_histogram(histogram[iternum], elt[iternum]->clist, im);
        iternum++;
    }

    float **Dr = new float*[iternum];
    float **Ds = new float*[iternum];
    for(int i = 0; i < iternum;i++)
    {
        Dr[i] = new float[iternum];
        Ds[i] = new float[iternum];
    }

    for(int i = 0; i < iternum;i++)
    {
        for(int j = i + 1;j < iternum;j++)
        {
            Ds[i][j] = exp(sqrt(square(elt[i]->x - elt[j]->x) + square(elt[i]->y - elt[j]->y)) / (-0.4));
            Dr[i][j] = 0.0;
            for(std::set<hiscolor>::iterator c1 = histogram[i].begin();c1 != histogram[i].end(); c1++)
                for(std::set<hiscolor>::iterator c2 = histogram[j].begin();c2 != histogram[j].end(); c2++)
                {    
                    rgb color1 = c1->getcolor();
                    rgb color2 = c2->getcolor();
                    float cdiff = diff((float)color1.r, (float)color1.g, (float)color1.b, (float)color2.r, (float)color2.g, (float)color2.b);
                    Dr[i][j] += c1->get() * c2->get() * cdiff;  
                }
            Ds[j][i] = Ds[i][j];
            Dr[j][i] = Dr[i][j];
        }
    }
    
    for(int i = 0;i < iternum;i++)
    {
        int focus = elt[i]->p;
        weight[focus] = 0;
        for(int j = 0;j < iternum;j++)
        {
            if(i != j)
                weight[focus] += Ds[i][j] * Dr[i][j] * elt[j]->size;
        }
        weight[focus] *= exp(-9 * (square(elt[i]->x - 0.5) + square(elt[i]->y - 0.5)));

        if(weight[focus] > maximum)
            maximum = weight[focus];
    }

    image<rgb> *output = new image<rgb>(width, height);
    for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
    {
        int picidx = y * width + x;
        int comp = u->find(picidx);
        imRef(output,x,y).r = (uchar)(int)(weight[comp] / maximum * 255);
        imRef(output,x,y).g = (uchar)(int)(weight[comp] / maximum * 255);
        imRef(output,x,y).b = (uchar)(int)(weight[comp] / maximum * 255);
    }

    return output;
}

#endif