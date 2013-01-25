-- Files lying around

require("core/actionobject")

File = class("File", ActionObject)

function File:__init(text)
    self.text = text or "- empty file -"

    self.x = 100
    self.y = 100
    self.z = 0.5

    ActionObject.__init(self)
    self.actionText = "Read Patient File"
end

function File:read()
    love.graphics.print(self.text, 10, 10)
end

function File:update(dt)
end

function File:draw()
    love.graphics.push()

    love.graphics.translate(self.x, self.y)

    love.graphics.setColor(255, 200, 100)
    love.graphics.rectangle("fill", -20, -20, 40, 40)

    love.graphics.pop()
end

function File:onAction()
    file.text = self.text
    stack:push(file)
end
