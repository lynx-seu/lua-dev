
local assert = assert
local array = require "array"

local a = array.new(10)
a:set(1, 5)
assert(a:get(1) == 5 and a:size() == 10)
print(a:get(1), a:size())

