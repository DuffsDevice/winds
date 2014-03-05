/* ======================================================================== */
/*  GIF Encoder routines.                                                   */
/*                                                                          */
/*  These are intended to support single-frame and multi-frame GIFs.        */
/*  Single frame support is for screen shots, multi-frame support is for    */
/*  converting movies to animated GIFs.                                     */
/*                                                                          */
/*  This GIF encoder doesn't trust that the decoder honors the aspect       */
/*  ratio stored in the GIF.  We just set it to 0.                          */
/*                                                                          */
/*  None of this code's thread-safe/reentrant.  BFD.                        */
/* ======================================================================== */
#ifndef GIF_ENC_H_
#define GIF_ENC_H_

#include <stdint.h>
#include <stdio.h>
#include <_type/type.h>

typedef struct gif_t
{
    FILE    *f;
    int     x_dim, y_dim;
    int     trans, n_cols;
    uint8_t *vid, *pal;
} gif_t;

static unused int gif_best_stat[6];

/* ======================================================================== */
/*  GIF_START -- Starts a single or multi-frame GIF.                        */
/* ======================================================================== */
int gif_start
(
    gif_t   *gif, 
    FILE    *f,             /* file to attach to GIF.                       */
    int     x_dim,          /* source image X dimension                     */
    int     y_dim,          /* source image Y dimension                     */
    uint8_t *pal,           /* palette to use for GIF.                      */
    int     n_cols,         /* number of colors in GIF.                     */
	int		trans,          /* Index of transparent Color in image          */
    int     multi           /* 0: Single image, 1: Multiple image           */
);

/* ======================================================================== */
/*  GIF_FINISH -- Finishes off a GIF, terminating it and freeing memory.    */
/* ======================================================================== */
int gif_finish(gif_t *gif);


/* ======================================================================== */
/*  GIF_WR_FRAME_S -- Writes single-frame image to GIF.                     */
/* ======================================================================== */
int gif_wr_frame_s
(
    gif_t   *gif,
    uint8_t *vid
);

/* ======================================================================== */
/*  GIF_WRITE       -- Wrapper around gif_start/gif_wr_frame_s.             */
/* ======================================================================== */
int gif_write
(
    FILE    *f,
    uint8_t *vid,
    int     x_dim,
    int     y_dim,
    uint8_t *pal,
    int     n_cols,
	int		trans
);


/* ======================================================================== */
/*  GIF_WR_FRAME_M -- Writes next frame to a multi-frame GIF.               */
/*                    Attempts to optimize image.                           */
/* ======================================================================== */
int gif_wr_frame_m
(
    gif_t   *gif,
    uint8_t *vid,
    int     delay,
    int     mode
);

#endif
/* ======================================================================== */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 2 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/*  General Public License for more details.                                */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program; if not, write to the Free Software             */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               */
/* ======================================================================== */
/*                   Copyright (c) 2005, Joseph Zbiciak                     */
/* ======================================================================== */


















/* ======================================================================== */
/*  LZW Encode                                                              */
/*                                                                          */
/*  This code compresses an input buffer using LZW compression, as defined  */
/*  by the GIF standard.  This includes dividing the compressed output      */
/*  into 256-byte blocks.                                                   */
/*                                                                          */
/*  My data structure is entirely uncreative.  I use an N-way tree to       */
/*  represent the current code table.  It's dirt simple to implement, but   */
/*  it's a memory pig.  Since the longest code is 12 bits, I use indices    */
/*  instead of pointers, and use a static table of codes.                   */
/* ======================================================================== */

#ifndef LZW_ENC_H_
#define LZW_ENC_H_ 1

int lzw_encode (const uint8_t *i_buf, uint8_t *o_buf, int i_len, int max_o_len);
int lzw_encode2(const uint8_t *i_buf, const uint8_t *i_buf_alt,
                      uint8_t *o_buf, int i_len, int max_o_len);

#endif
/* ======================================================================== */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 2 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/*  General Public License for more details.                                */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program; if not, write to the Free Software             */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               */
/* ======================================================================== */
/*                   Copyright (c) 2005, Joseph Zbiciak                     */
/* ======================================================================== */