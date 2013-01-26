-- (Elevator-) doors opening.

require("core/object")
require("core/vector")

Door = class("Door", Object)

function Door:__init(angle)
    self.open = false
    self.openness = 0

    self.locked = false
    self.proximityOpen = true

    self.x = 0
    self.y = 0
    self.z = 2
    self.angle = angle or math.pi * 0.5

    self.physicsObjectA = {}
    self.physicsObjectB = {}
end

function Door:enablePhysics()
    if self.angle == 0 then
        self.physicsObjectA.body = love.physics.newBody(self.group.physicsWorld, self.x - 32, self.y, "static")
        self.physicsObjectA.shape = love.physics.newRectangleShape(64, 20)
        self.physicsObjectB.body = love.physics.newBody(self.group.physicsWorld, self.x + 32, self.y, "static")
        self.physicsObjectB.shape = love.physics.newRectangleShape(64, 20)
    else
        self.physicsObjectA.body = love.physics.newBody(self.group.physicsWorld, self.x, self.y - 32, "static")
        self.physicsObjectA.shape = love.physics.newRectangleShape(20, 64)
        self.physicsObjectB.body = love.physics.newBody(self.group.physicsWorld, self.x, self.y + 32, "static")
        self.physicsObjectB.shape = love.physics.newRectangleShape(20, 63)
    end

    self.physicsObjectA.fixture = love.physics.newFixture(self.physicsObjectA.body, self.physicsObjectA.shape, 1)
    self.physicsObjectA.fixture:setUserData(self)
    self.physicsObjectB.fixture = love.physics.newFixture(self.physicsObjectB.body, self.physicsObjectB.shape, 1)
    self.physicsObjectB.fixture:setUserData(self)

    table.insert(self.group.physicsObjects, self.physicsObjectA)
    table.insert(self.group.physicsObjects, self.physicsObjectB)
end

function Door:disablePhysics()
    self.physicsObjectA.body:destroy()
    self.physicsObjectB.body:destroy()
end

function Door:update(dt)
    if self.locked then
        self.open = false
    else
        if self.proximityOpen then
            local d = Vector(main.player.x - self.x, main.player.y - self.y):len()
            local open = d < 100
            if open ~= self.open then
                self:toggle()
            end
        end
    end

    local target = 0
    if self.open then target = 1 end

    self.openness = self.openness + (target - self.openness ) * dt * 10

    local dist = 50 * self.openness
    if self.angle == 0 then
        self.physicsObjectA.body:setX(self.x - 32 - dist)
        self.physicsObjectB.body:setX(self.x + 32 + dist)
    else
        self.physicsObjectA.body:setY(self.y - 32 - dist)
        self.physicsObjectB.body:setY(self.y + 32 + dist)
    end
end

function Door:toggle()
    self.open = not self.open
    -- love.audio.play(resources.audio.door_slide)
    local snd = resources:makeSound("door_slide")
    snd:setPosition(self.x, 0, self.y)
    snd:play()
end

function Door:draw()
    love.graphics.push()

    love.graphics.translate(self.x, self.y)
    love.graphics.rotate(self.angle)


    love.graphics.setColor(255, 255, 255)
    local x, y = main:worldToScreen(self.x, self.y)
    love.graphics.setScissor(x - 64, y - 64, 128, 128)
    love.graphics.draw(resources.images.doorTop, -64 -self.openness * 50, 32, math.rad(-90), 1, 1)
    love.graphics.draw(resources.images.doorBot, self.openness * 50, 32, math.rad(90), -1, -1)
    -- love.graphics.rectangle("fill", 0,- 64 - self.openness * 50, -10, 64, 20)
    -- love.graphics.rectangle("fill", 0, 1, 1, self.openness * 50, -10, 64, 20)
                                --( drawable, x, y, r, sx, sy, ox, oy, kx, ky )

    love.graphics.setScissor()
    love.graphics.pop()
end
