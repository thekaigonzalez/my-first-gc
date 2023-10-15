/*Copyright 2019-2023 Kai D. Gonzalez*/

// simple attempt of a GC

#ifndef GC_H
#define GC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0
#define BOOL int

typedef struct gc_object
{
  void *ptr;
  struct gc_object *next;
  BOOL marked;
} gc_object;

typedef struct gc
{
  struct gc_object *tmp;
  gc_object *root; // root of the stack
  size_t depth;    // how deep the stack is
} gc;

gc *gc_init (void);
gc_object *gc_alloc (void);
void gc_gc_free (gc_object *o);
void gc_mark (gc_object *o);
void gc_add (gc *g, gc_object *o);
void gc_collect (gc *g);
BOOL gc_is_marked (gc_object *o);
void gc_set_data (gc_object *o, void *data);
void *gc_get_data (gc_object *o);
gc_object *gc_calculate_current (gc *g);
void gc_copy_object (gc_object *dest, gc_object *src);
void gc_dump (gc *g);

#endif
