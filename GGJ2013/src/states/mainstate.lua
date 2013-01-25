-- intro

require("core/gamestate")
require("core/resources")
require("core/objectgroup")
require("objects/player")
require("objects/door")
require("objects/file")
require("objects/trigger")
require("objects/enemy")

MainState = class("MainState", GameState)

function MainState:__init()
    self.objects = ObjectGroup()

    self.centerX = 0
    self.centerY = 0

    self.objects:add(Player())

    door = Door()
    self.objects:add(door)

    self.objects:add(Enemy())

    self.objects:add(File("This is patient number 12391. You died."))
end

function MainState:getOffset()
    return love.graphics.getWidth() / 2      - self.centerX,
        love.graphics.getHeight() / 2 - self.centerY
end

function MainState:screenToWorld(x, y)
    local ox, oy = self:getOffset()
    local scale = 1
    return (x - ox) / scale, (y - oy) / scale
end

function MainState:getMousePosition()
    return self:screenToWorld(love.mouse.getPosition())
end

function MainState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.tiny)
    love.graphics.print(love.timer.getFPS() .. " FPS", 10, 10)

    love.graphics.push()
    love.graphics.translate(self:getOffset())
    self.objects:draw()
    love.graphics.pop()

    love.graphics.setFont(resources.fonts.normal)
    if activeActionObject then
        local t = "[E] " .. activeActionObject.actionText
        love.graphics.print(t, love.graphics.getWidth() / 2 -  love.graphics.getFont():getWidth(t) / 2, love.graphics.getHeight() - 100)
    end
end

function MainState:update(dt)
    self.objects:update(dt)
end

function MainState:keypressed(k, u)
    if k == "escape" then
        stack:pop()
    elseif k == " " then
        door.open = not door.open
    elseif k == "f" then
        file.number = "21494"
        stack:push(file)
    elseif k == "e" then
        if activeActionObject then
            activeActionObject:onAction()
            activeActionObject = nil
        end
    end
end
