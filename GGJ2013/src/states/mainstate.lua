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

    self.objects:add(File())

    trigger = Trigger(100, 100, 200, 200)
    trigger.onEnter = function() print "enter" end
    trigger.onLeave = function() print "leave" end
    self.objects:add(trigger)

end

function MainState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the game. Fuck yeah.", 10, 10)

    self.objects:draw()
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
    end
end