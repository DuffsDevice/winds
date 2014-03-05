/*
 * Copyright 2004 Richard Wilson <richard.wilson@netsurf-browser.org>
 * Copyright 2008 Sean Fox <dyntryx@gmail.com>
 *
 * This file is part of NetSurf's libnsgif, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 */

/** \file
 * Progressive animated GIF file decoding (interface).
 */

#ifndef _LIBNSGIF_H_
#define _LIBNSGIF_H_

#include <stdbool.h>
#include <inttypes.h>

/*	Error return values
*/
enum gif_result : int {
	GIF_WORKING = 1,
	GIF_OK = 0,
	GIF_INSUFFICIENT_FRAME_DATA = -1,
	GIF_FRAME_DATA_ERROR = -2,
	GIF_INSUFFICIENT_DATA = -3,
	GIF_DATA_ERROR = -4,
	GIF_INSUFFICIENT_MEMORY = -5,
	GIF_FRAME_NO_DISPLAY = -6,
	GIF_END_OF_FRAME = -7
};

/*	The GIF frame data
*/
struct gif_frame {
  	bool display;				/**< whether the frame should be displayed/animated */
  	unsigned int frame_delay;		/**< delay (in cs) before animating the frame */
	/**	Internal members are listed below
	*/
  	unsigned int frame_pointer;		/**< offset (in bytes) to the GIF frame data */
  	bool virgin;				/**< whether the frame has previously been used */
	bool opaque;				/**< whether the frame is totally opaque */
	bool redraw_required;			/**< whether a forcable screen redraw is required */
	unsigned char disposal_method;		/**< how the previous frame should be disposed; affects plotting */
	bool transparency;	 		/**< whether we acknoledge transparency */
	unsigned char transparency_index;	/**< the index designating a transparent pixel */
	unsigned int redraw_x;			/**< x co-ordinate of redraw rectangle */
	unsigned int redraw_y;			/**< y co-ordinate of redraw rectangle */
	unsigned int redraw_width;		/**< width of redraw rectangle */
	unsigned int redraw_height;		/**< height of redraw rectangle */
};

/*	The GIF animation data
*/
struct gif_animation {
	unsigned char *gif_data;			/**< pointer to GIF data */
	unsigned int width;				/**< width of GIF (may increase during decoding) */
	unsigned int height;				/**< heigth of GIF (may increase during decoding) */
	unsigned int frame_count;			/**< number of frames decoded */
	unsigned int frame_count_partial;		/**< number of frames partially decoded */
	gif_frame* frames;				/**< decoded frames */
	int decoded_frame;				/**< current frame decoded to bitmap */
	void* frame_image;				/**< currently decoded image; stored as bitmap from bitmap_create callback */
	int loop_count;					/**< number of times to loop animation */
	gif_result current_error;			/**< current error type, or 0 for none*/
	/**	Internal members are listed below
	*/
	unsigned int buffer_position;			/**< current index into GIF data */
	unsigned int buffer_size;			/**< total number of bytes of GIF data available */
	unsigned int frame_holders;			/**< current number of frame holders */
	unsigned int background_index;			/**< index in the colour table for the background colour */
	unsigned int aspect_ratio;			/**< image aspect ratio (ignored) */
	unsigned int colour_table_size;		/**< size of colour table (in entries) */
	bool global_colours;				/**< whether the GIF has a global colour table */
	unsigned int* global_colour_table;		/**< global colour table */
	unsigned int* local_colour_table;		/**< local colour table */
};

void gif_create(gif_animation *gif);
gif_result gif_initialise(gif_animation *gif, size_t size, unsigned char *data);
gif_result gif_decode_frame(gif_animation *gif, unsigned int frame);
void gif_finalise(gif_animation *gif);

#endif
