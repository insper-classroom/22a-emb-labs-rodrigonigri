/*******************************************************************************
 * Size: 35 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/
#define LV_LVGL_H_INCLUDE_SIMPLE
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DSEG35
#define DSEG35 1
#endif

#if DSEG35

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+002D "-" */
    0x7f, 0xff, 0xbf, 0xff, 0xbf, 0xff, 0x80,

    /* U+002E "." */
    0x6f, 0xf6,

    /* U+0030 "0" */
    0xff, 0xff, 0xf4, 0xff, 0xff, 0xdc, 0xff, 0xfe,
    0xf8, 0x0, 0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0,
    0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0,
    0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0,
    0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0,
    0x1, 0xf8, 0x0, 0x3, 0xe0, 0x0, 0x7, 0x0,
    0x0, 0x38, 0x0, 0x1, 0xf0, 0x0, 0x7, 0xe0,
    0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8,
    0x0, 0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e,
    0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0, 0x1f,
    0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7,
    0xef, 0xff, 0xcf, 0x7f, 0xff, 0xcf, 0xff, 0xff,
    0xc0,

    /* U+0031 "1" */
    0x25, 0xbf, 0xff, 0xff, 0xff, 0xfb, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x60,

    /* U+0032 "2" */
    0xff, 0xff, 0xf4, 0xff, 0xff, 0xd0, 0xff, 0xfe,
    0xc0, 0x0, 0x3, 0x0, 0x0, 0x1c, 0x0, 0x0,
    0x70, 0x0, 0x1, 0xc0, 0x0, 0x7, 0x0, 0x0,
    0x1c, 0x0, 0x0, 0x70, 0x0, 0x1, 0xc0, 0x0,
    0x7, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x70, 0x0,
    0x1, 0xc0, 0x0, 0x3, 0xf, 0xff, 0xf4, 0x7f,
    0xff, 0x8b, 0xff, 0xfc, 0x30, 0x0, 0x0, 0xe0,
    0x0, 0x3, 0x80, 0x0, 0xe, 0x0, 0x0, 0x38,
    0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0xe,
    0x0, 0x0, 0x38, 0x0, 0x0, 0xe0, 0x0, 0x3,
    0x80, 0x0, 0xe, 0x0, 0x0, 0x38, 0x0, 0x0,
    0xef, 0xff, 0xc3, 0x7f, 0xff, 0xcf, 0xff, 0xff,
    0xc0,

    /* U+0033 "3" */
    0xff, 0xff, 0xeb, 0xff, 0xff, 0x47, 0xff, 0xf6,
    0x0, 0x0, 0x30, 0x0, 0x3, 0x80, 0x0, 0x1c,
    0x0, 0x0, 0xe0, 0x0, 0x7, 0x0, 0x0, 0x38,
    0x0, 0x1, 0xc0, 0x0, 0xe, 0x0, 0x0, 0x70,
    0x0, 0x3, 0x80, 0x0, 0x1c, 0x0, 0x0, 0xe0,
    0x0, 0x3, 0x1f, 0xff, 0xe8, 0xff, 0xfe, 0xcf,
    0xff, 0xfe, 0x0, 0x0, 0x70, 0x0, 0x3, 0x80,
    0x0, 0x1c, 0x0, 0x0, 0xe0, 0x0, 0x7, 0x0,
    0x0, 0x38, 0x0, 0x1, 0xc0, 0x0, 0xe, 0x0,
    0x0, 0x70, 0x0, 0x3, 0x80, 0x0, 0x1c, 0x0,
    0x0, 0xe0, 0x0, 0x7, 0x1f, 0xff, 0x98, 0xff,
    0xff, 0xf, 0xff, 0xfe,

    /* U+0034 "4" */
    0x0, 0x0, 0x4, 0x0, 0x0, 0x1c, 0x0, 0x0,
    0xf8, 0x0, 0x3, 0xe0, 0x0, 0x1f, 0x80, 0x0,
    0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0,
    0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0,
    0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0,
    0x1, 0xf8, 0x0, 0x3, 0xef, 0xff, 0xf7, 0x7f,
    0xff, 0xb3, 0xff, 0xfd, 0xc0, 0x0, 0x7, 0x0,
    0x0, 0x1c, 0x0, 0x0, 0x70, 0x0, 0x1, 0xc0,
    0x0, 0x7, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x70,
    0x0, 0x1, 0xc0, 0x0, 0x7, 0x0, 0x0, 0x1c,
    0x0, 0x0, 0x70, 0x0, 0x1, 0xc0, 0x0, 0x7,
    0x0, 0x0, 0xc, 0x0, 0x0, 0x0,

    /* U+0035 "5" */
    0xff, 0xff, 0xf0, 0xff, 0xff, 0xcc, 0xff, 0xfe,
    0x38, 0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0,
    0xe, 0x0, 0x0, 0x38, 0x0, 0x0, 0xe0, 0x0,
    0x3, 0x80, 0x0, 0xe, 0x0, 0x0, 0x38, 0x0,
    0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0xe, 0x0,
    0x0, 0x38, 0x0, 0x0, 0xef, 0xff, 0xf3, 0x7f,
    0xff, 0xb3, 0xff, 0xfd, 0xc0, 0x0, 0x7, 0x0,
    0x0, 0x1c, 0x0, 0x0, 0x70, 0x0, 0x1, 0xc0,
    0x0, 0x7, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x70,
    0x0, 0x1, 0xc0, 0x0, 0x7, 0x0, 0x0, 0x1c,
    0x0, 0x0, 0x70, 0x0, 0x1, 0xc0, 0x0, 0x7,
    0xf, 0xff, 0xcc, 0x7f, 0xff, 0xc3, 0xff, 0xff,
    0xc0,

    /* U+0036 "6" */
    0xff, 0xff, 0xf0, 0xff, 0xff, 0xcc, 0xff, 0xfe,
    0x38, 0x0, 0x0, 0xe0, 0x0, 0x3, 0x80, 0x0,
    0xe, 0x0, 0x0, 0x38, 0x0, 0x0, 0xe0, 0x0,
    0x3, 0x80, 0x0, 0xe, 0x0, 0x0, 0x38, 0x0,
    0x0, 0xe0, 0x0, 0x3, 0x80, 0x0, 0xe, 0x0,
    0x0, 0x38, 0x0, 0x0, 0xef, 0xff, 0xf3, 0x7f,
    0xff, 0xbb, 0xff, 0xfd, 0xf0, 0x0, 0x7, 0xe0,
    0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8,
    0x0, 0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e,
    0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0, 0x1f,
    0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7,
    0xef, 0xff, 0xcf, 0x7f, 0xff, 0xcf, 0xff, 0xff,
    0xc0,

    /* U+0037 "7" */
    0xff, 0xff, 0xf4, 0xff, 0xff, 0xdc, 0xff, 0xfe,
    0xf8, 0x0, 0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0,
    0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0,
    0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0,
    0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0,
    0x1, 0xf8, 0x0, 0x3, 0xe0, 0x0, 0x1, 0x0,
    0x0, 0x30, 0x0, 0x1, 0xc0, 0x0, 0x7, 0x0,
    0x0, 0x1c, 0x0, 0x0, 0x70, 0x0, 0x1, 0xc0,
    0x0, 0x7, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x70,
    0x0, 0x1, 0xc0, 0x0, 0x7, 0x0, 0x0, 0x1c,
    0x0, 0x0, 0x70, 0x0, 0x1, 0xc0, 0x0, 0x7,
    0x0, 0x0, 0xc, 0x0, 0x0, 0x0,

    /* U+0038 "8" */
    0xff, 0xff, 0xf4, 0xff, 0xff, 0xdc, 0xff, 0xfe,
    0xf8, 0x0, 0x3, 0xe0, 0x0, 0x1f, 0x80, 0x0,
    0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0,
    0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0,
    0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0,
    0x1, 0xf8, 0x0, 0x3, 0xef, 0xff, 0xf7, 0x7f,
    0xff, 0xbb, 0xff, 0xfd, 0xf0, 0x0, 0x7, 0xe0,
    0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8,
    0x0, 0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e,
    0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0, 0x1f,
    0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7,
    0xef, 0xff, 0xcf, 0x7f, 0xff, 0xcf, 0xff, 0xff,
    0xc0,

    /* U+0039 "9" */
    0xff, 0xff, 0xf4, 0xff, 0xff, 0xdc, 0xff, 0xfe,
    0xf8, 0x0, 0x3, 0xe0, 0x0, 0x1f, 0x80, 0x0,
    0x7e, 0x0, 0x1, 0xf8, 0x0, 0x7, 0xe0, 0x0,
    0x1f, 0x80, 0x0, 0x7e, 0x0, 0x1, 0xf8, 0x0,
    0x7, 0xe0, 0x0, 0x1f, 0x80, 0x0, 0x7e, 0x0,
    0x1, 0xf8, 0x0, 0x3, 0xef, 0xff, 0xf7, 0x7f,
    0xff, 0xb3, 0xff, 0xfd, 0xc0, 0x0, 0x7, 0x0,
    0x0, 0x1c, 0x0, 0x0, 0x70, 0x0, 0x1, 0xc0,
    0x0, 0x7, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x70,
    0x0, 0x1, 0xc0, 0x0, 0x7, 0x0, 0x0, 0x1c,
    0x0, 0x0, 0x70, 0x0, 0x1, 0xc0, 0x0, 0x7,
    0xf, 0xff, 0xcc, 0x7f, 0xff, 0xc3, 0xff, 0xff,
    0xc0,

    /* U+003A ":" */
    0x6f, 0xf6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6,
    0xff, 0x60
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 112, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 457, .box_w = 17, .box_h = 3, .ofs_x = 6, .ofs_y = 16},
    {.bitmap_index = 8, .adv_w = 0, .box_w = 4, .box_h = 4, .ofs_x = -2, .ofs_y = 0},
    {.bitmap_index = 10, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 457, .box_w = 3, .box_h = 34, .ofs_x = 22, .ofs_y = 1},
    {.bitmap_index = 120, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 457, .box_w = 21, .box_h = 35, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 457, .box_w = 22, .box_h = 34, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 403, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 457, .box_w = 22, .box_h = 34, .ofs_x = 3, .ofs_y = 1},
    {.bitmap_index = 691, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 788, .adv_w = 457, .box_w = 22, .box_h = 35, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 885, .adv_w = 112, .box_w = 4, .box_h = 19, .ofs_x = 1, .ofs_y = 8}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xd, 0xe
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 15, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 48, .range_length = 11, .glyph_id_start = 4,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t dseg35 = {
#else
lv_font_t dseg35 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 35,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -4,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if DSEG35*/

