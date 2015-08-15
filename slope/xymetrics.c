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

#include "slope/xymetrics_p.h"
#include "slope/funcplot_p.h"
#include "slope/list.h"
#include <cairo.h>
#include <stdlib.h>


slope_metrics_class_t* _slope_xymetrics_get_class()
{
  static int first_call = SLOPE_TRUE;
  static slope_metrics_class_t klass;

  if (first_call) {
    klass.destroy_fn = _slope_xymetrics_destroy;
    klass.update_fn = _slope_xymetrics_update;
    klass.draw_fn = _slope_xymetrics_draw;
    first_call = SLOPE_FALSE;
  }

  return &klass;
}


slope_metrics_t* slope_xymetrics_create()
{
  slope_xymetrics_t *self = malloc(sizeof(slope_xymetrics_t));
  slope_metrics_t *metrics = (slope_metrics_t*) self;

  metrics->klass = _slope_xymetrics_get_class();
  metrics->type = SLOPE_XYMETRICS;
  metrics->visible = SLOPE_TRUE;
  metrics->item_list = NULL;
  metrics->figure = NULL;

  metrics->x_low_bound = metrics->x_up_bound = 85.0;
  metrics->y_low_bound = metrics->y_up_bound = 55.0;

  self->axis_list = NULL;
  slope_item_t *axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_TOP, "");
  self->axis_list = slope_list_append(self->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_BOTTOM, "X");
  self->axis_list = slope_list_append(self->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_LEFT, "Y");
  self->axis_list = slope_list_append(self->axis_list, axis);
  axis = slope_xyaxis_create(
      metrics, SLOPE_XYAXIS_RIGHT, "");
  self->axis_list = slope_list_append(self->axis_list, axis);

  slope_metrics_update(metrics);
  return metrics;
}


void _slope_xymetrics_destroy (slope_metrics_t *metrics)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_iterator_t *axis_iter;

  /* destroy axis */
  SLOPE_LIST_FOREACH (axis_iter, self->axis_list) {
    slope_item_t *axis = (slope_item_t*) slope_iterator_data(axis_iter);
    slope_item_destroy(axis);
  }
}


void
_slope_xymetrics_draw (slope_metrics_t *metrics, cairo_t *cr,
                       const slope_rect_t *rect)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_iterator_t *iter;

  metrics->xmin_figure = rect->x + metrics->x_low_bound;
  metrics->ymin_figure = rect->y + metrics->y_low_bound;
  metrics->xmax_figure = rect->x + rect->width - metrics->x_up_bound;
  metrics->ymax_figure = rect->y + rect->height - metrics->y_up_bound;
  metrics->width_figure = metrics->xmax_figure - metrics->xmin_figure;
  metrics->height_figure = metrics->ymax_figure - metrics->ymin_figure;

  cairo_rectangle(
      cr, metrics->xmin_figure, metrics->ymin_figure,
      metrics->width_figure, metrics->height_figure);
  cairo_save(cr);
  cairo_clip(cr);

  /* draw user item */
  SLOPE_LIST_FOREACH (iter, metrics->item_list) {
    slope_item_t *item = (slope_item_t*) slope_iterator_data(iter);
    if (slope_item_get_visible(item)) {
      _slope_item_draw(item, cr, metrics);
    }
  }
  cairo_restore(cr);

  /* draw axis */
  SLOPE_LIST_FOREACH (iter, self->axis_list) {
    slope_item_t *axis = (slope_item_t*) slope_iterator_data(iter);
    if (slope_item_get_visible(axis)) {
      _slope_item_draw(axis, cr, metrics);
    }
  }
}


