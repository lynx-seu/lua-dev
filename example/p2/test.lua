
local array = require "array"
local a = array.new(10)
a:set(1, 5)
print(a:get(1), a:size())
