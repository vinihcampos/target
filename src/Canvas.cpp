#include "Canvas.h"
#include <string>
#include <cstring>
#include <iostream>
#include <math.h>

target::Canvas::Canvas(size_t width_, size_t height_, const Color & tl_, const Color & bl_, const Color & br_, const Color & tr_) 
	: width{width_}, height{height_}, tl{tl_}, bl{bl_}, br{br_}, tr{tr_} {
	canvas = std::make_unique<component_t[]>(width * height * 3);
	std::memset(canvas.get(), 0, width * height * 3 * sizeof(component_t));

	for(size_t row = 0; row < height; ++row){
		for(size_t col = 0; col < width; ++col){
			pixel( Point2d(row, col), interpolate( Point2d(row, col) ) );
		}
	}

}

target::Canvas::Canvas(size_t width_, size_t height_, const Color & tl_, const Color & br_) 
	: width{width_}, height{height_}, tl{tl_}, bl{tl_}, br{br_}, tr{br_} {
	canvas = std::make_unique<component_t[]>(width * height * 3);
	std::memset(canvas.get(), 0, width * height * 3 * sizeof(component_t));

	for(size_t row = 0; row < height; ++row){
		for(size_t col = 0; col < width; ++col){
			pixel( Point2d(row, col), interpolate( Point2d(row, col) ) );
		}
	}
}

target::Canvas::Canvas(size_t width_, size_t height_, const Color & color) 
	: width{width_}, height{height_}, tl{color}, bl{color}, br{color}, tr{color} {
	canvas = std::make_unique<component_t[]>(width * height * 3);
	std::memset(canvas.get(), 0, width * height * 3 * sizeof(component_t));

	for(size_t row = 0; row < height; ++row){
		for(size_t col = 0; col < width; ++col){
			pixel( Point2d(row, col), interpolate( Point2d(row, col) ) );
		}
	}

}

void target::Canvas::fill(const Color & color, const Point2d p, const Color & borderColor){
	if(p.x >= 0 && p.x < width && p.y >= 0 && p.y < height && !(get(p) == borderColor) && !(get(p) == color)){
		pixel(p, color);
		fill(color, Point2d(p.x, p.y+1), borderColor);
		fill(color, Point2d(p.x+1, p.y), borderColor);
		fill(color, Point2d(p.x, p.y-1), borderColor);
		fill(color, Point2d(p.x-1, p.y), borderColor);
	}
}

target::Color target::Canvas::get(const target::Point2d & p) const{
	return target::Color( canvas[p.y * width + p.x],
	                     canvas[p.y * width + p.x + width * height],
	                     canvas[p.y * width + p.x + width * height * 2] );
}

target::Color target::Canvas::get(const size_t & idx) const{
	return target::Color( canvas[idx],
	                     canvas[idx + width * height] ,
	                     canvas[idx + width * height * 2] );
}

void target::Canvas::pixel(const Point2d & p, const Color & color){
	if(p.x < 0 || p.x >= width || p.y < 0 || p.y >= height )
		return;
	int real_p = p.y * width + p.x;
	for(int i = 0; i < 3; ++i)
		canvas[real_p + i * width * height] = color[i];
}

void target::Canvas::pixels(const Point2d ps[], const size_t & p_size, const Color & color){
	for(size_t i = 0; i < p_size; ++i) 
		pixel(ps[i], color);
}

target::Color target::Canvas::interpolate(const Point2d p){

	int x1 = 0;
	int x2 = width-1;
	int y1 = 0;
	int y2 = height-1;

	float ratio = 1.0 / ( (x2-x1) * (y2-y1) * 1.0);

	int productRed = bl.r*(x2-p.x)*(y2-p.y) + br.r*(p.x-x1)*(y2-p.y) + tl.r*(x2-p.x)*(p.y-y1) + tr.r*(p.x-x1)*(p.y-y1);
	int productGreen = bl.g*(x2-p.x)*(y2-p.y) + br.g*(p.x-x1)*(y2-p.y) + tl.g*(x2-p.x)*(p.y-y1) + tr.g*(p.x-x1)*(p.y-y1);
	int productBlue = bl.b*(x2-p.x)*(y2-p.y) + br.b*(p.x-x1)*(y2-p.y) + tl.b*(x2-p.x)*(p.y-y1) + tr.b*(p.x-x1)*(p.y-y1);

	return Color(ratio*productRed, ratio*productGreen, ratio*productBlue);
}