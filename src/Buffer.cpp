#include "Buffer.h"
#include <string>
#include <cstring>
#include <iostream>
#include <math.h>

target::Buffer::Buffer(size_t width_, size_t height_, size_t depth_, const Color & tl_, const Color & bl_, const Color & br_, const Color & tr_) 
	: width{width_}, height{height_}, depth{depth_}, tl{tl_}, bl{bl_}, br{br_}, tr{tr_} {
	buffer = std::make_unique<component_t[]>(width * height * depth * 3);
	std::memset(buffer.get(), 0, width * height * depth * 3 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_, size_t depth_, const Color & tl_, const Color & br_) 
	: width{width_}, height{height_}, depth{depth_}, tl{tl_}, bl{tl_}, br{br_}, tr{br_} {
	buffer = std::make_unique<component_t[]>(width * height * depth * 3);
	std::memset(buffer.get(), 0, width * height * depth * 3 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_, size_t depth_, const Color & color) 
	: width{width_}, height{height_}, depth{depth_}, tl{color}, bl{color}, br{color}, tr{color} {
	buffer = std::make_unique<component_t[]>(width * height * depth * 3);
	std::memset(buffer.get(), 0, width * height * depth * 3 * sizeof(component_t));
}

target::Buffer::Buffer(size_t width_, size_t height_, size_t depth_) 
	: width{width_}, height{height_}, depth{depth_}, tl{WHITE}, bl{WHITE}, br{WHITE}, tr{WHITE} {
	buffer = std::make_unique<component_t[]>(width * height * depth * 3);
	std::memset(buffer.get(), 0, width * height * depth * 3 * sizeof(component_t));
}

void target::Buffer::fill(const Color & color, const Point3d p, const Color & borderColor){
	if(p.x >= 0 && p.x < width && p.y >= 0 && p.y < height && !(get(p) == borderColor) && !(get(p) == color)){
		pixel(p, color);
		fill(color, Point3d(p.x, p.y+1,p.z), borderColor);
		fill(color, Point3d(p.x+1, p.y,p.z), borderColor);
		fill(color, Point3d(p.x, p.y-1,p.z), borderColor);
		fill(color, Point3d(p.x-1, p.y,p.z), borderColor);
	}
}

target::Color target::Buffer::get(const target::Point3d & p) const{
	return target::Color(buffer[p.y * width + p.x + (width * height * 3 * p.z)],
	                     buffer[p.y * width + p.x + width * height + (width * height * 3 * p.z)],
	                     buffer[p.y * width + p.x + width * height * 2 + (width * height * 3 * p.z)] );
}

target::Color target::Buffer::get(const size_t & idx) const{
	return target::Color(buffer[idx],
	                     buffer[idx + width * height] ,
	                     buffer[idx + width * height * 2] );
}

void target::Buffer::pixel(const Point3d & p, const Color & color){
	if(p.x < 0 || p.x >= width || p.y < 0 || p.y >= height || p.z < 0 || p.z >= depth)
		return;
	int real_p = p.y * width + p.x;
	for(int i = 0; i < 3; ++i)
		buffer[real_p + i * width * height + (width * height * 3 * p.z)] = color[i];
}

void target::Buffer::pixels(const Point3d ps[], const size_t & p_size, const Color & color){
	for(size_t i = 0; i < p_size; ++i) 
		pixel(ps[i], color);
}