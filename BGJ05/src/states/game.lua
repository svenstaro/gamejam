require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/building")
require("entities/wisp")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    self.wisp = Wisp()
    self.wisp.position = Vector(0, -200)
    self.world:add(self.wisp)

    self.world:add(Building(0, Vector(300, 100)))

    self.generatedUntil = -SIZE.x
end

function Game:onUpdate(dt)
    local speed = dt * 100
    if love.keyboard.isDown("left")  then self.wisp:move(Vector(-1,  0) * speed) end
    if love.keyboard.isDown("right") then self.wisp:move(Vector( 1,  0) * speed) end
    if love.keyboard.isDown("up")    then self.wisp:move(Vector( 0, -1) * speed) end
    if love.keyboard.isDown("down")  then self.wisp:move(Vector( 0,  1) * speed) end

    -- generate full view and a bit (GENEREATE_AHEAD)
    while self.wisp.position.x + GENERATE_AHEAD > self.generatedUntil do
        print("Generate")
        self:generateWorld()
    end

    self.world:update(dt)
end

function Game:generateWorld()
    local x = self.generatedUntil
    local w = randf(50, 300)
    local h = randf(50, MAX_HEIGHT)
    self.world:add(Building(x, Vector(w, h)))
    self.generatedUntil = x + w * randf(0.6, 1.2)
end

function Game:onDraw()
    love.graphics.setBackgroundColor(20, 10, 50)
    love.graphics.clear()

    local center = self.wisp.position
    love.graphics.push()
    love.graphics.translate((HALFSIZE-center):unpack())

    -- ground
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", center.x - HALFSIZE.x, 0, SIZE.x, SIZE.y)

    self.world:draw()
    love.graphics.pop()

    -- debug info
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.setColor(255, 255, 255)
    love.graphics.print("Generated until " .. self.generatedUntil, 10, 10)
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        stack:pop()
    end
end
