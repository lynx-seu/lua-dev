
local function print_array(result)
    local array_str = '{'
    local sep = ''

    local sz = #result
    for i = 1, sz do array_str = array_str .. sep .. result[i]
        sep = ', '
    end
    array_str = array_str .. '}'
    print(array_str)
end

local assert = assert
local dlx = require "dlx"

local a = dlx.new(4, 3)
--a:addrow(1, {1, 0, 0})
--a:addrow(2, {1, 0, 1})
--a:addrow(3, {0, 1, 1})
a:set(1, 1, 1)
a:set(2, 1, 1)
a:set(2, 3, 1)
a:set(3, 2, 1)
a:set(3, 3, 1)

local result = a:dance()

for _, v in ipairs(result) do
    print_array(v)
end

----Problem: hustoj 1017
local b = dlx.new(6, 7)
b:set(1, 1, 1)
b:set(1, 4, 1)
b:set(1, 7, 1)
b:set(2, 1, 1)
b:set(2, 4, 1)
b:set(3, 4, 1)
b:set(3, 5, 1)
b:set(3, 7, 1)
b:set(4, 3, 1)
b:set(4, 5, 1)
b:set(4, 6, 1)
b:set(5, 2, 1)
b:set(5, 3, 1)
b:set(5, 6, 1)
b:set(5, 7, 1)
b:set(6, 2, 1)
b:set(6, 7, 1)

local result = b:dance()
for _, v in ipairs(result) do
    print_array(v)
end
