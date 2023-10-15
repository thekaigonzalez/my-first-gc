/*Copyright 2019-2023 Kai D. Gonzalez*/

#include "gc.h"

const int VERBOSE = 0;

#define error(x)                                                              \
  fprintf (stderr, "%s: %s\n", __func__, x);                                  \
  exit (1);

#define note(x)                                                               \
  if (VERBOSE)                                                                \
    fprintf (stderr, "%s: NOTE: %s\n", __func__, x);

gc *
gc_init (void)
{
  gc *g = malloc (sizeof (gc));

  g->root = gc_alloc ();
  g->root->marked = true;
  g->root->next = NULL;
  g->tmp = NULL;

  g->depth = 1;

  return g;
}

gc_object *
gc_alloc (void)
{
  gc_object *o = malloc (sizeof (gc_object));

  o->ptr = NULL;
  o->next = NULL;
  o->marked = true;

  return o;
}

void
gc_free (gc_object *o)
{
  free (o);
  //   o = NULL;
}

void
gc_mark (gc_object *o)
{
  o->marked = true;
}

// sets the roots next and moves the current to the next
void
gc_add (gc *g, gc_object *o)
{
  gc_object *current = g->root;

  if (current == NULL)
    {
      error ("the root is null?");
    }

  if (g->depth == 1)
    {
      g->root = o;
    }
  else
    {
      for (int i = 1; i < g->depth; i++)
        {
          if (current->next == NULL && i < g->depth)
            {
              current->next = o;
            }
          current = current->next;

          if (current == NULL)
            {
              error ("(in the for loop) the current is null?")
            }
        }
      current = o;
    }

  g->depth++;
}

void
gc_collect (gc *g)
{
  gc_object *tmp;

  while (g->root != NULL)
    {
      tmp = g->root;
      g->root = g->root->next;
      if (VERBOSE)
        printf ("free %s\n", (char *)tmp->ptr);
      free (tmp);
      tmp = NULL;
    }
  if (tmp == NULL)
    {
      note ("attempting to free (already-null) temp")
    }
  free (g->tmp);
  g->tmp = NULL;
  free (g);
  g = NULL;
}

BOOL
gc_is_marked (gc_object *o)
{
  return o->marked;
}

void
gc_set_data (gc_object *o, void *data)
{
  if (o == NULL)
    {
      error ("must set an object that is properly allocated")
    }
  o->ptr = data;
}

void *
gc_get_data (gc_object *o)
{
  if (o == NULL)
    {
      error ("object not properly allocated")
    }
  return o->ptr;
}

gc_object *
gc_calculate_current (gc *g)
{
  if (g == NULL)
    {
      error ("g is NULL");
    }
  g->tmp = g->root;
  if (g->tmp == NULL)
    {
      error ("the root is null? (g->root)");
    }
  if (g->depth == 1)
    {
      return g->tmp;
    }

  for (int i = 0; i < g->depth; i++)
    {
      if (VERBOSE)
      printf ("depth %d\n", i);
      if (gc_is_marked (g->tmp))
        {
          if (g->tmp->next != NULL && g->tmp != NULL)
            {
              if (VERBOSE)
              printf ("object found: %s\n", (char *)g->tmp->ptr);
              g->tmp = g->tmp->next;
            }
        }
    }
  return g->tmp;
}

void
gc_copy_object (gc_object *dest, gc_object *src)
{
  if (dest == NULL || src == NULL)
    {
      error ("destination or source is null. can not copy properties")
    }
  dest->ptr = src->ptr;
  dest->next = src->next;
  dest->marked = src->marked;
}

void
gc_dump (gc *g)
{
  gc_object *o = g->root;

  if (o == NULL)
    {
      error ("there is either no root to print or the root is NULL") return;
    }

  for (int i = 1; i < g->depth; i++)
    {
      if (i == 1)
        printf ("%d: %s\n", i, (char *)o->ptr);
      else
        {
          if (o->next == NULL)
            {
              error ("depth and next do not align");
            }
          o = o->next;
          printf ("%d: %s\n", i, (char *)o->ptr);
        }
    }
}
