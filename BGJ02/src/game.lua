-- game

require("arena")
require("util/gamestate")
require("util/resources")

require("ship")
require("asteroid")
require("world")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    ship = Ship()
    self.world:add(ship)

    arena = Arena()
    self.world:add(arena)
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
    self.world:draw(dt)
    love.graphics.pop()
end

function Game:update(dt)
    self.world:update(dt)
end

function Game:keypressed(k, u)
    if k == "escape" then
        stack:pop() -- game
        stack:pop() -- menu
    elseif k == " " then
        local a = Asteroid(math.random(1,3))
        a.position = ship.position
        a.velocity = Vector(math.random(-20, 20), math.random(-20, 20))
        self.world:add(a)
    elseif k == "lshift" then
        ship:shoot()
    end
end

