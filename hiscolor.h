#ifndef HISCOLOR_H
#define HISCOLOR_H

#include "misc.h"
#include <set>
#include <cstdio>

class hiscolor
{
	public:
		rgb color;
		int frequency;
	public:
		hiscolor(rgb color, int frequency = 1)
		{
			this->color = color;
			this->frequency = frequency;
		}

		hiscolor()
		{
			this->frequency = 0;
			color.r = color.g = color.b = 0;
		}

		rgb getcolor() const
		{
			return color;
		}

		int get() const
		{
			return frequency;
		}

		void set(int frequency)
		{
			this->frequency = frequency;
		}

	    void add()
		{
			frequency++;
		}

		void decrease()
		{
			frequency--;
		}

		friend inline bool operator<(const hiscolor &a, const hiscolor &b);
};

inline bool operator<(const hiscolor &a, const hiscolor &b)
{
	return (a.color.r < b.color.r) || ((a.color.r == b.color.r) && (a.color.g < b.color.g)) || ((a.color.r == b.color.r) 
		&& (a.color.g == b.color.g) && (a.color.b < b.color.b));
}

class weightcolor
{
	public:
		hiscolor color;

		weightcolor(hiscolor color)
		{
			this->color = color;
		}

		friend inline bool operator<(const weightcolor &a, const weightcolor &b);
};

inline bool operator<(const weightcolor &a, const weightcolor &b)
{
	return (a.color.frequency < b.color.frequency) || (a.color < b.color);
}

void get_histogram(std::set<hiscolor> &histogram, image<rgb>* im , float scale = 0.95)
{
    int width = im->width();
    int height = im->height();

    for (int y = 0; y < height; y++) 
    	for (int x = 0; x < width; x++) 
    	{
        	hiscolor color(imRef(im, x, y));
        	if(histogram.count(color) == 0)
        		histogram.insert(color);
        	else
        	{
        		std::set<hiscolor>::iterator it = histogram.find(color);
        		int frequency = it->get() + 1;
        		histogram.erase(it);
        		color.set(frequency);
        		histogram.insert(color);
        	}
    	}
    std::set<weightcolor> sorthistogram;
    for(std::set<hiscolor>::iterator it = histogram.begin(); it != histogram.end(); it++)
    {
    	sorthistogram.insert(weightcolor(*it));
    }

    int totalweight = width * height * (1 - scale);
    int currentweight = 0;

    for(std::set<weightcolor>::iterator it = sorthistogram.begin(); it != sorthistogram.end(); it++)
    {
    	currentweight += it->color.frequency;
    	std::set<hiscolor>::iterator iter = histogram.find(it->color);
    	histogram.erase(iter);
    	if(currentweight > totalweight)
    		break;
    }

    printf("color size: %d\n", histogram.size());
}

#endif