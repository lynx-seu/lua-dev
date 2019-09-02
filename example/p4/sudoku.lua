
local R, C = 9, 9

local dlx = require "dlx"

local sudoku = {} sudoku.__index = sudoku
local function new(str)
    local sz = #str
    assert(sz == R*C)

    local o = {
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
    }
    for i = 1, sz do
        local v = str:sub(i, i)
        o[i] = tonumber(v)
    end
    return setmetatable(o, sudoku)
end

function sudoku:__tostring()
    local str = '{\n'

    for r = 1, R do
        local sep = '\t'
        for c = 1, C do
            local i = (r-1)*C + c
            str = str .. sep .. self[i]
            sep = ' '
        end
        str = str .. '\n'
    end

    str = str .. '}'
    return str
end


-- TEST
--local s1 = new "800000000003600000070090200050007000000045700000100030001000068008500010090000400"
local s1 = new "000000000000000000000000000000000000000000000000000000000000000000000000000000000"
print(s1)

local dlx_matrix = dlx.new(729, 324)
local row = 1

for r = 1, R do
    for c = 1, C do
        local index = (r-1)*C + c
        local v = s1[index]
        for i = 1, 9 do
            if v == 0 or v == i then
                -- 唯一约束:每个空格填入一个数
                dlx_matrix:set(row, (r-1)*C+c, 1)
                -- 行约束: 每行 1-9 唯一
                dlx_matrix:set(row, (r-1)*R+i+81, 1)
                -- 列约束: 每行 1-9 唯一
                dlx_matrix:set(row, (c-1)*C+i+162, 1)
                -- 九宫约束
                local r1 = math.floor((r-1)/3) + 1
                local c1 = math.floor((c-1)/3) + 1
                local block = (r1-1)*3+c1
                dlx_matrix:set(row, (block-1)*9+i+243, 1)
            end

            row = row + 1
        end
    end
end

--local start = os.time()
--local str = ''
--for r = 1, 729 do
--    local sep = ''
--    for c = 1, 324 do
--        str = str .. sep .. dlx_matrix:get(r, c)
--        sep = ' '
--    end
--    str = str .. '\n'
--end
--print(str)
--print(os.difftime(os.time(),start))

local start = os.time()
local result
result = dlx_matrix:dance()
print(os.difftime(os.time(),start))

assert(#result == 1 and #(result[1]) == 81)
for _, x in ipairs(result[1]) do
    x = x - 1
    local v = x % 9 + 1
    x = math.floor(x/9)
    local c = x % 9 + 1
    local r = math.floor(x/9) + 1
    local i = (r-1)*C + c
    s1[i] = v
end
print(s1)


