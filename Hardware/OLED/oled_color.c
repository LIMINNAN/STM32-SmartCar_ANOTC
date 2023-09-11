#include "oled_color.h"



static Type_color _Draw=pix_white;
static Type_color _fill=pix_white;


void SetDrawColor(Type_color value)
{
	_Draw=value;
}

Type_color GetDrawColor(void)
{
	return _Draw;
}

void SetFillcolor(Type_color value)
{
	_fill=value;

}

Type_color GetFillColor(void)
{

	return _fill;
}


