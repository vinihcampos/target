#ifndef _BUFFER_
#define _BUFFER_

#include <memory>
#include <string>
#include "Vec3.h"
#include "Point2.h"

namespace target{
	
	/**
	*	Type of a color component.
	*/
	typedef unsigned char component_t;

	/**
	* Provides methods to draw an image.
	* 
	* @author Vinícius Campos
	* @date 3/12/2019
	*/
	class Buffer{
		private:			
			size_t width, height;
			std::unique_ptr<component_t[]> buffer;

			Point2 Q11,Q12,Q21,Q22;

			Color tl, tr;
			Color bl, br;

		public:
			/**
			*	Creates an empty buffer.
			*	@param width_ The width of buffer.
			*	@param height_ The height of buffer.
			*/
			Buffer(size_t width_, size_t height_, const Color & tl_, const Color & bl_, const Color & br_, const Color & tr_);

			/**
			*	Creates an empty buffer.
			*	@param width_ The width of buffer.
			*	@param height_ The height of buffer.
			*/
			Buffer(size_t width_, size_t height_, const Color & tl_, const Color & br_);

			/**
			*	Creates an empty buffer.
			*	@param width_ The width of buffer.
			*	@param height_ The height of buffer.
			*/
			Buffer(size_t width_, size_t height_, const Color & color);

			/**
			*	Creates an empty buffer.
			*	@param width_ The width of buffer.
			*	@param height_ The height of buffer.
			*/
			Buffer(size_t width_, size_t height_);

			/**
			*	Creates an empty buffer.
			*/
			Buffer(){}
			
			/**
			*	Method to get the width of buffer. 
			*	@return The width of buffer.
			*/
			size_t getWidth() const { return width; }
			
			/**
			*	Method to get the height of buffer.
			*	@return The height of buffer.
			*/
			size_t getHeight() const { return height; }

			/**
			*	Method to get the color of top left point.
			*	@return The color of top left point.
			*/
			Color getTl() const { return tl; }
			
			void setTl(const Color & tl_) { tl = tl_; }

			/**
			*	Method to get the color of top bottom point.
			*	@return The color of bottom left point.
			*/
			Color getBl() const { return bl; }
			
			void setBl(const Color & bl_) { bl = bl_; }

			/**
			*	Method to get the color of top right point.
			*	@return The color of top right point.
			*/
			Color getTr() const { return tr; }
			
			void setTr(const Color & tr_) { tr = tr_; }

			/**
			*	Method to get the color of bottom right point.
			*	@return The color of bottom right point.
			*/
			Color getBr() const { return br; }
			
			void setBr(const Color & br_) { br = br_; }

			/**
			*	Method to get the color of a pixel.
			*	@param p A point of buffer.
			*	@return The color of point p.
			*/
			Color get(const Point2 & p) const;

			/**
			*	Method to get the color of a pixel.
			*	@param idx An index of buffer array.
			*	@return The color of index array idx.
			*/
			Color get(const size_t & idx) const;

			/**
			*	Method to fill the background from a point with a specific color using flood fill algorithm.
			*	@param color The color to paint the background.
			*	@param p The start point of painting.
			*	@param borderColor The limit color of algorithm.
			*/
			void fill(const Color & color, const Point2 p = Point2(0,0), const Color & borderColor = BLACK);
			
			/**
			* 	Method to paint a pixel.
			*	@param p A point of buffer.
			*	@param color The color to paint the pixel.
			*/
			void pixel(const Point2 & p, const Color & color);
			
			/**
			* 	Method to paint a list of pixels.
			*	@param ps The list of points.
			*	@param p_size The size of list ps.
			*	@param color The color to paint the list of pixels.
			*/
			void pixels(const Point2 ps[], const size_t & p_size, const Color & color);
	};

}

#endif