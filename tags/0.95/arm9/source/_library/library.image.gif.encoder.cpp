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

#include "_library/library.image.gif.encoder.h"
#include "_type/type.system.h"
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
//#define dprintf(x) printf x
#define dprintf(x)

uint8_t *gif_enc_buf = NULL;
int     gif_enc_buf_sz = 0;

uint8_t *gif_img_tr  = NULL;
uint8_t *gif_img_a   = NULL;
uint8_t *gif_img_b   = NULL;
uint8_t *gif_img_d   = NULL, *gif_pal_d = NULL;
uint8_t *gif_img_e   = NULL, *gif_pal_e = NULL;
uint8_t *gif_img_f   = NULL, *gif_pal_f = NULL;
int     gif_img_sz  = 0;

int gen_mpi(uint8_t *src, uint8_t *xtra, uint8_t *dst, 
                  int cnt, uint8_t *pal);

int gif_best_stats[6];

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
)
{
    int i, gt_size;
    uint8_t *enc_ptr;
    size_t wrote;

    /* -------------------------------------------------------------------- */
    /*  Set up our GIF structure to hold incoming images.                   */
    /* -------------------------------------------------------------------- */
    memset(gif, 0, sizeof(gif_t));
    gif->f     = f;
    gif->x_dim = x_dim;
    gif->y_dim = y_dim;
    if (multi)
    {
        gif->vid = (uint8_t*) calloc(x_dim, y_dim);
        gif->pal = (uint8_t*) calloc(n_cols, 3);
    } else
    {
        gif->vid = gif->pal = NULL;
    }

    if (!gif_enc_buf || gif_enc_buf_sz < x_dim * y_dim * 2)
    {
        if (gif_enc_buf) free(gif_enc_buf);
        gif_enc_buf_sz = x_dim * y_dim * 2;
        gif_enc_buf    = (uint8_t*) malloc(gif_enc_buf_sz);
    }

    if (!gif_enc_buf || (multi && (!gif->pal || !gif->vid)))
    {
        _system::debug( "gif_start: out of memory\n");
        return -1;
    }

    if (multi && n_cols < 256)
    {
        gif->n_cols = n_cols + 1;
        gif->trans  = n_cols;
    } else
    {
        gif->n_cols = n_cols;
        gif->trans  = trans;
    }
    if (multi)
        memset(gif->vid, 0xFF & gif->trans, x_dim * y_dim);


    /* -------------------------------------------------------------------- */
    /*  Save a copy of the palette if this is a multiframe GIF.             */
    /* -------------------------------------------------------------------- */
    if (multi)
        memcpy(gif->pal, pal, n_cols * 3);

    /* -------------------------------------------------------------------- */
    /*  Output a GIF header.                                                */
    /* -------------------------------------------------------------------- */
    gt_size = 0;

    for (i = 0; i < 8; i++)
        if ((2 << i) >= gif->n_cols)
        {
            gt_size = i;
            break;
        }
	
	dprintf(("color_depth: %d\n",gt_size));

    memcpy(gif_enc_buf, multi ? "GIF89a" : "GIF87a", 6);
    enc_ptr = gif_enc_buf + 6;

    *enc_ptr++ = (x_dim >> 0) & 0xFF;   /* X dimension LSB                  */
    *enc_ptr++ = (x_dim >> 8) & 0xFF;   /* X dimension MSB                  */
    *enc_ptr++ = (y_dim >> 0) & 0xFF;   /* Y dimension LSB                  */
    *enc_ptr++ = (y_dim >> 8) & 0xFF;   /* Y dimension MSB                  */
    *enc_ptr++ = 0x80           /* Global Color Table present               */
               | 0x70           /* 8 significant bits in palette entries    */
               | gt_size;       /* Color Depth (sets GCT size)              */
    *enc_ptr++ = 0;             /* Background color index.                  */
    *enc_ptr++ = 0;             /* Aspect ratio.  We hardcode to 0.         */


    for (i = 0; i < gif->n_cols; i++)
    {
        *enc_ptr++ = pal[i*3+0];
        *enc_ptr++ = pal[i*3+1];
        *enc_ptr++ = pal[i*3+2];
    }
    for (i = gif->n_cols; i < (2 << gt_size); i++)
    {
        *enc_ptr++ = 0xFF;
        *enc_ptr++ = 0x80;
        *enc_ptr++ = 0x80;
    }

    /* -------------------------------------------------------------------- */
    /*  If this is a multi-frame GIF, write out the extension block which   */
    /*  instructs the web browser to loop the animation.                    */
    /*                                                                      */
    /*      byte 1         : 33 (hex 0x21) GIF Extension code               */
    /*      byte 2         : 255 (hex 0xFF) Application Extension Label     */
    /*      byte 3         : 11 (hex (0x0B) Length of Application Block     */
    /*                       (eleven bytes of data to follow)               */
    /*      bytes 4 to 11  : "NETSCAPE"                                     */
    /*      bytes 12 to 14 : "2.0"                                          */
    /*      byte 15        : 3 (hex 0x03) Length of Data Sub-Block          */
    /*                       (three bytes of data to follow)                */
    /*      byte 16        : 1 (hex 0x01)                                   */
    /*      bytes 17 to 18 : 0 to 65535, little-endian unsigned integer     */
    /*                       This indicate the number of iterations the     */
    /*                       loop should be executed.  0 == infinite        */
    /*      bytes 19       : 0 (hex 0x00) a Data Sub-block Terminator.      */
    /*                                                                      */
    /*  Text above comes from http://the-labs.com/GIFMerge/ w/ minor edits. */
    /* -------------------------------------------------------------------- */
    if (multi)
    {
        *enc_ptr++ = 0x21; // Introduces Extension Block
        *enc_ptr++ = 0xFF; // Specifies what extension this is
        *enc_ptr++ = 0x0B; // = Size: 11 bytes
        memcpy(enc_ptr, "NETSCAPE2.0", 11); enc_ptr += 11;
        *enc_ptr++ = 3; // Next Extension?
        *enc_ptr++ = 1;
        *enc_ptr++ = 0; /* "forever" */
        *enc_ptr++ = 0; /* "forever" */
        *enc_ptr++ = 0;
    }
	else if( trans >= 0 )
	{
		*enc_ptr++ = 0x21; // Introduces Extension Block
        *enc_ptr++ = 0xF9; // Specifies what extension this is
        *enc_ptr++ = 0x04; // = Size: 4 bytes
        *enc_ptr++ = 0x01;
        *enc_ptr++ = 0x00;
        *enc_ptr++ = 0x00;
		*enc_ptr++ = trans;
		*enc_ptr++ = 0x00;
	}

    /* -------------------------------------------------------------------- */
    /*  Write out the GIF header, and that's it.  The caller will need      */
    /*  to call gif_wr_frame_X() to write out the image(s).                 */
    /* -------------------------------------------------------------------- */
    wrote = fwrite(gif_enc_buf, 1, enc_ptr - gif_enc_buf, gif->f);
    if (wrote < (unsigned)(enc_ptr - gif_enc_buf))
	{
		_system::debug( "gif_start: file write failed!\n");
        return -1;
	}

    return wrote;
}

