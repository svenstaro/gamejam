-- intro

require("core/gamestate")
require("core/resources")
require("core/objectgroup")
require("objects/player")
require("objects/door")
require("objects/file")
require("objects/trigger")

MainState = class("MainState", GameState)

function MainState:__init()
    self.objects = ObjectGroup()

    self.objects:add(Player())

    door = Door()
    self.objects:add(door)

    self.objects:add(File("This is patient number 12391. You died."))
end

function MainState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the game. Fuck yeah.", 10, 10)

    self.objects:draw()

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
