
-- a self-managing toast popup text

require("core/object")

Toast = class("Toast", Object)

function Toast:__init(text, x, y, angle, color, font)
    self.x = x + math.random(-20, 20)
    self.y = y + math.random(-20, 20)
    self.z = 1
    self.angle = angle
    self.lifetime = 0
    self.text = text
    self.color = color or {255, 255, 255, 255}
    self.font = font or resources.fonts.normal
end

function Toast:update(dt)
    Object.update(self, dt)
    if self.lifetime > 1 then
        self:suicide()
    end
end

function Toast:draw()
    local a = 1
    if self.lifetime > 0.5 then
        a = 1 - (self.lifetime - 0.5) * 2
    end

    a = a * (1 - math.abs(math.sin(self.lifetime * 10)) * 0.4)

    local v = self.color[4] or 255
    love.graphics.setColor(self.color[1], self.color[2], self.color[3], v * a)

    love.graphics.setFont(self.font)

    love.graphics.print(self.text, self.x - (self.font:getWidth(self.text) / 2),
                                   self.y - (self.font:getHeight(self.text) / 2), self.angle,
                                   2 * math.pow(self.lifetime / 0.5, 5) + 1,
                                   2 * math.pow(self.lifetime / 0.5, 5) + 1)   
end

function Object:makeToast(text, color, font)
    self.group:add(Toast(text, self.x, self.y, 0.5 - math.random(), color, font))
end
