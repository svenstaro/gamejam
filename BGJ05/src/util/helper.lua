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

function pack(...)
    return {...}
end

function setLightRendering(enabled, notAdditive)
    if enabled then
        love.graphics.setCanvas(LIGHT_CANVAS)
        if not notAdditive then love.graphics.setBlendMode("additive") end
    else
        love.graphics.setCanvas()
        love.graphics.setBlendMode("alpha")
    end
end

function drawLights()
    love.graphics.setColor(255, 255, 255, 180)
    love.graphics.setBlendMode("multiplicative")
    love.graphics.draw(LIGHT_CANVAS, 0, 0)
    love.graphics.setColor(255, 255, 255, 50)
    love.graphics.setBlendMode("additive")
    love.graphics.draw(LIGHT_CANVAS, 0, 0)
    love.graphics.setBlendMode("alpha")
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
        setmetatable(obj, cls)
        if obj.__init then obj:__init(...) end
        obj.__class = cls
        return obj
    end})
    return cls
end

function inherits(obj, name)
    local c = obj.__class
    while c do
        if c.__name == name then return true end
        c = c.__super
    end
    return false
end

function randf(from, to)
    if from then
        if to then
            return from + math.random() * (to - from)
        else
            return from * math.random()
        end
    else
        return randf(0, 1)
    end
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

function fadeColor(a, b, s)
    return
        a[1]*(1-s)+b[1]*s,
        a[2]*(1-s)+b[2]*s,
        a[3]*(1-s)+b[3]*s
end
