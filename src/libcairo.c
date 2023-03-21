

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <time.h>
#include <math.h>
#include <pango/pangocairo.h>
#include <pango/pangoft2.h>
#include <fontconfig/fontconfig.h>
#include <cairo/cairo.h>

#define RADIUS 150
#define N_WORDS 10
#define FONT "Ubuntu 12"

int l_write(lua_State *L)
{
    cairo_t *cr = (cairo_t *)lua_touserdata(L, 1);
    const char *text = lua_tostring(L, 2);
    int show = lua_toboolean(L, 3);

    // assert(cr != NULL);

    // FcBool fontAddStatus = FcConfigAppFontAddDir(FcConfigGetCurrent(), "/usr/share/fonts/truetype/ubuntu/");

    PangoLayout *layout;
    PangoFontDescription *desc;

    // cairo_surface_t *s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 200, 200);
    // cairo_surface_t *s = cairo_svg_surface_create("tmp.svg", 200, 200);
    // cairo_t *cr = cairo_create(s);

    /* Center coordinates on the middle of the region we are drawing */
    // cairo_translate(cr, RADIUS, RADIUS);

    /* Create a PangoLayout, set the font and text */
    layout = pango_cairo_create_layout(cr);
    // PangoFontMap *map = PANGO_CAIRO_FONT_MAP(pango_cairo_font_map_new());
    // PangoContext *ctx = pango_cairo_font_map_create_context(map);
    // layout = pango_layout_new(ctx);

    pango_layout_set_markup(layout, text, -1);
    // desc = pango_font_description_from_string(FONT);
    // pango_layout_set_font_description(layout, desc);

    /* Inform Pango to re-layout the text with the new transformation */
    pango_cairo_update_layout(cr, layout);

    int width, height, baseline;

    pango_layout_get_size(layout, &width, &height);
    baseline = pango_layout_get_baseline(layout);

    double tw = ((double)-width) / (PANGO_SCALE * 2.0);
    double th = ((double)-(height)) / (PANGO_SCALE * 2.0);

    if (show)
    {
        // cairo_translate(cr, tw, th);

        pango_cairo_show_layout(cr, layout);

        // cairo_translate(cr, -tw, -th);
    }

    // pango_font_description_free(desc);
    //  g_object_unref(desc);
    //  desc = NULL;

    // cairo_surface_flush(s);

    /* free the layout object */
    // g_object_unref(layout);
    layout = NULL;

    // cairo_surface_destroy(s);

    lua_pushnumber(L, width / (double)PANGO_SCALE);
    lua_pushnumber(L, height / (double)PANGO_SCALE);
    lua_pushnumber(L, baseline / (double)PANGO_SCALE);

    return 3;
}

int l_pango_layout_set_markup(lua_State *L)
{
    cairo_t *cr = (cairo_t *)lua_touserdata(L, 1);
    PangoLayout *layout = (PangoLayout *)lua_touserdata(L, 2);
    const char *text = lua_tostring(L, 3);

    assert(layout != NULL);

    assert(PANGO_IS_LAYOUT(layout));

    printf("_____________before\n");

    pango_layout_set_markup(layout, text, -1);
    pango_cairo_update_layout(cr, layout);

    int width, height, baseline;

    pango_layout_get_size(layout, &width, &height);
    baseline = pango_layout_get_baseline(layout);

    lua_pushnumber(L, width / (double)PANGO_SCALE);
    lua_pushnumber(L, height / (double)PANGO_SCALE);
    lua_pushnumber(L, baseline / (double)PANGO_SCALE);

    return 3;
}

int l_pango_is_layout(lua_State *L)
{
    PangoLayout *layout = (PangoLayout *)lua_touserdata(L, 1);

    int islayout = PANGO_IS_LAYOUT(layout);

    lua_pushboolean(L, islayout);

    return 1;
}

int l_pango_cairo_create_layout(lua_State *L)
{
    cairo_t *cr = (cairo_t *)lua_touserdata(L, 1);
    PangoLayout *layout = pango_cairo_create_layout(cr);

    assert(PANGO_IS_LAYOUT(layout));

    lua_pushlightuserdata(L, layout);

    return 1;
}