/* ======================================================================== */
/*  GIF_FINISH -- Finishes off a GIF, terminating it and freeing memory.    */
/* ======================================================================== */
int gif_finish(gif_t *gif)
{
    /* -------------------------------------------------------------------- */
    /*  Write an image terminator.                                          */
    /* -------------------------------------------------------------------- */
    fputc(0x3B, gif->f);
    fflush(gif->f);

    /* -------------------------------------------------------------------- */
    /*  Free any allocated memory in the gif structure.                     */
    /* -------------------------------------------------------------------- */
    if (gif->vid) { free(gif->vid); gif->vid = NULL; }
    if (gif->pal) { free(gif->pal); gif->pal = NULL; }

    return 1;   /* wrote 1 byte. */
}


/* ======================================================================== */
/*  GIF_WR_FRAME_S -- Writes single-frame image to GIF.                     */
/* ======================================================================== */
int gif_wr_frame_s
(
    gif_t  *gif,
    uint8_t *vid
)
{
    uint8_t *enc_ptr = gif_enc_buf;
    int lzw_len;
    size_t wrote;

    assert(enc_ptr);

    /* -------------------------------------------------------------------- */
    /*  Output local header.                                                */
    /* -------------------------------------------------------------------- */
    *enc_ptr++ = 0x2C;                      /* Local header signature.      */
    *enc_ptr++ = 0;                         /* X position of image = 0 LSB  */
    *enc_ptr++ = 0;                         /* X position of image = 0 MSB  */
    *enc_ptr++ = 0;                         /* Y position of image = 0 LSB  */
    *enc_ptr++ = 0;                         /* Y position of image = 0 MSB  */
    *enc_ptr++ = (gif->x_dim >> 0) & 0xFF;  /* Width = X dimension LSB      */
    *enc_ptr++ = (gif->x_dim >> 8) & 0xFF;  /* Width = X dimension MSB      */
    *enc_ptr++ = (gif->y_dim >> 0) & 0xFF;  /* Height = Y dimension LSB     */
    *enc_ptr++ = (gif->y_dim >> 8) & 0xFF;  /* Height = Y dimension MSB     */
    *enc_ptr++ = 0;                         /* No local colors              */


    /* -------------------------------------------------------------------- */
    /*  Now compress the image.                                             */
    /* -------------------------------------------------------------------- */
    lzw_len = lzw_encode(vid, enc_ptr, gif->x_dim*gif->y_dim, 
                         gif_enc_buf_sz - (enc_ptr - gif_enc_buf));

    if (lzw_len < 0)
	{
		_system::debug( "gif_wr_frame_s: lzw_encode failed!\n");
        return -1;
	}

    enc_ptr += lzw_len;
    assert(enc_ptr < gif_enc_buf + gif_enc_buf_sz);

    /* -------------------------------------------------------------------- */
    /*  Write the compressed image out.                                     */
    /* -------------------------------------------------------------------- */
    wrote = fwrite(gif_enc_buf, 1, enc_ptr - gif_enc_buf, gif->f);
    if (wrote < (unsigned)(enc_ptr - gif_enc_buf))
	{
		_system::debug( "gif_wr_frame_s: file write failed!\n");
        return -1;
	}

    return wrote;
}

/* ======================================================================== */
/*  GIF_WRITE       -- Wrapper around gif_start/gif_wr_frame_s.             */
/* ======================================================================== */
int gif_write
(
    FILE   *f,
    uint8_t *vid,
    int     x_dim,
    int     y_dim,
    uint8_t *pal,
    int     n_cols,
	int     trans
)
{
    gif_t   gif;
    int     wrote = 0, ret;

    /* -------------------------------------------------------------------- */
    /*  Start the GIF.                                                      */
    /* -------------------------------------------------------------------- */
    ret    = gif_start(&gif, f, x_dim, y_dim, pal, n_cols, trans, 0);
    wrote += ret;

    if (ret < 0)
	{
		_system::debug( "gif_write: gif_start failed!\n");
        return -1;
	}

    /* -------------------------------------------------------------------- */
    /*  Send the image.                                                     */
    /* -------------------------------------------------------------------- */
    ret    = gif_wr_frame_s(&gif, vid);
    wrote += ret;

    if (ret < 0)
	{
		_system::debug( "gif_wr_frame_s: gif_start failed!\n");
        return -1;
	}

    /* -------------------------------------------------------------------- */
    /*  Terminate it.                                                       */
    /* -------------------------------------------------------------------- */
    ret    = gif_finish(&gif);
    wrote += ret;

    if (ret < 0)
	{
		_system::debug( "gif_finish: gif_start failed!\n");
        return -1;
	}

    return wrote;
}



