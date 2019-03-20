#include "Canvas.h"
#include <string>
#include <cstring>
#include <iostream>
#include <math.h>

target::Canvas::Canvas(size_t width_, size_t height_) : width{width_}, height{height_} {
	canvas = std::make_unique<component_t[]>(width * height * 3);
	std::memset(canvas.get(), 0, width * height * 3 * sizeof(component_t));
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