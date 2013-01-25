-- Files lying around

require("core/object")

File = class("File", Object)

function File:__init(text)
    self.text = text or "test"

    self.x = 200
    self.y = 300
    self.z = 1
end

function File:read()
    love.graphics.print(self.text, 10, 10)
end

function File:update(dt)

end


function File:draw()
    love.graphics.push()

    love.graphics.translate(self.x, self.y)

    love.graphics.setColor(255, 235, 205)
    love.graphics.rectangle("fill", 0, 0, 50, 50)

    love.graphics.pop()
end