/* ======================================================================== */
/*  GIF_WR_FRAME_M -- Writes next frame to a multi-frame GIF.               */
/*                    Attempts to optimize image.                           */
/* ======================================================================== */
int gif_wr_frame_m
(
    gif_t  *gif,
    uint8_t *vid,
    int     delay,
    int     mode
)
{
    int trans = gif->trans >= 0;
    uint8_t *enc_ptr = gif_enc_buf;
    uint8_t *vid_ptr, *prv_ptr, *trn_ptr;
    uint8_t *best_img1, *best_img2, *best_lct;
    int x, y, xx, yy, min_x, min_y, max_x, max_y, width, height;
    int n_col_d, n_col_e = 0, n_col_f = 0, lct_sz_d, lct_sz_e, lct_sz_f;
    int enc_sz_a, enc_sz_b, enc_sz_c, enc_sz_d, enc_sz_e, enc_sz_f;
    int best_sz, best_lct_sz, trans_idx, do_trans = 0;
    int lzw_len; 
    int cnt, num_trans = 0;
    char best = '*';
    size_t wrote;

    assert(enc_ptr);

    /* -------------------------------------------------------------------- */
    /*  Allocate our image optimization scratch buffers, if necessary.      */
    /* -------------------------------------------------------------------- */
    if (gif_img_sz < gif->x_dim * gif->y_dim)
    {
        if (gif_img_tr) free(gif_img_tr);
        gif_img_sz = gif->x_dim * gif->y_dim;
        gif_img_tr = (uint8_t*) malloc(7 * gif->x_dim * gif->y_dim + 3*256);

        if (!gif_img_tr)
        {
            _system::debug( "gif_wr_frame_m: out of memory\n");
            return -1;
        }

        gif_img_a = gif_img_tr + gif_img_sz * 2;
        gif_img_b = gif_img_a  + gif_img_sz;
        gif_img_d = gif_img_b  + gif_img_sz;
        gif_img_e = gif_img_d  + gif_img_sz;
        gif_img_f = gif_img_e  + gif_img_sz;
        gif_pal_d = gif_img_f  + gif_img_sz;
        gif_pal_e = gif_pal_d  + 256;
        gif_pal_f = gif_pal_e  + 256;
    }

    /* -------------------------------------------------------------------- */
    /*  Optimize image:                                                     */
    /*                                                                      */
    /*   1. Generate "transparency" image with trans pixels wherever this   */
    /*      image is the same as the previous one.                          */
    /*                                                                      */
    /*   2. Compute tighter bounding box on image--that is, smallest box    */
    /*      that contains all the non-trans pixels.                         */
    /*                                                                      */
    /*   3. Crop the incoming image based on the tighter bounding box.      */
    /*                                                                      */
    /*   4. Generate "minimal palette" images that renumber all the         */
    /*      pixels into the smallest possible numbering space.              */
    /*                                                                      */
    /*   5. Compress the image multiple ways, and take the best:            */
    /*                                                                      */
    /*       a. Cropped image, orig palette, no trans pixels                */
    /*       b. Cropped image, orig palette, trans pixels                   */
    /*       c. Cropped image, orig palette, "wildcard" trans/no-trans      */
    /*       d. Cropped image, new palette, no trans pixels                 */
    /*       e. Cropped image, new palette, trans pixels                    */
    /*       f. Cropped image, new palette, "wildcard" trans/no-trans       */
    /*                                                                      */
    /*      When comparing new palette to orig palette, include cost        */
    /*      of sending local palette.  If the global palette has too        */
    /*      many colors, we may not be able to try b or c.  If the local    */
    /*      palette has too many colors, we may not be able to try e or     */
    /*      f.                                                              */
    /*                                                                      */
    /* -------------------------------------------------------------------- */

    /* -------------------------------------------------------------------- */
    /*   1. Generate "transparency" image...                                */
    /*   2. Compute tighter bounding box on image...                        */
    /* -------------------------------------------------------------------- */
    min_x = gif->x_dim;
    min_y = gif->y_dim;
    max_x = 0;
    max_y = 0;

    vid_ptr = vid;
    prv_ptr = gif->vid;
    trn_ptr = gif_img_tr;

    for (y = 0; y < gif->y_dim; y++)
        for (x = 0; x < gif->x_dim; x++)
        {
            uint8_t curr = *vid_ptr++;
            uint8_t prev = *prv_ptr++;
            uint8_t tran = gif->trans;

            if (curr != prev)
            {
                tran = curr;
                if (x < min_x) min_x = x;
                if (y < min_y) min_y = y;
                if (x > max_x) max_x = x;
                if (y > max_y) max_y = y;
            }

            *trn_ptr++ = tran;
        }


    /* -------------------------------------------------------------------- */
    /*  Stop now if current image matches previous image.                   */
    /* -------------------------------------------------------------------- */
    if (min_x > max_x || min_y > max_y)
        return 0;

    if (mode != 0)
    {
        min_x = 0;
        min_y = 0;
        max_x = gif->x_dim - 1;
        max_y = gif->y_dim - 1;
    }

    width  = max_x - min_x + 1;
    height = max_y - min_y + 1;
    cnt    = width*height;

    /* -------------------------------------------------------------------- */
    /*   3. Crop the incoming image based on the tighter bounding box.      */
    /* -------------------------------------------------------------------- */
    for (y = min_y, yy = 0; y <= max_y; y++, yy++)
        for (x = min_x, xx = 0; x <= max_x; x++, xx++)
        {
            /*  a. Cropped image, orig palette, no trans pixels             */
            /*  b. Cropped image, orig palette, trans pixels                */
            gif_img_a[xx + yy*width] = vid       [x + y*gif->x_dim];
            gif_img_b[xx + yy*width] = gif_img_tr[x + y*gif->x_dim];

            if (gif_img_tr[x + y*gif->x_dim] == gif->trans)
                num_trans++;
        }

    if (num_trans == 0) trans = 0;
    if (mode != 0)      trans = 0;
#if 0
    {
        FILE *ff;
        ff = fopen("debug", "wb");
        if (ff)
        {
            fwrite(gif_img_a, 1, cnt, ff);
            fclose(ff);
        }
    }
#endif

    /* -------------------------------------------------------------------- */
    /*   4. Generate "minimal palette" images...                            */
    /*       d. Cropped image, new palette, no trans pixels                 */
    /*       e. Cropped image, new palette, trans pixels                    */
    /*       f. Cropped image, new palette, "wildcard" trans/no-trans       */
    /* -------------------------------------------------------------------- */
              n_col_d = gen_mpi(gif_img_a, NULL,    gif_img_d, cnt, gif_pal_d);
    if(trans) n_col_e = gen_mpi(gif_img_b, NULL,    gif_img_e, cnt, gif_pal_e);
    if(trans) n_col_f = gen_mpi(gif_img_b,gif_img_a,gif_img_f, cnt, gif_pal_f);

    for (lct_sz_d = 1; (2 << lct_sz_d) < n_col_d; lct_sz_d++);
    for (lct_sz_e = 1; (2 << lct_sz_e) < n_col_e; lct_sz_e++);
    for (lct_sz_f = 1; (2 << lct_sz_f) < n_col_f; lct_sz_f++);

    if (trans)
    {
        int i;
        for (i = 0; i < n_col_d; i++)
            assert(gif_pal_d[i] == gif_pal_f[i]);
//printf("n_col_d=%d, n_col_f=%d gif_pal_f[n_col_f-1]=%d gif->trans=%d\n", n_col_d, n_col_f, gif_pal_f[n_col_f-1], gif->trans);
        assert(n_col_d < n_col_f);
        assert(gif_pal_f[n_col_f - 1] == gif->trans);
    }


    /* -------------------------------------------------------------------- */
    /*   5. Compress the image multiple ways, and take the best...          */
    /*                                                                      */
    /*  We reuse gif_img_tr as a compression buffer here, since we don't    */
    /*  need that image any longer, but we do need a temp buffer for LZW.   */
    /* -------------------------------------------------------------------- */
    enc_sz_a  = lzw_encode(gif_img_a, gif_img_tr, cnt, gif_img_sz*2);
    enc_sz_b  = -1;
    enc_sz_c  = -1;
    enc_sz_d  = lzw_encode(gif_img_d, gif_img_tr, cnt, gif_img_sz*2);
    enc_sz_e  = -1;
    enc_sz_f  = -1;
    if (trans)
    {
        enc_sz_b = lzw_encode(gif_img_b, gif_img_tr, cnt, gif_img_sz*2);
        enc_sz_e = lzw_encode(gif_img_e, gif_img_tr, cnt, gif_img_sz*2);
#if 1
        enc_sz_c = lzw_encode2(gif_img_b,  gif_img_a, 
                               gif_img_tr, cnt, gif_img_sz);
        enc_sz_f = lzw_encode2(gif_img_f,  gif_img_d, 
                               gif_img_tr, cnt, gif_img_sz);
#endif
    }

    if (enc_sz_d > 0) enc_sz_d += 3 << (lct_sz_d + 1);
    if (enc_sz_e > 0) enc_sz_e += 3 << (lct_sz_e + 1);
    if (enc_sz_f > 0) enc_sz_f += 3 << (lct_sz_f + 1);

    if (enc_sz_a < 0) enc_sz_a = INT_MAX;
    if (enc_sz_b < 0) enc_sz_b = INT_MAX;
    if (enc_sz_c < 0) enc_sz_c = INT_MAX;
    if (enc_sz_d < 0) enc_sz_d = INT_MAX;
    if (enc_sz_e < 0) enc_sz_e = INT_MAX;
    if (enc_sz_f < 0) enc_sz_f = INT_MAX;

    /* Initially assume A's the best. */
    best_img1   = gif_img_a;
    best_img2   = NULL;
    best_lct    = NULL;
    best_lct_sz = 0;
    best_sz     = enc_sz_a;
    trans_idx   = 0;
    do_trans    = 0;
    best        = 'a';

#if 1
    if (enc_sz_b < best_sz) 
    { 
        best_img1   = gif_img_b; 
        best_img2   = NULL;
        best_lct    = NULL;
        best_lct_sz = 0;
        best_sz     = enc_sz_b;
        trans_idx   = gif->trans;
        do_trans    = 1;
        best        = 'b';
    }              
                   
    if (enc_sz_d < best_sz) 
    {              
        best_img1   = gif_img_d; 
        best_img2   = NULL;
        best_lct    = gif_pal_d;
        best_lct_sz = lct_sz_d;
        best_sz     = enc_sz_d;
        trans_idx   = 0;
        do_trans    = 0;
        best        = 'd';
    }              
                   
    if (enc_sz_e < best_sz) 
    {              
        best_img1   = gif_img_e; 
        best_img2   = NULL;
        best_lct    = gif_pal_e;
        best_lct_sz = lct_sz_e;
        best_sz     = enc_sz_e;
        trans_idx   = n_col_e - 1;
        do_trans    = 1;
        best        = 'e';
    }              

    if (enc_sz_c < best_sz) 
    {              
        best_img1   = gif_img_b; 
        best_img2   = gif_img_a; 
        best_lct    = NULL;
        best_lct_sz = 0;
        best_sz     = enc_sz_c;
        trans_idx   = gif->trans;
        do_trans    = 1;
        best        = 'c';
    }              
                   
    if (enc_sz_f < best_sz) 
    {              
        best_img1   = gif_img_f; 
        best_img2   = gif_img_d; 
        best_lct    = gif_pal_f;
        best_lct_sz = lct_sz_f;
        best_sz     = enc_sz_f;
        trans_idx   = n_col_f - 1;
        do_trans    = 1;
        best        = 'f';
    }              

    if (best_sz < 0 || best_sz == INT_MAX)
    {
        _system::debug( "gif_wr_frame_m: Image overflowed compression "
                        "buffer.\n");
        return -1;
    }
#endif
    gif_best_stat[best - 'a']++;

    /* -------------------------------------------------------------------- */
    /*  Output a Graphic Control Ext.                                       */
    /* -------------------------------------------------------------------- */
    *enc_ptr++ = 0x21;                  /* Extension signature              */
    *enc_ptr++ = 0xF9;                  /* Graphic control extension        */
    *enc_ptr++ = 0x04;                  /* Length of block:  Fixed at 4.    */
    *enc_ptr++ = 0x04 | do_trans;       /* Disposal 01, No input, Trans     */
    *enc_ptr++ = (delay >> 0) & 0xFF;   /* delay in 100ths of a second LSB  */
    *enc_ptr++ = (delay >> 8) & 0xFF;   /* delay in 100ths of a second MSB  */
    *enc_ptr++ = trans_idx;             /* Transparency index, if any.      */
    *enc_ptr++ = 0x00;                  /* GCE block terminator.            */

    /* -------------------------------------------------------------------- */
    /*  Output an Image Descriptor.                                         */
    /* -------------------------------------------------------------------- */
    *enc_ptr++ = 0x2C;                  /* Image Separator                  */
    *enc_ptr++ = (min_x >> 0) & 0xFF;   /* Left edge, LSB                   */
    *enc_ptr++ = (min_x >> 8) & 0xFF;   /* Left edge, MSB                   */
    *enc_ptr++ = (min_y >> 0) & 0xFF;   /* Top edge, LSB                    */
    *enc_ptr++ = (min_y >> 8) & 0xFF;   /* Top edge, MSB                    */
    *enc_ptr++ = (width >> 0) & 0xFF;   /* Image width, LSB                 */
    *enc_ptr++ = (width >> 8) & 0xFF;   /* Image width, MSB                 */
    *enc_ptr++ = (height >> 0) & 0xFF;  /* Image height, LSB                */
    *enc_ptr++ = (height >> 8) & 0xFF;  /* Image height, MSB                */
    *enc_ptr++ = best_lct_sz == 0 ? 0   /* no local color table?            */
               : best_lct_sz | 0x80;    /* or yes local color table?        */

//printf("[%3d,%3d] [%3d,%3d] [%3d,%3d] delay=%-3d trans=%-3d%c lct=%d\n", min_x, min_y, width, height, min_x + width, min_y + height, delay, trans_idx, do_trans ? '*' : ' ', best_lct_sz);

    /* -------------------------------------------------------------------- */
    /*  If we're sending a local color table, put it here.                  */
    /* -------------------------------------------------------------------- */
    if (best_lct_sz)
    {
        int i;

        for (i = 0; i < (2 << best_lct_sz); i++)
        {
            if (best_lct[i] < gif->n_cols)
            {
                *enc_ptr++ = gif->pal[3*best_lct[i] + 0];
                *enc_ptr++ = gif->pal[3*best_lct[i] + 1];
                *enc_ptr++ = gif->pal[3*best_lct[i] + 2];
            } else
            {
                *enc_ptr++ = 0;
                *enc_ptr++ = 0;
                *enc_ptr++ = 0;
            }
        }
    }

    /* -------------------------------------------------------------------- */
    /*  Now, re-encode the winning image.                                   */
    /* -------------------------------------------------------------------- */
    if (best_img2)
    {
        lzw_len = lzw_encode2(best_img1, best_img2, enc_ptr, cnt, 
                              gif_enc_buf_sz - (enc_ptr - gif_enc_buf) - 1);

    } else
    {
        lzw_len = lzw_encode(best_img1, enc_ptr, cnt,
                             gif_enc_buf_sz - (enc_ptr - gif_enc_buf) - 1);
    }
    if (lzw_len < 0)
    {
        _system::debug( "gif_wr_frame_m: Final encode failed?\n");
        return -1;
    }

    enc_ptr += lzw_len;
    assert(enc_ptr < gif_enc_buf + gif_enc_buf_sz);

    /* -------------------------------------------------------------------- */
    /*  Write the compressed image out.                                     */
    /* -------------------------------------------------------------------- */
    wrote = fwrite(gif_enc_buf, 1, enc_ptr - gif_enc_buf, gif->f);
    if (wrote < (unsigned)(enc_ptr - gif_enc_buf))
    {
        _system::debug( "gif_wr_frame_m: Short write? %ld vs %ld\n", 
                (long)wrote, (long)(enc_ptr - gif_enc_buf));
        return -1;
    }

    /* -------------------------------------------------------------------- */
    /*  Make the current image the new previous image.                      */
    /* -------------------------------------------------------------------- */
    memcpy(gif->vid, vid, gif->x_dim * gif->y_dim);

    return wrote;
}

