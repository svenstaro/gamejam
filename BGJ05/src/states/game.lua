require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/wisp")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    self.wisp = Wisp()
    self.wisp.position = Vector(100, 100)
    self.world:add(self.wisp)
end

function Game:onUpdate(dt)
    self.world:update(dt)
end

function Game:onDraw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()

    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the main game state", 10, 10)

    self.world:draw()
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        stack:pop()
    end
end
