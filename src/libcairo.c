

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <time.h>
#include <math.h>
#include <pango-1.0/pango/pangocairo.h>

int l_write(lua_State *L)
{

    cairo_surface_t *surf = (cairo_surface_t *)lua_touserdata(L, 1);
    cairo_t *cr = (cairo_t *)lua_touserdata(L, 2);
    const char *text = lua_tostring(L, 3);

    assert(cr != NULL);

    PangoLayout *layout;        // layout for a paragraph of text
    PangoFontDescription *desc; // this structure stores a description of the style of font you'd most like

    cairo_translate(cr, 10, 20);                               // set the origin of cairo instance 'cr' to (10,20) (i.e. this is where
                                                               // drawing will start from).
    layout = pango_cairo_create_layout(cr);                    // init pango layout ready for use
    pango_layout_set_text(layout, text, -1);                   // sets the text to be associated with the layout (final arg is length, -1
                                                               // to calculate automatically when passing a nul-terminated string)
    desc = pango_font_description_from_string("Sans Bold 12"); // specify the font that would be ideal for your particular use
    pango_layout_set_font_description(layout, desc);           // assign the previous font description to the layout
    pango_font_description_free(desc);                         // free the description

    // cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);                // set the colour to blue
    pango_cairo_update_layout(cr, layout); // if the target surface or transformation properties of the cairo instance
                                           // have changed, update the pango layout to reflect this
    pango_cairo_show_layout(cr, layout);   // draw the pango layout onto the cairo surface

    //g_object_unref(layout); // free the layout

    free (layout);

    cairo_surface_write_to_png(surf, "a.png");

    return 0;
}

static const struct luaL_Reg libcairo[] = {
    {"write", l_write},
    {NULL, NULL} /* sentinel */
};

int luaopen_libcairo(lua_State *L)
{
    luaL_newlib(L, libcairo);
    return 1;
}
