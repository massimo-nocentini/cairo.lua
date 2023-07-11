

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <poppler/glib/poppler.h>
#include <time.h>
#include <math.h>
#include <pango/pangocairo.h>
#include <cairo/cairo.h>

int l_pango_text_with_attrlist_spec(lua_State *L)
{
    cairo_t *cr = (cairo_t *)lua_touserdata(L, 1);
    const char *text = lua_tostring(L, 2);
    const char *attrlist_spec = lua_tostring(L, 3);
    int show = lua_toboolean(L, 4);

    // PangoFontMap *map = pango_cairo_font_map_get_default();
    // PangoContext *ctx = pango_font_map_create_context(map);
    // PangoLayout *layout = pango_layout_new(ctx);

    PangoAttrList *attrlist = pango_attr_list_from_string(attrlist_spec);

    PangoLayout *layout = pango_cairo_create_layout(cr);

    pango_layout_set_text(layout, text, -1);
    pango_layout_set_attributes(layout, attrlist);
    pango_cairo_update_layout(cr, layout);

    PangoRectangle rect;

    pango_layout_get_pixel_extents(layout, NULL, &rect);

    if (show)
        pango_cairo_show_layout(cr, layout);

    pango_attr_list_unref(attrlist);
    g_object_unref(layout);
    // g_object_unref(ctx);

    lua_pushinteger(L, rect.x);
    lua_pushinteger(L, rect.y);
    lua_pushinteger(L, rect.width);
    lua_pushinteger(L, rect.height);
    printf("\nFinished\n");
    return 4;
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

int l_pango_layout_with_default_font_map(lua_State *L)
{
    PangoFontMap *map = pango_cairo_font_map_get_default();
    PangoContext *ctx = pango_font_map_create_context(map);
    PangoLayout *layout = pango_layout_new(ctx);

    assert(PANGO_IS_LAYOUT(layout));

    lua_pushlightuserdata(L, layout);
    lua_pushlightuserdata(L, ctx);
    lua_pushlightuserdata(L, map);

    return 3;
}

int l_g_object_unref(lua_State *L)
{
    gpointer ptr = lua_touserdata(L, 1);

    g_object_unref(ptr);

    return 0;
}

int l_pango_layout_get_extents(lua_State *L)
{
    PangoLayout *layout = (PangoLayout *)lua_touserdata(L, 1);
    int logical = lua_toboolean(L, 2);

    PangoRectangle rect;

    if (logical)
        pango_layout_get_pixel_extents(layout, NULL, &rect);
    else
        pango_layout_get_pixel_extents(layout, &rect, NULL);

    lua_pushnumber(L, (rect.x));
    lua_pushnumber(L, (rect.y));
    lua_pushnumber(L, (rect.width));
    lua_pushnumber(L, (rect.height));

    return 4;
}

int l_pango_attr_list_splice(lua_State *L)
{
    PangoAttrList *list = (PangoAttrList *)lua_touserdata(L, 1);
    PangoAttrList *other = (PangoAttrList *)lua_touserdata(L, 2);

    PangoAttrList *join = pango_attr_list_new();

    /*
        For backwards compatibility, the function behaves differently when len is 0.
        In this case, the attributes from other are not imited to len, and are just overlayed on top of list.

        This mode is useful for merging two lists of attributes together.
    */

    pango_attr_list_splice(join, other, -1, 0);
    pango_attr_list_splice(join, list, -1, 0);

    lua_pushlightuserdata(L, join);

    return 1;
}

int l_pango_attr_list_from_string(lua_State *L)
{
    const char *spec = lua_tostring(L, 1);

    PangoAttrList *attributes = pango_attr_list_from_string(spec);

    lua_pushlightuserdata(L, attributes);

    return 1;
}

int l_pango_attr_list_unref(lua_State *L)
{
    PangoAttrList *attributes = (PangoAttrList *)lua_touserdata(L, 1);

    pango_attr_list_unref(attributes);

    return 0;
}

typedef struct
{
    PangoAttrList *list;
    int nel;
    int start;
} pango_attr_list_map_t;

void translate_pango_attribute(gpointer data, gpointer user_data)
{
    PangoAttribute *attr = (PangoAttribute *)data;
    pango_attr_list_map_t *m = (pango_attr_list_map_t *)user_data;

    m->nel++;

    int s = m->start;

    attr->start_index += s;
    attr->end_index += s;

    pango_attr_list_insert(m->list, attr);
}

int l_g_markup_escape_text(lua_State *L)
{
    const char *str = lua_tostring(L, 1);
    gchar *escaped = g_markup_escape_text(str, -1);

    lua_pushstring(L, escaped);

    g_free(escaped);

    return 1;
}

int l_pango_parse_markup(lua_State *L)
{
    const char *markup_text = lua_tostring(L, 1);
    int start = lua_tointeger(L, 2);
    PangoAttrList *empty_attr_list = (PangoAttrList *)lua_touserdata(L, 3);

    char *text;
    PangoAttrList *attr_list;

    gboolean res = pango_parse_markup(markup_text, -1, 0, &attr_list, &text, NULL, NULL);

    lua_pushboolean(L, res == TRUE);

    if (res == TRUE)
    {
        pango_attr_list_map_t m;
        m.list = empty_attr_list;
        m.start = start;
        m.nel = 0;

        GSList *list = pango_attr_list_get_attributes(attr_list);
        g_slist_foreach(list, &translate_pango_attribute, &m);
        g_slist_free(list);

        char *attrs_string = pango_attr_list_to_string(m.list);
        lua_pushstring(L, text);
        lua_pushstring(L, attrs_string);
        lua_pushinteger(L, m.nel);

        free(text);
        free(attrs_string);

        pango_attr_list_unref(attr_list);
    }
    else
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushinteger(L, 0);
    }

    return 4;
}