/* ======================================================================== */
/*  GEN_MPI -- Generate Minimal Palette Image, and its palette.             */
/* ======================================================================== */
int gen_mpi
(
    uint8_t *src, 
    uint8_t *xtra, 
    uint8_t *dst, 
    int     cnt, 
    uint8_t *pal_map
)
{
    int hist1[256];
    int hist2[256];
    int rev_map[256];
    int i, r = 0;

    memset(hist1,   0, sizeof(hist1));
    memset(hist2,   0, sizeof(hist2));
    memset(rev_map, 0, sizeof(rev_map));

    /* -------------------------------------------------------------------- */
    /*  Find what colors are in the source image.                           */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < cnt; i++)
        hist1[src[i]]++;

    /* -------------------------------------------------------------------- */
    /*  If we have a secondary image, look for its palette also.  It gets   */
    /*  first dibs on palette allocation, because our goal is to just       */
    /*  *extend* the extra image's palette to accomodate our source image.  */
    /* -------------------------------------------------------------------- */
    if (xtra)
    {
        for (i = 0; i < cnt; i++)
            hist2[xtra[i]]++;

        for (i = 0; i < 256; i++)
            if (hist2[i])
            {
                rev_map[i] = r;
                pal_map[r] = i;
                r++;
            }
    }


    /* -------------------------------------------------------------------- */
    /*  Build up our palette map from the source image.                     */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < 256; i++)
        if (hist1[i] && !hist2[i])
        {
            rev_map[i] = r;
            pal_map[r] = i;
            r++;
        }

    /* -------------------------------------------------------------------- */
    /*  Remap the source image to the smaller palette.                      */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < cnt; i++)
        dst[i] = rev_map[src[i]];

    return r;
}

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


