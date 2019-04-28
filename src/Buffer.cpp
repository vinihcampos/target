#include "Buffer.h"
#include <string>
#include <cstring>
#include <iostream>
#include <math.h>

target::Buffer::Buffer(size_t width_, size_t height_, const Color & tl_, const Color & bl_, const Color & br_, const Color & tr_) 
	: width{width_}, height{height_}, tl{tl_}, bl{bl_}, br{br_}, tr{tr_} {
	buffer = std::make_unique<component_t[]>(width * height * 4);
	std::memset(buffer.get(), 0, width * height * 4 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_, const Color & tl_, const Color & br_) 
	: width{width_}, height{height_}, tl{tl_}, bl{tl_}, br{br_}, tr{br_} {
	buffer = std::make_unique<component_t[]>(width * height * 4);
	std::memset(buffer.get(), 0, width * height * 4 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_, const Color & color) 
	: width{width_}, height{height_}, tl{color}, bl{color}, br{color}, tr{color} {
	buffer = std::make_unique<component_t[]>(width * height * 4);
	std::memset(buffer.get(), 0, width * height * 4 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_) 
	: width{width_}, height{height_}, tl{WHITE}, bl{WHITE}, br{WHITE}, tr{WHITE} {
	buffer = std::make_unique<component_t[]>(width * height * 4);
	std::memset(buffer.get(), 0, width * height * 4 * sizeof(component_t));
}

void target::Buffer::fill(const Color & color, const Point2 p, const Color & borderColor){
	if(p.x >= 0 && p.x < width && p.y >= 0 && p.y < height && !(get(p) == borderColor) && !(get(p) == color)){
		pixel(p, color);
		fill(color, Point2(p.x, p.y+1), borderColor);
		fill(color, Point2(p.x+1, p.y), borderColor);
		fill(color, Point2(p.x, p.y-1), borderColor);
		fill(color, Point2(p.x-1, p.y), borderColor);
	}
}

target::Color target::Buffer::get(const target::Point2 & p) const{
	return target::Color(buffer[p.y * width + p.x],
	                     buffer[p.y * width + p.x + width * height],
	                     buffer[p.y * width + p.x + width * height * 2] );
}

target::Color target::Buffer::get(const size_t & idx) const{
	return target::Color(buffer[idx],
	                     buffer[idx + width * height] ,
	                     buffer[idx + width * height * 2] );
}

void target::Buffer::pixel(const Point2 & p, const Color & color){
	if(p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
		return;
	int real_p = p.y * width + p.x;
	for(int i = 0; i < 3; ++i)
		buffer[real_p + i * width * height] = color[i];
}

void target::Buffer::pixels(const Point2 ps[], const size_t & p_size, const Color & color){
	for(size_t i = 0; i < p_size; ++i) 
		pixel(ps[i], color);
}