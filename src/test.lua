
local pango = require 'libcairo'

print(pango.write (nil, '<span>hello\nworld<sub>12<sub>3</sub></sub></span>', true))
