require("util/helper")

function assert_vector(v)
    assert(v ~= nil and v.__name == "Vector")
end

Vector = class("Vector")

function Vector:__init(x, y)
    self.x = x or 0
    self.y = y or 0
end

function Vector:clone()
    return Vector(self.x, self.y)
end

function Vector:unpack()
	return self.x, self.y
end

function Vector:__tostring()
	return "[" .. tonumber(self.x) .. " | " .. tonumber(self.y) .. "]"
end

function Vector.__unm(a)
	return Vector(-a.x, -a.y)
end

function Vector.__add(a, b)
    assert_vector(a)
    assert_vector(b)
	return Vector(a.x + b.x, a.y + b.y)
end

function Vector.__sub(a, b)
    assert_vector(a)
    assert_vector(b)
	return Vector(a.x - b.x, a.y - b.y)
end

function Vector.__mul(a, b)
	if type(a) == "number" then
		return Vector(a*b.x, a*b.y)
	elseif type(b) == "number" then
		return Vector(b*a.x, b*a.y)
	else
        assert_vector(a)
        assert_vector(b)
		return a.x*b.x + a.y*b.y
	end
end

function Vector.__div(a, b)
    assert_vector(a)
    assert(type(b) == "number", "Not a number: " .. b)
	return Vector(a.x / b, a.y / b)
end

function Vector.__eq(a, b)
    assert_vector(a)
    assert_vector(b)
	return a.x == b.x and a.y == b.y
end

function Vector.__lt(a,b)
    assert_vector(a)
    assert_vector(b)
	return a.x < b.x or (a.x == b.x and a.y < b.y)
end

function Vector.__le(a,b)
    assert_vector(a)
    assert_vector(b)
	return a.x <= b.x and a.y <= b.y
end

function Vector.permul(a,b)
    assert_vector(a)
    assert_vector(b)
	return Vector(a.x*b.x, a.y*b.y)
end

function Vector:len2()
	return self.x * self.x + self.y * self.y
end

function Vector:len()
	return math.sqrt(self:len2())
end

function Vector.dist(a, b)
    assert_vector(a)
    assert_vector(b)
	local dx = a.x - b.x
	local dy = a.y - b.y
	return math.sqrt(dx * dx + dy * dy)
end

function Vector:normalize()
	local l = self:len()
	if l > 0 then
		self.x, self.y = self.x / l, self.y / l
	end
	return self
end

function Vector:normalized()
	return self:clone():normalize()
end

function Vector:rotate(phi)
	local c, s = math.cos(phi), math.sin(phi)
	self.x, self.y = c * self.x - s * self.y, s * self.x + c * self.y
	return self
end

function Vector:rotated(phi)
	local c, s = math.cos(phi), math.sin(phi)
	return Vector(c * self.x - s * self.y, s * self.x + c * self.y)
end

function Vector:angleTo(v2)
    local quotient = (self:len() * v2:len())
    if quotient ~= 0 then
        local input = (self * v2) / quotient
        return math.acos(input)
    else
        return 0
    end
end

function Vector:perpendicular()
	return Vector(-self.y, self.x)
end

function Vector:projectOn(v)
    assert_vector(v)
	-- (self * v) * v / v:len2()
	local s = (self.x * v.x + self.y * v.y) / (v.x * v.x + v.y * v.y)
	return Vector(s * v.x, s * v.y)
end

function Vector:mirrorOn(v)
    assert_vector(v)
	local s = 2 * (self.x * v.x + self.y * v.y) / (v.x * v.x + v.y * v.y)
	return Vector(s * v.x - self.x, s * v.y - self.y)
end

function Vector:cross(v)
    assert_vector(v)
	return self.x * v.y - self.y * v.x
end

function Vector:unpack()
    return self.x, self.y
end

-- returns the mouse position in entity space
function getMouseVector()
    local x, y = love.mouse.getPosition()
    return (Vector(x, y) - HALFSIZE) / states.game.zoom + states.game.camCenter
end
