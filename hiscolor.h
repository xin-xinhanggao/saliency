#ifndef HISCOLOR_H
#define HISCOLOR_H

#include "misc.h"

class hiscolor
{
	private:
		rgb color;
		int frequency;
	public:
		hiscolor(rgb color)
		{
			this->color = color;
			frequency = 0;
		}

		void add()
		{
			frequency++;
		}

		void decrease()
		{
			frequency--;
		}

		friend inline bool operator==(const hiscolor &a, const hiscolor &b);
		friend inline bool operator<(const hiscolor &a, const hiscolor &b);
		friend inline bool operator>(const hiscolor &a, const hiscolor &b);
};

inline bool operator==(const hiscolor &a, const hiscolor &b) 
{
  return (a.color == b.color);
}

inline bool operator<(const hiscolor &a, const hiscolor &b)
{
	return (a.frequency < b.frequency);
}

inline bool operator>(const hiscolor &a, const hiscolor &b)
{
	return (a.frequency > b.frequency);
}
#endif