-- helpers for lua coding

function math.round(num, decs)
    decs = decs or 0
    num = num * (10 ^ decs)
    local rem = num % 1
    num = num - rem
    if rem >= 0.5 then
        num = num + 1
    end
    return num / (10 ^ decs)
end

function dist(x1, y1, x2, y2)
    return math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))
end

function table.join(a, b)
    local tmp = {}
    for k, v in pairs(a) do
        table.insert(tmp, v)
    end
    for k, v in pairs(b) do
        table.insert(tmp, v)
    end
    return tmp
end

function table.getKey(tbl, val)
    for key, value in pairs(tbl) do
        if value == val then
            return key
        end
    end
    return -1
end

function table.removeKey(tbl, key)
    local val = tbl[key]
    table.remove(tbl, key)
    return val
end

function table.removeValue(tbl, val)
    local key = table.getKey(tbl, val)
    if key == -1 then return nil end
    return table.removeKey(tbl, key)
end

function table.containsKey(tbl, key)
    return tbl[key] ~= nil
end

function table.containsValue(tbl, val)
    return table.getKey(tbl, val) ~= -1
end

function class(name, super)
    -- main metadata
    local cls = {}

    -- copy the members of the superclass
    if super then
        for k,v in pairs(super) do
            cls[k] = v
        end
    end

    cls.__name = name
    cls.__super = super

    -- when the class object is being called,
    -- create a new object containing the class'
    -- members, calling its __init with the given
    -- params
    cls = setmetatable(cls, {__call = function(c, ...)
        local obj = {}
        for k,v in pairs(cls) do
            --if not k == "__call" then
                obj[k] = v
            --end
        end
        if obj.__init then obj:__init(...) end
        return obj
    end})
    return cls
end


-- Converts HSL to RGB (input and output range: 0 - 255)
function hsl2rgb(h, s, l)
if s == 0 then return l,l,l end
    h, s, l = h/256*6, s/255, l/255
    local c = (1-math.abs(2*l-1))*s
    local x = (1-math.abs(h%2-1))*c
    local m,r,g,b = (l-.5*c), 0,0,0
    if h < 1     then r,g,b = c,x,0
    elseif h < 2 then r,g,b = x,c,0
    elseif h < 3 then r,g,b = 0,c,x
    elseif h < 4 then r,g,b = 0,x,c
    elseif h < 5 then r,g,b = x,0,c
    else              r,g,b = c,0,x
    end
    return math.ceil((r+m)*256),math.ceil((g+m)*256),math.ceil((b+m)*256)
end
