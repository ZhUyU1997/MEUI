#pragma once

#include <plutovg.h>

plutovg_path_t *get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w);
plutovg_rect_t measure_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w);