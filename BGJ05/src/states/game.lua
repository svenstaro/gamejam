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
require("entities/lamp-star")

Game = class("Game", GameState)

function Game:__init()
    self:reset()

    music = love.audio.newSource("data/Cupids Revenge.ogg")
    love.audio.play(music)
end

function Game:reset()
    self.world = World()
    self.wisp = Wisp()
    self.wisp.position = Vector(-10, -100)
    self.world:add(self.wisp)

    local lampstatic = LampStatic()
    lampstatic.position = Vector(0, -200)
    self.world:add(lampstatic)

    -- local lampchain = LampChain()
    -- lampchain.position = Vector(200, -200)
    -- self.world:add(lampchain)

    -- local lampantenna = LampAntenna()
    -- lampantenna.position = Vector(-200, -200)
    -- self.world:add(lampantenna)

    -- local lampairplane = LampAirplane()
    -- lampairplane.position = Vector(0, -500)
    -- self.world:add(lampairplane)

    -- local lampstar = LampStar()
    -- lampstar.position = Vector(0, -600)
    -- self.world:add(lampstar)

    self.generatedUntil = -SIZE.x*5

    self.keyHelpOpacity = 1
    self.zoom = 1
    self.gameOver = false
    self.gameOverTimer = 0

    self.camCenter = Vector(0, -300)
    self.maxCamX = self.camCenter.x
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

    for k,v in pairs(self.world.entities) do
        if v.position.x + (v.size and v.size.x or 0) < self.camCenter.x - SIZE.x/MIN_ZOOM * 2 then
            v:kill()
        end
    end

    self.world:update(dt)

    if self.wisp.position.y > SIZE.y / 2 / self.zoom then
        self.gameOver = true
    end

    if self.wisp.position.x < self.camCenter.x - SIZE.x/2/self.zoom then
        self.gameOver = true
    end

    if self.gameOver then
        for k,v in pairs(self.world:findByType("Lamp", true)) do
            v:burnout()
        end

        self.gameOverTimer = self.gameOverTimer + dt
    end
end

function Game:generateWorld()
    local x = self.generatedUntil
    local w = math.random(4, 10) * 50
    local h = randf(500, MAX_HEIGHT)
    self.world:add(Building(x, Vector(w, h)))

    local antenna = LampAntenna()
    antenna.position = Vector(x + randf(0, w), -h)
    self.world:add(antenna)

    local airplane = LampAirplane()
    airplane.position = Vector(x + w, -randf(1000, 5000))
    self.world:add(airplane)

    local chainlamp = LampChain()
    chainlamp.position = Vector(x + w, -randf(500, h))
    self.world:add(chainlamp)

    local starlamp = LampStar()
    starlamp.position = Vector(x + w, -randf(4000, 8000))
    self.world:add(starlamp)

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
    self.maxCamX = math.max(self.camCenter.x, self.maxCamX)
    self.camCenter.x = math.max(self.maxCamX - SIZE.x, self.camCenter.x)

    local zoomSpeed = 400 / (Vector(self.wisp.physicsObject.body:getLinearVelocity()):len() or 1)
    local zoomHeight = 1 + self.wisp.position.y * 0.0006
    local zoom = math.min(zoomSpeed, zoomHeight) * 0.01 + self.zoom * 0.99
    self.zoom = math.min(MAX_ZOOM, math.max(MIN_ZOOM, zoom))

    TRANSLATION = -(self.camCenter - HALFSIZE)
    love.graphics.push()
    love.graphics.translate(HALFSIZE:unpack())
    love.graphics.scale(self.zoom)
    love.graphics.translate((-self.camCenter):unpack())

    -- ground
    for i=3,1,-1 do
        local a = 10 + 10 * i
        love.graphics.setColor(a, a, a)
        love.graphics.rectangle("fill", self.camCenter.x - HALFSIZE.x/self.zoom, -50-50*i, SIZE.x/self.zoom, 100)
    end
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", self.camCenter.x - HALFSIZE.x/self.zoom, 0, SIZE.x/self.zoom, SIZE.y)

    self.world:draw()

    love.graphics.pop()
    TRANSLATION = Vector()

    -- lights
    drawLights()

    -- help
    TRANSLATION = -(self.camCenter - HALFSIZE)
    love.graphics.push()
    love.graphics.translate(TRANSLATION:unpack())
    love.graphics.scale(self.zoom)

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

    local p = 1 - math.pow(1-math.min(1, self.gameOverTimer - 1), 3)
    love.graphics.setColor(255, 255, 255, 100)
    love.graphics.draw(resources.images.key_space, SIZE.x / 2, SIZE.y - p * 100,
        0.0, 0.5, 0.5, resources.images.key_space:getWidth() / 2, 0)

    -- debug info
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.setColor(255, 255, 255)
    love.graphics.print(self.gameOver and "Game over" or "Good luck", 10, 10)
end

function Game:onKeyPressed(k, u)
    if k == "escape" then
        stack:pop()
    elseif k == " " then
        if self.gameOver then
            self:reset()
        else
            self.wisp:jump(self:getKeyboardVector())
        end
    end
end
