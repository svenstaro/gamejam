require("util/gamestate")
require("util/resources")

require("scene/world")

require("entities/background")
require("entities/building")
require("entities/wisp")
require("entities/lamp-chain")
require("entities/lamp-static")
require("entities/lamp-antenna")
require("entities/lamp-airplane")

Game = class("Game", GameState)

function Game:__init()
    self.world = World()

    self.wisp = Wisp()
    self.wisp.position = Vector(-10, -100)
    self.world:add(self.wisp)

    local lampstatic = LampStatic()
    lampstatic.position = Vector(0, -200)
    self.world:add(lampstatic)

    local lampchain = LampChain()
    lampchain.position = Vector(200, -200)
    self.world:add(lampchain)

    local lampantenna = LampAntenna()
    lampantenna.position = Vector(-200, -200)
    self.world:add(lampantenna)

    local lampairplane = LampAirplane()
    lampairplane.position = Vector(0, -500)
    self.world:add(lampairplane)

    self.generatedUntil = -SIZE.x*5

    self.keyHelpOpacity = 1

    self.camCenter = Vector(0, -300)
end

function Game:getKeyboardVector()
    local v = Vector()
    if love.keyboard.isDown("left")  then v.x = v.x - 1 end
    if love.keyboard.isDown("right") then v.x = v.x + 1 end
    if love.keyboard.isDown("up")    then v.y = v.y - 1 end
    if love.keyboard.isDown("down")  then v.y = v.y + 1 end
    return v:normalized()
end

function Game:onUpdate(dt)
    self.wisp:move(self:getKeyboardVector())

    self.keyHelpOpacity = math.max(0, self.keyHelpOpacity - dt / 10)

    -- generate full view and a bit (GENEREATE_AHEAD)
    while self.wisp.position.x + GENERATE_AHEAD > self.generatedUntil do
        self:generateWorld()
    end

    self.world:update(dt)
end

function Game:generateWorld()
    local x = self.generatedUntil
    local w = math.random(4, 10) * 50
    local h = randf(500, MAX_HEIGHT)
    self.world:add(Building(x, Vector(w, h)))

    local antenna = LampAntenna()
    antenna.position = Vector(x + randf(0, w), -h)
    self.world:add(antenna)

    self.generatedUntil = x + w * randf(1.0, 4)

    local bx = x

    if x > 200 then
        while x < self.generatedUntil do
            local lamp = LampStatic()
            lamp.position = Vector(x, randf(-100, -500))
            self.world:add(lamp)
            x = x + randf(100, 300)
        end
    end

    while bx < self.generatedUntil do
        self.world:add(Background(bx))
        bx = bx + randf(20, 90)/3
    end
end

function Game:onDraw()
    LIGHT_CANVAS:clear(0, 0, 0)

    love.graphics.setBackgroundColor(20, 10, 50)
    love.graphics.clear()

    love.graphics.setBackgroundColor(255, 255, 255)
    love.graphics.draw(resources.images.sky, 0, 0, 0, (HALFSIZE):unpack())

    local wp = Vector(self.wisp.position:unpack())
    local s = SIZE/3/2

    if wp.x < self.camCenter.x - s.x then self.camCenter.x = wp.x + s.x
    elseif wp.x > self.camCenter.x + s.x then self.camCenter.x = wp.x - s.x
    end

    if wp.y < self.camCenter.y - s.y then self.camCenter.y = wp.y + s.y
    elseif wp.y > self.camCenter.y + s.y then self.camCenter.y = wp.y - s.y
    end

    self.camCenter.y = math.min(self.camCenter.y, 0)

    TRANSLATION = -(self.camCenter - HALFSIZE)
    love.graphics.push()
    love.graphics.translate(TRANSLATION:unpack())

    -- ground
    for i=3,1,-1 do
        local a = 10 + 10 * i
        love.graphics.setColor(a, a, a)
        love.graphics.rectangle("fill", self.camCenter.x - HALFSIZE.x, -50-50*i, SIZE.x, 100)
    end
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", self.camCenter.x - HALFSIZE.x, 0, SIZE.x, SIZE.y)

    self.world:draw()

    love.graphics.pop()
    TRANSLATION = Vector()

    -- lights
    drawLights()

    -- help
    TRANSLATION = -(self.camCenter - HALFSIZE)
    love.graphics.push()
    love.graphics.translate(TRANSLATION:unpack())

    --love.graphics.setColor(255, 255, 255)
    --love.graphics.draw(resources.images.left, -SIZE.x, center.y-HALFSIZE.y, 0, SIZE.x, SIZE.y)

    love.graphics.setColor(255, 255, 255, 255 * self.keyHelpOpacity)
    local ax, ay, s = 0, -400, resources.images.key_arrow:getWidth() * 0.5 + 4
    love.graphics.draw(resources.images.key_arrow, ax,   ay,   math.pi * 0.5, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax+s, ay+s, math.pi * 1.0, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax,   ay+s, math.pi * 1.5, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)
    love.graphics.draw(resources.images.key_arrow, ax-s, ay+s, math.pi * 0.0, 0.5, 0.5, resources.images.key_arrow:getWidth()/2, resources.images.key_arrow:getHeight()/2)


    love.graphics.pop()
    TRANSLATION = Vector()

    -- debug info
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.setColor(255, 255, 255)
    love.graphics.print("Generated until " .. self.generatedUntil, 10, 10)
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        stack:pop()
    elseif k == " " then
        self.wisp:jump(self:getKeyboardVector())
    end
end
