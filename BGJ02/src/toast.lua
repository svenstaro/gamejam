require("entity")

Toast = class("Toast", Entity)

function Toast:__init(text, position, color, font)
    Entity.__init(self)
    self.text = text
    self.position = position
    self.color = color or {255, 255, 255, 255}
    self.font = font or resources.fonts.toast
end

function Toast:update(dt)
    Entity.update(self, dt)
    if self.lifetime > 2 then
        self:kill()
    end
end

function Toast:draw()
    local a = 1
    if self.lifetime > 1.5 then
        a = 1 - (self.lifetime - 1.5) * 2
    end
    local v = self.color[4] or 255
    love.graphics.setColor(self.color[1], self.color[2], self.color[3], v * a)

    love.graphics.setFont(self.font)

    love.graphics.print(self.text, self.position.x - self.font:getWidth(self.text) / 2, self.position.y - 50 * self.lifetime)
end




function Entity:makeToast(text, color, font)
    self.world:add(Toast(text, self.position, color, font))
end
