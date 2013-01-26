-- Files lying around

require("core/actionobject")

File = class("File", ActionObject)

function File:__init(x, y, text)
    self.text = text or "- empty file -"

    self.x = x
    self.y = y
    self.z = 0.5
    self.counter = 0

    ActionObject.__init(self)
    self.actionText = "Read Patient File"
end

function File:read()
    love.graphics.print(self.text, 10, 10)
end

function File:update(dt)
    self.counter = self.counter + dt
end

function File:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.file, self.x, self.y, math.sin(self.counter) * 0.3, 3, 3, resources.images.file:getWidth() / 2,resources.images.file:getHeight() / 2)
end

function File:onAction()
    file.text = self.text
    stack:push(file)
end
