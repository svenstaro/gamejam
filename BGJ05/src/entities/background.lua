require("util/helper")
require("util/vector")

require("scene/entity")

Background = class("Background", Entity)

function Background:__init(x)
    Entity.__init(self)

    local l = math.random(0, 8)
    if l < 4 then self.layer = 1
    elseif l < 7 then self.layer = 2
    else self.layer = 3 end

    self.position = Vector(x, -100-100* self.layer)
    self.size = Vector(randf(20, 60), randf(60, 120))
    self.z = 100 * self.layer
    self.parallax = 1 - 0.1 * self.layer
end

function Background:onDraw()
    local a = 10 + 10 * self.layer
    love.graphics.setColor(unpack(BACKGROUND_COLORS[self.layer]))
    local screenCenter = - TRANSLATION + HALFSIZE
    local dx = self.position.x - screenCenter.x
    local x = screenCenter.x + dx * self.parallax
    love.graphics.rectangle("fill", x - self.size.x/2, self.position.y, self.size.x, -self.size.y)
end
