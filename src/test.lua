
local pango = require 'libcairo'

--print(pango.write (nil, '<span>hello\nworld<sub>12<sub>3</sub></sub></span>', true))
--

local fontmap = pango.pango_cairo_font_map_get_default ()
local ctx = pango.pango_font_map_create_context (fontmap)

for i = 1, 10 do print(pango.dummy_layout (ctx, '<span>hello<sub>12</sub></span>')) end

pango.g_object_unref (ctx)
--pango.g_object_unref (fontmap)

print 'finishing'