int lzw_encode(const uint8_t *i_buf, uint8_t *o_buf, int i_len, int max_o_len)
{
    static uint16_t *dict = NULL;
    static int      dict_size = 0;
    const uint8_t *i_end = i_buf + i_len;
    const uint8_t *i_ptr;
    uint8_t *o_end = o_buf + max_o_len - 1; 
    uint8_t *o_ptr; 
    uint8_t *last_len_byte;
    int i;
    int code_size;
    int max_sym = 0, dict_stride;
    uint32_t curr_word = 0;
    int curr_bits = 0;
    int code = 0, next_new_code, curr_size;
    int end_of_info, clear_code;
    int next_char = 0, next_code;

    /* -------------------------------------------------------------------- */
    /*  First, scan the buffer and determine the total dynamic range of     */
    /*  the input bytes.  We'll pick our starting code size based on that.  */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < i_len; i++)
        if (i_buf[i] > max_sym) 
            max_sym = i_buf[i];
    dict_stride = max_sym + 1;
    dprintf(("max_sym = %.2X\n", max_sym));

    /* -------------------------------------------------------------------- */
    /*  Compute and output the starting code-size.                          */
    /* -------------------------------------------------------------------- */
    for (code_size = 2; code_size < 8; code_size++)
        if ((1 << code_size) > max_sym)
            break;
    dprintf(("code_size = %.2X\n", code_size));
    /* -------------------------------------------------------------------- */
    /*  Allocate the dictionary.  We store the tree in a 2-D array.  One    */
    /*  dimension is the code number, and the other is the codes it chains  */
    /*  to.  We size this to the maximum number of symbols in the input,    */
    /*  so that it's not too big.                                           */
    /* -------------------------------------------------------------------- */
    if (dict_size < dict_stride)
    {
        if (dict) 
            free(dict);
        dict      = (uint16_t*) malloc(4096 * sizeof(uint16_t) * dict_stride);
        dict_size = dict_stride;
    }

    /* -------------------------------------------------------------------- */
    /*  Output the code length, and prepare to compress.                    */
    /* -------------------------------------------------------------------- */
    o_ptr         = o_buf;
    *o_ptr++      = code_size;
    last_len_byte = o_ptr++;  /* save room for first data block length byte */
    i_ptr         = i_buf;
    curr_size     = code_size + 1;
    curr_word     = 0;
    curr_bits     = 0;
    next_new_code = 0x1000;         /* trigger dictionary flush first go */
    clear_code    = (1 << code_size);
    end_of_info   = (1 << code_size) + 1;
    next_char     = *i_ptr++;

    /* -------------------------------------------------------------------- */
    /*  Compress!                                                           */
    /* -------------------------------------------------------------------- */
    while (i_ptr <= i_end && code != end_of_info)
    {
        dprintf(("remaining: %10d\n", i_end - i_ptr));

        /* ---------------------------------------------------------------- */
        /*  If dictionary's full, send a clear code and flush dictionary.   */
        /*  Otherwise, patch the previous code+char into the dictionary.    */
        /* ---------------------------------------------------------------- */
        if (next_new_code == 0x1000)
        {
            dprintf(("CLEAR %.3X %d\n", clear_code, curr_size));

            curr_word |= clear_code << curr_bits; 
            curr_bits += curr_size;
            while (curr_bits > 8)
            {
                /* Handle packaging data into 256-byte records */
                if (o_ptr - last_len_byte == 256)
                {
                    dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", 
                              last_len_byte, o_ptr));

                    *last_len_byte = 255;
                    last_len_byte  = o_ptr++;
                }
                if (o_ptr >= o_end)
                    goto overflow;

                *o_ptr++    = curr_word & 0xFF;
                curr_word >>= 8;
                curr_bits  -= 8;
            }

            curr_size = code_size + 1;
            next_new_code = (1 << code_size) + 2;
            memset(dict, 0, 4096*sizeof(uint16_t)*dict_stride);
        } else
        {
            dprintf(("new code: %.3X = %.3X + %.2X\n", next_new_code, 
                     code, next_char));

            dict[code*dict_stride + next_char] = next_new_code;
            if (next_new_code == (1 << curr_size))
                curr_size++;
            next_new_code++;
        }

        code = next_char;  /* Previous concat becomes new initial code */
        dprintf(("next code: %.2X %c\n", code, code == end_of_info ? '*':' '));

        /* ---------------------------------------------------------------- */
        /*  Keep concatenating as long as we stay in the dictionary.        */
        /* ---------------------------------------------------------------- */
        if (i_ptr == i_end)
        {
            next_char = end_of_info;
            dprintf(("--> next is EOI!\n"));
        } else
        {
            next_code = -1;
            while (next_code && i_ptr < i_end)
            {
                next_char = *i_ptr++;
                next_code = dict[code*dict_stride + next_char];
                dprintf(("--> code: %.3X + %.2X = %.3X\n", code, 
                         next_char, next_code));

                if (next_code)
                    code = next_code;
            }
            if (next_code && i_ptr == i_end)
                next_char = end_of_info;

            if (next_char == end_of_info) 
            { 
                dprintf(("--> next is EOI! (b)\n"));
            }
        }

        /* ---------------------------------------------------------------- */
        /*  Ok, no longer in the dictionary.  Emit the current code and the */
        /*  extra character.  We can stuff two codes in, since curr_bits    */
        /*  should never be more than 7, and curr_size should be no more    */
        /*  than 12.                                                        */
        /* ---------------------------------------------------------------- */
        curr_word |= code << curr_bits; 
        curr_bits += curr_size;
        dprintf(("SEND %.4X %d curr: %.8X %2d\n", code, curr_size, 
                 curr_word, curr_bits));
        while (curr_bits > 8)
        {
            /* Handle packaging data into 256-byte records */
            if (o_ptr - last_len_byte == 256)
            {
                dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", last_len_byte, 
                          o_ptr));
                *last_len_byte = 255;
                last_len_byte  = o_ptr++;
            }
            if (o_ptr >= o_end)
                goto overflow;

            *o_ptr++    = curr_word & 0xFF;
            curr_word >>= 8;
            curr_bits  -= 8;
        }
    }

    /* -------------------------------------------------------------------- */
    /*  If we have any left-over bits (at most 7), go ahead and flush them. */
    /* -------------------------------------------------------------------- */
    while (curr_bits > 0)  /* flush it ALL out. */
    {
        /* Handle packaging data into 256-byte records */
        if (o_ptr - last_len_byte == 256)
        {
            dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", last_len_byte, o_ptr));
            *last_len_byte = 255;
            last_len_byte  = o_ptr++;
        }
        if (o_ptr >= o_end)
            goto overflow;

        *o_ptr++    = curr_word & 0xFF;
        curr_word >>= 8;
        curr_bits  -= 8;
    }

    /* -------------------------------------------------------------------- */
    /*  Patch in the last length byte, and a 0-length record.  We are       */
    /*  guaranteed to have room here, since our overflow criterion above    */
    /*  is conservative by one character.                                   */
    /* -------------------------------------------------------------------- */
    *last_len_byte = o_ptr - last_len_byte - 1;
    *o_ptr++ = 0;

    dprintf(("encoded %d bytes\n", o_ptr - o_buf));

    return o_ptr - o_buf;

