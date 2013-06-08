require("util/helper")
require("util/vector")
require("entities/lamp")

LampStatic = class("LampStatic", Lamp)

function LampStatic:__init()
    Lamp.__init(self)
end

function LampStatic:onUpdate(dt)
    Lamp.onUpdate(self, dt)
end

function LampStatic:onDraw()
    Lamp.onDraw(self)

    local img = resources.images.lantern
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(img, self.position.x, self.position.y, self.rotation, self.scale * 0.1, self.scale * 0.1,
        img:getWidth() / 2, img:getHeight() / 2)
end
