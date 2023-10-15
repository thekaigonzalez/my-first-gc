# garbage collector

a very very simple garbage collector that keeps track of all the objects
allocated and will remove them once they are no longer needed.

it allocates by default around 24 bytes to itself, no more are needed and no
more will be allocated

```c
#include "gc.h"

int main(void) {
    gc *g = gc_init();

    gc_object *o = gc_alloc();

    gc_set_data(o, "hello world!");
    gc_mark(o);
    gc_add(g, o);

    gc_object *z = gc_alloc();
    gc_set_data(z, "hello world again!");
    gc_mark(z);

    gc_add(g, z);

    gc_object *w = gc_alloc();
    gc_set_data(w, NULL);
    gc_mark(w);

    gc_add(g, w);

    gc_object *x = gc_alloc();
    gc_set_data(x, "hello world again! 3");
    gc_mark(x);

    gc_add(g, x);

    gc_dump(g);
    gc_collect(g);

    return 0;
}
```