int l_pango_font_map_create_context(lua_State *L)
{
    PangoFontMap *map = (PangoFontMap *)lua_touserdata(L, 1);
    PangoContext *ctx = pango_font_map_create_context(map);

    lua_pushlightuserdata(L, ctx);

    return 1;
}

int l_pango_cairo_font_map_new(lua_State *L)
{
    PangoFontMap *map = pango_cairo_font_map_new();
    lua_pushlightuserdata(L, map);

    return 1;
}

int l_pango_cairo_font_map_get_default(lua_State *L)
{
    PangoFontMap *map = pango_cairo_font_map_get_default();
    lua_pushlightuserdata(L, map);

    return 1;
}

int l_pango_layout_new(lua_State *L)
{
    PangoContext *ctx = (PangoContext *)lua_touserdata(L, 1);
    PangoLayout *layout = pango_layout_new(ctx);

    lua_pushlightuserdata(L, layout);

    return 1;
}

int l_dummy_test(lua_State *L)
{
    const char *markup = lua_tostring(L, 1);

    PangoFontMap *map = pango_cairo_font_map_get_default();
    PangoContext *ctx = pango_font_map_create_context(map);
    PangoLayout *layout = pango_layout_new(ctx);
    PangoFontDescription *desc = pango_font_description_from_string("Lucida Grande 12");

    pango_layout_set_font_description(layout, desc);
    pango_layout_set_markup(layout, markup, -1);

    PangoRectangle rect;

    pango_layout_get_extents(layout, NULL, &rect);

    pango_font_description_free(desc);

    g_object_unref(layout);
    g_object_unref(ctx);

    lua_pushnumber(L, (lua_Number)rect.x / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.y / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.width / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.height / PANGO_SCALE);

    return 4;
}

int l_dummy_layout(lua_State *L)
{
    PangoContext *ctx = (PangoContext *)lua_touserdata(L, 1);
    const char *markup = lua_tostring(L, 2);

    PangoLayout *layout = pango_layout_new(ctx);
    PangoFontDescription *desc = pango_font_description_from_string(FONT);

    pango_layout_set_font_description(layout, desc);
    pango_layout_set_markup(layout, markup, -1);

    PangoRectangle rect;

    pango_layout_get_extents(layout, NULL, &rect);

    pango_font_description_free(desc);

    g_object_unref(layout);

    lua_pushnumber(L, (lua_Number)rect.x / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.y / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.width / PANGO_SCALE);
    lua_pushnumber(L, (lua_Number)rect.height / PANGO_SCALE);

    return 4;
}

int l_pango_layout_with_default_font_map(lua_State *L)
{
    PangoFontMap *map = pango_cairo_font_map_get_default();
    PangoContext *ctx = pango_font_map_create_context(map);
    PangoLayout *layout = pango_layout_new(ctx);

    assert(PANGO_IS_LAYOUT(layout));

    lua_pushlightuserdata(L, layout);
    lua_pushlightuserdata(L, ctx);
    lua_pushlightuserdata(L, map);

    pango_layout_set_text(layout, "", 0);

    return 3;
}

int l_g_object_unref(lua_State *L)
{
    gpointer ptr = lua_touserdata(L, 1);

    g_object_unref(ptr);

    return 0;
}

static const struct luaL_Reg libcairo[] = {
    {"write", l_write},
    {"pango_cairo_create_layout", l_pango_cairo_create_layout},
    {"pango_layout_with_default_font_map", l_pango_layout_with_default_font_map},
    {"pango_layout_set_markup", l_pango_layout_set_markup},
    {"g_object_unref", l_g_object_unref},
    {"pango_is_layout", l_pango_is_layout},
    {"pango_cairo_font_map_get_default", l_pango_cairo_font_map_get_default},
    {"pango_cairo_font_map_new", l_pango_cairo_font_map_new},
    {"pango_layout_new", l_pango_layout_new},
    {"dummy_test", l_dummy_test},
    {"dummy_layout", l_dummy_layout},
    {"pango_font_map_create_context", l_pango_font_map_create_context},
    {NULL, NULL} /* sentinel */
};

int luaopen_libcairo(lua_State *L)
{
    luaL_newlib(L, libcairo);
    return 1;
}