void _slope_xymetrics_update (slope_metrics_t *metrics)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_iterator_t *iterator = NULL;
  slope_item_t *item = NULL;
  slope_rect_t rect;

  if (metrics->item_list == NULL) {
    self->xmin = 0.0;
    self->xmax = 1.0;
    self->ymin = 0.0;
    self->ymax = 1.0;
    self->width = self->xmax - self->xmin;
    self->height = self->ymax - self->ymin;
    return;
  }

  iterator = slope_list_first(metrics->item_list);
  item = (slope_item_t*) slope_iterator_data(iterator);

  while (slope_item_get_rescalable(item) == SLOPE_FALSE) {
    slope_iterator_next(&iterator);
    item = (slope_item_t*) slope_iterator_data(iterator);
  }

  slope_item_get_rect(item, &rect);

  self->xmin = rect.x;
  self->xmax = rect.x + rect.width;
  self->ymin = rect.y;
  self->ymax = rect.y + rect.height;

  slope_iterator_next(&iterator);

  while (iterator) {
    item = (slope_item_t*) slope_iterator_data(iterator);

    if (slope_item_get_rescalable(item) == SLOPE_FALSE) {
      slope_iterator_next(&iterator);
      continue;
    }

    slope_item_get_rect(item, &rect);

    if (rect.x               < self->xmin) self->xmin = rect.x;
    if (rect.x + rect.width  > self->xmax) self->xmax = rect.x + rect.width;
    if (rect.y               < self->ymin) self->ymin = rect.y;
    if (rect.y + rect.height < self->ymax) self->ymax = rect.y + rect.height;

    slope_iterator_next(&iterator);
  }

  double xbound = (self->xmax - self->xmin) /20.0;
  self->xmin -= xbound;
  self->xmax += xbound;
  double ybound = (self->ymax - self->ymin) /20.0;
  self->ymin -= ybound;
  self->ymax += ybound;
  self->width = self->xmax - self->xmin;
  self->height = self->ymax - self->ymin;
}


double
slope_xymetrics_map_x (const slope_metrics_t *metrics, double x)
{
  const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
  double tmp = (x - self->xmin) /self->width;
  return metrics->xmin_figure + tmp*metrics->width_figure;
}


double
slope_xymetrics_map_y (const slope_metrics_t *metrics, double y)
{
  const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
  double tmp = (y - self->ymin) /self->height;
  return metrics->ymax_figure - tmp*metrics->height_figure;
}


double
slope_xymetrics_unmap_x (const slope_metrics_t *metrics, double x)
{
  const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
  double tmp = (x - metrics->xmin_figure) /metrics->width_figure;
  return self->xmin + tmp*self->width;
}


double
slope_xymetrics_unmap_y (const slope_metrics_t *metrics, double y)
{
  const slope_xymetrics_t *self = (const slope_xymetrics_t*) metrics;
  double tmp = (metrics->ymax_figure - y) /metrics->height_figure;
  return self->ymin + tmp*self->height;
}


slope_item_t*
slope_xymetrics_get_axis (slope_metrics_t *metrics,
                          slope_xyaxis_type_t type)
{
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  slope_iterator_t *axis_iter;

  SLOPE_LIST_FOREACH (axis_iter, self->axis_list) {
  slope_item_t *axis = (slope_item_t*) slope_iterator_data(axis_iter);
    if (slope_xyaxis_get_type(axis) == type) {
      return axis;
    }
  }
  return NULL;
}


void
slope_xymetrics_set_x_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
  if (metrics == NULL) {
    return;
  }
  metrics->x_low_bound = low;
  metrics->x_up_bound = hi;
}


void
slope_xymetrics_set_y_boundary (slope_metrics_t *metrics,
                                double low, double hi)
{
  if (metrics == NULL) {
    return;
  }
  metrics->y_low_bound = low;
  metrics->y_up_bound = hi;
}


void
slope_xymetrics_set_x_range (slope_metrics_t *metrics,
                             double xi, double xf)
{
  if (metrics == NULL) {
    return;
  }
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  self->xmin = xi;
  self->xmax = xf;
  self->width = self->xmax - self->xmin;
}


void
slope_xymetrics_set_y_range (slope_metrics_t *metrics,
                             double yi, double yf)
{
  if (metrics == NULL) {
    return;
  }
  slope_xymetrics_t *self = (slope_xymetrics_t*) metrics;
  self->ymin = yi;
  self->ymax = yf;
  self->height = self->ymax - self->ymin;
}

/* slope/xymetrics.h */
