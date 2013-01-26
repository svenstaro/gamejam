-- the player object that moves around the screen

require("core/object")
require("objects/toast")
require("src/external/AnAL")

Player = class("Player", Object)

function Player:__init()
    self.x = 0
    self.y = 0
    self.z = 1
    self.angle = 0
    self.lifetime = 0
    self.walk_angle = 0
    self.head_angle = 0
    self.timeSinceLastStep = 0
    self.anim = newAnimation(resources.images.player_anim, 16, 16, 0.1, 4)
    self.head = resources.images.player_head
    self.anim = newAnimation(resources.images.player_anim, 16, 16, 0.1, 4)

    self.physicsObject = {}
end

function Player:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.group.physicsWorld, self.x, self.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(16)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.group.physicsObjects, self.physicsObject)
end

function Player:update(dt)
    local mx, my = main:getMousePosition()
    self.head_angle = math.atan2(my - self.y, mx - self.x)

    walking = false
    
    local dx, dy = 0, 0

    if love.keyboard.isDown("a") then 
        dx = dx - 1 
        walking = true 
    end
    if love.keyboard.isDown("d") then 
        dx = dx + 1 
        walking = true 
    end
    if love.keyboard.isDown("w") then 
        dy = dy - 1 
        walking = true 
    end
    if love.keyboard.isDown("s") then 
        dy = dy + 1 
        walking = true 
    end

    if math.abs(dy) + math.abs(dx) == 2 then
        dx = dx * 0.84
        dy = dy * 0.84
    end

    self.timeSinceLastStep = self.timeSinceLastStep + dt

    if math.abs(dx) + math.abs(dy) > 0 then
        self.walk_angle = math.atan2(dy, dx)
    end

    if (dx ~= 0 or dy ~= 0) and self.timeSinceLastStep >= 0.3 then
        --self:makeToast("Tap", {100, 100, 100, 100})
        -- TODO: add random foot step sound here!
        self.timeSinceLastStep = 0
    end

    local speed = 200
    self.x = self.x + dx * speed * dt
    self.y = self.y + dy * speed * dt

    self.physicsObject.body:setX(self.x)
    self.physicsObject.body:setY(self.y)
    self.physicsObject.body:setAngle(self.angle)

    main.centerX = self.x
    main.centerY = self.y
    if walking then
        self.anim:update(dt)
    end

    love.audio.setPosition(self.x, 0, self.y)
    love.audio.setOrientation(math.cos(self.walk_angle), math.sin(self.walk_angle), 0, 0, 0, -1, 0)

end

function Player:draw()
    love.graphics.setColor(255, 255, 255)
    self.anim:draw(self.x, self.y, self.walk_angle, 4, 4, 8, 8)
    love.graphics.draw(self.head, self.x, self.y, self.head_angle, 1, 1, 32, 32)
end
 
