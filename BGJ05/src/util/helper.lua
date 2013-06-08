-- helpers for lua coding

-- simplifies OOP
--[[
function class(name, superclass)
    local cls = superclass and superclass() or {}
    cls.__name = name or ""
    cls.__super = superclass
    return setmetatable(cls, {__call = function (c, ...)
        local self = setmetatable({__class = cls}, cls)
        if cls.__init then
            cls.__init(self, ...)
        end
        for k,v in pairs(cls) do
            self[k] = v
        end
        return self
    end})
end
]]--

function class(name, super)
    -- main metadata
    local cls = {}
    cls.__name = name
    cls.__super = super

    -- copy the members of the superclass
    if super then
        for k,v in pairs(super) do
            cls[k] = v
        end
    end

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