overflow:
	_system::debug( "lzw_encode: overflowed!\n");
    return -1;
}

int lzw_encode2(const uint8_t *i_buf, const uint8_t *i_buf_alt,
                uint8_t *o_buf, int i_len, int max_o_len)
{
    static uint16_t *dict = NULL;
    static int      dict_size = 0;
    int i_idx = 0;
    uint8_t *o_end = o_buf + max_o_len - 1; 
    uint8_t *o_ptr; 
    uint8_t *last_len_byte;
    int i;
    int code_size;
    int max_sym = 0, dict_stride;
    uint32_t curr_word = 0;
    int curr_bits = 0;
    int code = 0, next_new_code, curr_size;
    int end_of_info, clear_code;
    int next_char = 0, next_code;

    /* -------------------------------------------------------------------- */
    /*  First, scan the buffer and determine the total dynamic range of     */
    /*  the input bytes.  We'll pick our starting code size based on that.  */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < i_len; i++)
    {
        if (i_buf[i] > max_sym) 
            max_sym = i_buf[i];
        if (i_buf_alt[i] > max_sym) 
            max_sym = i_buf_alt[i];
    }

    dict_stride = max_sym + 1;
    dprintf(("max_sym = %.2X\n", max_sym));

    /* -------------------------------------------------------------------- */
    /*  Compute and output the starting code-size.                          */
    /* -------------------------------------------------------------------- */
    for (code_size = 2; code_size < 8; code_size++)
        if ((1 << code_size) > max_sym)
            break;
    dprintf(("code_size = %.2X\n", code_size));
    /* -------------------------------------------------------------------- */
    /*  Allocate the dictionary.  We store the tree in a 2-D array.  One    */
    /*  dimension is the code number, and the other is the codes it chains  */
    /*  to.  We size this to the maximum number of symbols in the input,    */
    /*  so that it's not too big.                                           */
    /* -------------------------------------------------------------------- */
    if (dict_size < dict_stride)
    {
        if (dict) 
            free(dict);
        dict      = (uint16_t*) malloc(4096 * sizeof(uint16_t) * dict_stride);
        dict_size = dict_stride;
    }

    /* -------------------------------------------------------------------- */
    /*  Output the code length, and prepare to compress.                    */
    /* -------------------------------------------------------------------- */
    o_ptr         = o_buf;
    *o_ptr++      = code_size;
    last_len_byte = o_ptr++;  /* save room for first data block length byte */
    curr_size     = code_size + 1;
    curr_word     = 0;
    curr_bits     = 0;
    next_new_code = 0x1000;         /* trigger dictionary flush first go */
    clear_code    = (1 << code_size);
    end_of_info   = (1 << code_size) + 1;
    next_char     = i_buf[i_idx++];

    /* -------------------------------------------------------------------- */
    /*  Compress!                                                           */
    /* -------------------------------------------------------------------- */
    while (i_idx <= i_len && code != end_of_info)
    {
        dprintf(("remaining: %10d\n", i_len - i_idx));

        /* ---------------------------------------------------------------- */
        /*  If dictionary's full, send a clear code and flush dictionary.   */
        /*  Otherwise, patch the previous code+char into the dictionary.    */
        /* ---------------------------------------------------------------- */
        if (next_new_code == 0x1000)
        {
            dprintf(("CLEAR %.3X %d\n", clear_code, curr_size));

            curr_word |= clear_code << curr_bits; 
            curr_bits += curr_size;
            while (curr_bits > 8)
            {
                /* Handle packaging data into 256-byte records */
                if (o_ptr - last_len_byte == 256)
                {
                    dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", 
                              last_len_byte, o_ptr));

                    *last_len_byte = 255;
                    last_len_byte  = o_ptr++;
                }
                if (o_ptr >= o_end)
                    goto overflow;

                *o_ptr++    = curr_word & 0xFF;
                curr_word >>= 8;
                curr_bits  -= 8;
            }

            curr_size = code_size + 1;
            next_new_code = (1 << code_size) + 2;
            memset(dict, 0, 4096*sizeof(uint16_t)*dict_stride);
        } else
        {
            dprintf(("new code: %.3X = %.3X + %.2X\n", next_new_code, 
                     code, next_char));

            dict[code*dict_stride + next_char] = next_new_code;
            if (next_new_code == (1 << curr_size))
                curr_size++;
            next_new_code++;
        }

        code = next_char;  /* Previous concat becomes new initial code */
        dprintf(("next code: %.2X %c\n", code, code == end_of_info ? '*':' '));

        /* ---------------------------------------------------------------- */
        /*  Keep concatenating as long as we stay in the dictionary.        */
        /* ---------------------------------------------------------------- */
        if (i_idx == i_len)
        {
            next_char = end_of_info;
            dprintf(("--> next is EOI!\n"));
        } else
        {
            next_code = -1;
            while (next_code && i_idx < i_len)
            {
                int tmp;

                next_char = i_buf[i_idx];
                if ((tmp = dict[code*dict_stride + i_buf[i_idx]]) != 0)
                {
                    next_code = tmp;
                    dprintf(("--> code: %.3X + %.2X(a) = %.3X\n", code, 
                             next_char, next_code));
                } else 
                if ((tmp = dict[code*dict_stride + i_buf_alt[i_idx]]) != 0)
                {
                    next_char = i_buf_alt[i_idx];
                    next_code = tmp;
                    dprintf(("--> code: %.3X + %.2X(b) = %.3X\n", code, 
                             next_char, next_code));
                } else
                {
                    next_code = 0;
                    dprintf(("--> code: %.3X + %.2X(c) = %.3X\n", code, 
                             next_char, next_code));
                }
                i_idx++;

                if (next_code)
                    code = next_code;
            }
            if (next_code && i_idx == i_len)
                next_char = end_of_info;

            if (next_char == end_of_info) 
            { 
                dprintf(("--> next is EOI! (b)\n"));
            }
        }

        /* ---------------------------------------------------------------- */
        /*  Ok, no longer in the dictionary.  Emit the current code and the */
        /*  extra character.  We can stuff two codes in, since curr_bits    */
        /*  should never be more than 7, and curr_size should be no more    */
        /*  than 12.                                                        */
        /* ---------------------------------------------------------------- */
        curr_word |= code << curr_bits; 
        curr_bits += curr_size;
        dprintf(("SEND %.4X %d curr: %.8X %2d\n", code, curr_size, 
                 curr_word, curr_bits));
        while (curr_bits > 8)
        {
            /* Handle packaging data into 256-byte records */
            if (o_ptr - last_len_byte == 256)
            {
                dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", last_len_byte, 
                          o_ptr));
                *last_len_byte = 255;
                last_len_byte  = o_ptr++;
            }
            if (o_ptr >= o_end)
                goto overflow;

            *o_ptr++    = curr_word & 0xFF;
            curr_word >>= 8;
            curr_bits  -= 8;
        }
    }

    /* -------------------------------------------------------------------- */
    /*  If we have any left-over bits (at most 7), go ahead and flush them. */
    /* -------------------------------------------------------------------- */
    while (curr_bits > 0)  /* flush it ALL out. */
    {
        /* Handle packaging data into 256-byte records */
        if (o_ptr - last_len_byte == 256)
        {
            dprintf(("last_len_byte=%.8X o_ptr=%.8X\n", last_len_byte, o_ptr));
            *last_len_byte = 255;
            last_len_byte  = o_ptr++;
        }
        if (o_ptr >= o_end)
            goto overflow;

        *o_ptr++    = curr_word & 0xFF;
        curr_word >>= 8;
        curr_bits  -= 8;
    }

    /* -------------------------------------------------------------------- */
    /*  Patch in the last length byte, and a 0-length record.  We are       */
    /*  guaranteed to have room here, since our overflow criterion above    */
    /*  is conservative by one character.                                   */
    /* -------------------------------------------------------------------- */
    *last_len_byte = o_ptr - last_len_byte - 1;
    *o_ptr++ = 0;

    dprintf(("encoded %d bytes\n", o_ptr - o_buf));

    return o_ptr - o_buf;

overflow:
	_system::debug( "lzw_encode2: overflowed!\n");
    return -1;
}


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