
local function print_array(result)
    local array_str = '{'
    local sep = ''

    local sz = #result
    for i = 1, sz do
        array_str = array_str .. sep .. result[i]
        sep = ', '
    end
    array_str = array_str .. '}'
    print(array_str)
end

local assert = assert
local dlx = require "dlx"

local a = dlx.new(3, 3)
a:addrow(1, {1, 0, 0})
a:addrow(2, {1, 0, 1})
a:addrow(3, {0, 1, 1})

local result = a:dance()

for _, v in ipairs(result) do
    print_array(v)
end
