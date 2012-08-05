require("entity")

Label = class("Label", Entity)

function Label:__init(text, position, color, font)
    Entity.__init(self)
    self.text = text
    self.position = position
    self.color = color or {255, 255, 255, 20}
    self.font = font
    self.highlight = 0 
    self.maxAlpha = 255
    self.scaleFactor = 0.1
    self.fadeTime = 0.2
end

function Label:update(dt)
    self.highlight = self.highlight * (1 - dt / self.fadeTime)

    Entity.update(self, dt)
    if self.lifetime > 2 then
        self:kill()
    end
end

function Label:setText(s)
    if self.text ~= s then
        self.text = s
        local o = self.color[4] or 255
        self.highlight = 1
    end
end

function Label:draw()
    local v = self.color[4] or 255
    local f = self.font or love.graphics.getFont()
    local s = 1 + self.highlight * self.scaleFactor
    local a = self.maxAlpha * self.highlight + v * (1 - self.highlight)
    love.graphics.setColor(self.color[1], self.color[2], self.color[3], a)
    love.graphics.setFont(f)
    love.graphics.print(self.text, self.position.x - s * f:getWidth(self.text) / 2, self.position.y - s * f:getHeight() / 2, 0, s, s)
end
