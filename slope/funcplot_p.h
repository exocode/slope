/*
 * Copyright (C) 2015  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLOPE_FUNCPLOT_P_H
#define SLOPE_FUNCPLOT_P_H

#include "slope/funcplot.h"
#include "slope/item_p.h"

SLOPE_BEGIN_DECLS

typedef struct _slope_funcplot slope_funcplot_t;

struct _slope_funcplot
{
    slope_item_t    parent;
    int             rescalable;
    const double   *vx, *vy;
    int             n;
    double          xmin, xmax;
    double          ymin, ymax;
    slope_color_t   color;
    slope_scatter_t scatter;
    int             fill_symbol;
    int             antialias;
    double          line_width;
};

/**
 */
slope_item_class_t* _slope_funcplot_get_class();

void _slope_funcplot_init (slope_item_t *item);

/**
 */
void _slope_funcplot_draw (slope_item_t *item, cairo_t *cr,
                          const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_line (slope_item_t *item, cairo_t *cr,
                               const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_circles (slope_item_t *item, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_triangles (slope_item_t *item, cairo_t *cr,
                                    const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_squares (slope_item_t *item, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_plusses (slope_item_t *item, cairo_t *cr,
                                  const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_line_circles (slope_item_t *item, cairo_t *cr,
                                       const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_line_triangles (slope_item_t *item, cairo_t *cr,
                                         const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_line_plusses (slope_item_t *item, cairo_t *cr,
                                       const slope_metrics_t *metrics);

/**
 */
void _slope_funcplot_draw_thumb (slope_item_t *item,
                                const slope_point_t *pos, cairo_t *cr);

/**
 */
void _slope_funcplot_check_ranges (slope_item_t *item);

/**
 *
 */
void _slope_funcplot_get_rect (slope_item_t *item, slope_rect_t *rect);


SLOPE_END_DECLS

#endif /*SLOPE_FUNCPLOT_P_H */
