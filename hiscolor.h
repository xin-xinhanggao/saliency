#ifndef HISCOLOR_H
#define HISCOLOR_H

#include "misc.h"
#include <set>

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

void get_histogram(std::set<hiscolor> &histogram, image<rgb>* im)
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
}

#endif