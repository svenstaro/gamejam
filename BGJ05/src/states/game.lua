require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/building")
require("entities/wisp")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    self.wisp = Wisp()
    self.wisp.position = Vector(100, -100)
    self.world:add(self.wisp)

    self.world:add(Building(0, Vector(300, 100)))
end

function Game:onUpdate(dt)
    local speed = dt * 100
    if love.keyboard.isDown("left")  then self.wisp:move(Vector(-1,  0) * speed) end
    if love.keyboard.isDown("right") then self.wisp:move(Vector( 1,  0) * speed) end
    if love.keyboard.isDown("up")    then self.wisp:move(Vector( 0, -1) * speed) end
    if love.keyboard.isDown("down")  then self.wisp:move(Vector( 0,  1) * speed) end

    self.world:update(dt)
end

function Game:onDraw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()

    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the main game state", 10, 10)

    local center = self.wisp.position
    love.graphics.push()
    love.graphics.translate((HALFSIZE-center):unpack())

    -- ground
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", center.x - HALFSIZE.x, 0, SIZE.x, SIZE.y)

    self.world:draw()
    love.graphics.pop()
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        stack:pop()
    end
end
