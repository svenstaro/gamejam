require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/building")
require("entities/wisp")
require("entities/lamp-chain")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    self.wisp = Wisp()
    self.wisp.position = Vector(0, -200)
    self.world:add(self.wisp)

    self.lamp = LampChain()
    self.lamp.position = Vector(0, -400)
    self.world:add(self.lamp)

    self.world:add(Building(0, Vector(300, 100)))

    self.generatedUntil = -SIZE.x

    self.keyHelpOpacity = 1
end

function Game:onUpdate(dt)
    local speed = dt * 400
    if love.keyboard.isDown("left")  then self.wisp:move(Vector(-1,  0) * speed) end
    if love.keyboard.isDown("right") then self.wisp:move(Vector( 1,  0) * speed) end
    if love.keyboard.isDown("up")    then self.wisp:move(Vector( 0, -1) * speed) end
    if love.keyboard.isDown("down")  then self.wisp:move(Vector( 0,  1) * speed) end

    self.keyHelpOpacity = math.max(0, self.keyHelpOpacity - dt / 5)

    -- generate full view and a bit (GENEREATE_AHEAD)
    while self.wisp.position.x + GENERATE_AHEAD > self.generatedUntil do
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

    -- help

    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.left, -SIZE.x/2, -SIZE.y*10, 0, SIZE.x/2/2, SIZE.y*20/2)

    love.graphics.setColor(255, 255, 255, 255 * self.keyHelpOpacity)
    local ax, ay, s = 0, 40, resources.images.key_arrow:getWidth() * 0.5 + 4
    love.graphics.draw(resources.images.key_arrow, ax,   ay,   math.pi * 0.5, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax+s, ay+s, math.pi * 1.0, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax,   ay+s, math.pi * 1.5, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax-s, ay+s, math.pi * 0.0, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)

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
