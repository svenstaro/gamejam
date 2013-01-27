-- the player object that moves around the screen

require("core/object")
require("objects/toast")
require("external/AnAL")

Player = class("Player", Object)

function Player:__init()
    self.x = 200
    self.y = 200
    self.z = 1
    self.angle = 0
    self.lifetime = 0
    self.walk_angle = 0
    self.head_angle = 0
    self.timeSinceLastStep = 0
    self.head = resources.images.player_head
    self.anim = newAnimation(resources.images.player_anim, 16, 16, 0.1, 4)

    self.physicsObject = {}

    self.shadowMapData = love.image.newImageData(512, 1)
    self.shadowMap = nil
end

function Player:bakeShadows(world)
    local width = self.shadowMapData:getWidth()
    for i = 0, width  - 1 do
        local alpha = i * math.pi * 2 / width
        local length = 1000
        local dx, dy = self.x + math.cos(alpha) * length, self.y - math.sin(alpha) * length

        local results = {}
        world:rayCast(self.x, self.y, dx, dy, function(fixture, x, y, xn, yn, fraction)
            local user = fixture:getUserData()
            if user.__name == "Player" then return 1 end
            if user.__name == "WallTile" and not user.castShadow then return 1 end

            --love.graphics.setColor(255, 0, 0)
            --love.graphics.circle("fill", x, y, 2)

            local d = math.sqrt((self.x - x) * (self.x - x) + (self.y - y) * (self.y - y))
            table.insert(results, d)
            return 1
        end)

        local smallest = 1000
        for k, v in pairs(results) do
            if smallest > v then smallest = v end
        end
        local d = smallest * 1000

        local r = d % 256
        local g = ((d - r) / 256) % 256
        local b = (d - g * 256) / 256 / 256

        self.shadowMapData:setPixel(i, 0, r, g, b, 255)
    end
    self.shadowMap = love.graphics.newImage(self.shadowMapData)
    --self.shadowMap:setFilter("nearest", "nearest")
end

function Player:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.group.physicsWorld, self.x, self.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(24)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.group.physicsObjects, self.physicsObject)
end

function Player:disablePhysics()
    self.physicsObject.body:destroy()
end

function Player:update(dt)
    -- retrieve the position the physics engine has calculated during update()
    self.x = self.physicsObject.body:getX()
    self.y = self.physicsObject.body:getY()

    local mx, my = main:getMousePosition()

    walking = false

    local dx, dy = 0, 0

    if love.keyboard.isDown("a") then
        dx = dx - 1
        walking = true
        goleft = false
    end
    if love.keyboard.isDown("d") then
        dx = dx + 1
        walking = true
        goleft = true
    end
    if love.keyboard.isDown("w") then
        dy = dy - 1
        walking = true
        goleft = true
    end
    if love.keyboard.isDown("s") then
        dy = dy + 1
        walking = true
        goleft = true
    end

    local tmphead_angle = math.atan2(my - self.y, mx - self.x)

    if math.abs(tmphead_angle - self.walk_angle) < 1 then
        self.head_angle = math.atan2(my - self.y, mx - self.x)
    end
    if not goleft and ((tmphead_angle > (math.pi -0.5) or (tmphead_angle < -2.1))) then
        self.head_angle = math.atan2(my - self.y, mx - self.x)
    end

    --change head directon on turn
    if olddx == -1 * dx then
        self.head_angle  = self.head_angle + 3.14
    end
    if olddy == -1 * dy then
        self.head_angle  = self.head_angle + 3.14
    end

    if dy ~= 0 then
        olddy = dy
    end
    if dx ~= 0 then
        olddx = dx
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
        --self:makeToast("Tap", {100, 100, 100, 50}, resources.fonts.tiny)
        if walking == "walk_high" then
            walking = "walk_low"
        else
            walking = "walk_high"
        end

        resources:makeSound(walking):play()

        self.timeSinceLastStep = 0
    end

    local speed = 200
    if love.keyboard.isDown("lshift") then speed = speed * 3 end
    self.x = self.x + dx * speed * dt
    self.y = self.y + dy * speed * dt

    local b = self.physicsObject.body
    b:setPosition(self.x, self.y)

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

    -- debug
    -- if self.shadowMap then love.graphics.draw(self.shadowMap, self.x, self.y - 20, 0, 1, 10) end
end

function Player:kill()
    print("Player died")
end