int l_poppler_document_new_from_file(lua_State *L)
{
    cairo_t *cr = lua_isnil(L, 1) ? NULL : (cairo_t *)lua_touserdata(L, 1);
    const char *absolute = lua_tostring(L, 2);
    int page_num = lua_isinteger(L, 3) ? lua_tointeger(L, 3) : 1;
    int show = lua_isboolean(L, 4) ? lua_toboolean(L, 4) : 0;

    PopplerDocument *document;
    PopplerPage *page;
    double width, height;
    GError *error;
    int num_pages;

    char *uri = g_filename_to_uri(absolute, NULL, &error);
    if (uri == NULL)
    {
        luaL_error(L, error->message);
    }

    
    document = poppler_document_new_from_file(uri, NULL, &error);
    printf("--------------URI: %s\n", uri);

    if (document == NULL)
    {
        luaL_error(L, error->message);
    }

    num_pages = poppler_document_get_n_pages(document);
    if (page_num < 1 || page_num > num_pages)
    {
        luaL_error(L, "page must be between 1 and %d.", num_pages);
    }

    page = poppler_document_get_page(document, page_num - 1);
    if (page == NULL)
    {
        luaL_error(L, "poppler fail: page not found.");
    }

    poppler_page_get_size(page, &width, &height);

    if (show)
    {
        cairo_save(cr);
        poppler_page_render(page, cr);
        cairo_restore(cr);
    }

    g_free(uri);
    // g_object_unref(page);
    // g_object_unref(document);

    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    lua_pushlightuserdata (L, page);
    lua_pushlightuserdata (L, document);

    return 4;
}

static const struct luaL_Reg libcairo[] = {
    {"pango_text_with_attrlist_spec", l_pango_text_with_attrlist_spec},
    {"pango_cairo_create_layout", l_pango_cairo_create_layout},
    {"pango_layout_with_default_font_map", l_pango_layout_with_default_font_map},
    {"g_object_unref", l_g_object_unref},
    {"pango_cairo_font_map_get_default", l_pango_cairo_font_map_get_default},
    {"pango_cairo_font_map_new", l_pango_cairo_font_map_new},
    {"pango_layout_new", l_pango_layout_new},
    {"pango_font_map_create_context", l_pango_font_map_create_context},
    {"pango_layout_get_extents", l_pango_layout_get_extents},
    {"pango_attr_list_from_string", l_pango_attr_list_from_string},
    {"pango_attr_list_unref", l_pango_attr_list_unref},
    {"pango_parse_markup", l_pango_parse_markup},
    {"pango_attr_list_splice", l_pango_attr_list_splice},
    {"g_markup_escape_text", l_g_markup_escape_text},
    {"poppler_document_new_from_file", l_poppler_document_new_from_file},

    {NULL, NULL} /* sentinel */
};

int luaopen_libcairo(lua_State *L)
{
    luaL_newlib(L, libcairo);
    return 1;
}
