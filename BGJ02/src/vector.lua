require("util/helper")

Vector = class("Vector")

function Vector:__init()
    self.x = 0
    self.y = 0
end

function Vector:__init(x, y)
    self.x = x
    self.y = y
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
	return Vector(a.x + b.x, a.y + b.y)
end

function Vector.__sub(a, b)
	return Vector(a.x - b.x, a.y - b.y)
end

function Vector.__mul(a, b)
	if type(a) == "number" then
		return Vector(a*b.x, a*b.y)
	elseif type(b) == "number" then
		return Vector(b*a.x, b*a.y)
	else
		return a.x*b.x + a.y*b.y
	end
end

function Vector.__div(a, b)
	return Vector(a.x / b, a.y / b)
end

function Vector.__eq(a, b)
	return a.x == b.x and a.y == b.y
end

function Vector.__lt(a,b)
	return a.x < b.x or (a.x == b.x and a.y < b.y)
end

function Vector.__le(a,b)
	return a.x <= b.x and a.y <= b.y
end

function Vector.permul(a,b)
	return Vector(a.x*b.x, a.y*b.y)
end

function Vector:len2()
	return self.x * self.x + self.y * self.y
end

function Vector:len()
	return sqrt(self.x * self.x + self.y * self.y)
end

function Vector.dist(a, b)
	local dx = a.x - b.x
	local dy = a.y - b.y
	return sqrt(dx * dx + dy * dy)
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
	local c, s = cos(phi), sin(phi)
	self.x, self.y = c * self.x - s * self.y, s * self.x + c * self.y
	return self
end

function Vector:rotated(phi)
	local c, s = cos(phi), sin(phi)
	return Vector(c * self.x - s * self.y, s * self.x + c * self.y)
end

function Vector:perpendicular()
	return Vector(-self.y, self.x)
end

function Vector:projectOn(v)
	-- (self * v) * v / v:len2()
	local s = (self.x * v.x + self.y * v.y) / (v.x * v.x + v.y * v.y)
	return Vector(s * v.x, s * v.y)
end

function Vector:mirrorOn(v)
	local s = 2 * (self.x * v.x + self.y * v.y) / (v.x * v.x + v.y * v.y)
	return Vector(s * v.x - self.x, s * v.y - self.y)
end

function Vector:cross(v)
	return self.x * v.y - self.y * v.x
end
