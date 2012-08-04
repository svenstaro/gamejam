-- game

require("arena")
require("util/gamestate")
require("util/resources")

require("ship")

Game = class("Game", GameState)

function Game:__init()
    self.entities = {}

    ship = Ship()
    arena = Arena()
    table.insert(self.entities, ship)
    table.insert(self.entities, arena)
end

function Game:draw()
    love.graphics.setBackgroundColor(0, 0, 0)
    love.graphics.clear()

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    local s = "Game state"
    love.graphics.print(s, love.graphics.getWidth() - love.graphics.getFont():getWidth(s) - 10, 10)

    love.graphics.push()
    love.graphics.translate(love.graphics.getWidth() / 2, love.graphics.getHeight() / 2)
    for k, v in pairs(self.entities) do
        v:draw() 
    end
    love.graphics.pop()
end

function Game:update(dt)
    for k, v in pairs(self.entities) do
        v:update(dt)
    end
end

function Game:keypressed(k, u)
    if k == "escape" then
        stack:pop() -- game
        stack:pop() -- menu
    end
    if k == "lshift" then
        ship:shoot()
    end
end

