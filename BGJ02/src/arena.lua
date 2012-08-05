require("util/vector")
require("polygonentity")

Arena =  class("Arena", PolygonEntity)

ARENA_MAX_LENGTH = 100

function Arena:__init()
    PolygonEntity.__init(self)

    self.reference_point = 0
    self.dragging = false
    self.unspawnedAsteroid = nil

    self.cachedMouse = Vector()
    self.nearest = Vector()

    self.size = Vector(600, 400)
    self:addPoint(Vector(-self.size.x/2, -self.size.y/2))
    self:addPoint(Vector(-self.size.x/2,  self.size.y/2))
    self:addPoint(Vector( self.size.x/2,  self.size.y/2))
    self:addPoint(Vector( self.size.x/2, -self.size.y/2))

    self.physicsObject = {}
end

function Arena:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "static")
    local dp = self:getDrawPoints()
    self.physicsObject.shape1 = love.physics.newEdgeShape(dp[1], dp[2], dp[3], dp[4])
    self.physicsObject.fixture1 = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape1, 1)
    self.physicsObject.fixture1:setUserData(self)
    self.physicsObject.fixture1:setCategory(2) -- don't collide with arena walls
    self.physicsObject.shape2 = love.physics.newEdgeShape(dp[3], dp[4], dp[5], dp[6])
    self.physicsObject.fixture2 = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape2, 1)
    self.physicsObject.fixture2:setCategory(2) -- don't collide with arena walls
    self.physicsObject.fixture2:setUserData(self)
    self.physicsObject.shape3 = love.physics.newEdgeShape(dp[5], dp[6], dp[7], dp[8])
    self.physicsObject.fixture3 = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape3, 1)
    self.physicsObject.fixture3:setCategory(2) -- don't collide with arena walls
    self.physicsObject.fixture3:setUserData(self)
    self.physicsObject.shape4 = love.physics.newEdgeShape(dp[7], dp[8], dp[1], dp[2])
    self.physicsObject.fixture4 = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape4, 1)
    self.physicsObject.fixture4:setCategory(2) -- don't collide with arena walls
    self.physicsObject.fixture4:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Arena:mouse()
    local mouse = getMouseVector()
    local diff = self:ref() - mouse
    if diff:len() > ARENA_MAX_LENGTH then
        diff = diff:normalized() * ARENA_MAX_LENGTH
    end
    return self:ref() - diff
end

function Arena:isValidMouse()
    local mouse = getMouseVector()
    local b = 20
    return math.abs(mouse.x) - b > self.size.x / 2 or math.abs(mouse.y) - b > self.size.y / 2
end

function Arena:ref()
    return self:getPointByDistance(self.reference_point)
end

function Arena:splitWidth()
    local l = (self:ref() - self:mouse()):len() / ARENA_MAX_LENGTH
    return (1.0 - l * 0.5) * 40
end

function Arena:right()
    return self:getPointByDistance(self.reference_point + self:splitWidth())
end

function Arena:left()
    return self:getPointByDistance(self.reference_point - self:splitWidth())
end

function Arena:draw()
    love.graphics.setColor(255,255,255)
    self.__super.draw(self)

    if self.dragging and self:isValidMouse() then
        local ref = self:ref()
        local right = self:right()
        local left = self:left()
        local mouse = self:mouse()

        love.graphics.setColor(255, 255, 255)
        love.graphics.polygon("line", right.x, right.y, mouse.x, mouse.y, left.x, left.y)

        self.unspawnedAsteroid:draw()
    end
end

function Arena:getPointByDistance(d)
    local u = self.size.x * 2 + self.size.y * 2 
    while d < 0 do d = d + u end
    while d > u do d = d - u end

    if d < self.size.x then
        return Vector(- self.size.x / 2 + d, -self.size.y / 2)
    elseif d < self.size.x + self.size.y then
        return Vector(self.size.x / 2, d - self.size.x - self.size.y / 2)
    elseif d < self.size.x * 2 + self.size.y then
        return Vector(- d + self.size.x * 1.5 + self.size.y, self.size.y / 2)
    else
        return Vector(-self.size.x / 2, - d + self.size.x * 2 + self.size.y * 1.5)
    end
end

function Arena:findReferencePoint()
    local mouse = getMouseVector()

    if mouse.x > self.size.x / 2 and mouse.y < -self.size.y / 2 then
        self.reference_point = self.size.x
    elseif mouse.x > self.size.x / 2 and mouse.y > self.size.y / 2 then
        self.reference_point = self.size.x + self.size.y
    elseif mouse.x < -self.size.x / 2 and mouse.y > self.size.y / 2 then
        self.reference_point = self.size.x * 2 + self.size.y
    elseif mouse.x < -self.size.x / 2 and mouse.y < -self.size.y / 2 then
        self.reference_point = 0
    else
        local d_left   = math.abs(mouse.x + self.size.x / 2)
        local d_right  = math.abs(mouse.x - self.size.x / 2)
        local d_top    = math.abs(mouse.y + self.size.y / 2)
        local d_bottom = math.abs(mouse.y - self.size.y / 2)

        local min = math.min(d_left, d_right, d_top, d_bottom)

        if min > 20 then return false end

        local in_x = math.abs(mouse.x) <= self.size.x / 2
        local in_y = math.abs(mouse.y) <= self.size.y / 2
        if in_x and (min == d_top or mouse.y < -self.size.y / 2) then
            self.reference_point = self.size.x / 2 + mouse.x
        elseif in_y and (min == d_right or mouse.x > self.size.x / 2) then
            self.reference_point = self.size.x + self.size.y / 2 + mouse.y
        elseif in_x and (min == d_bottom or mouse.y > self.size.y / 2) then
            self.reference_point = self.size.x + self.size.y + self.size.x / 2 - mouse.x
        else
            self.reference_point = self.size.x * 2 + self.size.y * 1.5 - mouse.y
        end
    end

    return true
end

function Arena:update(dt)
    local valid = self:isValidMouse()

    if love.mouse.isDown("l") then
        if not self.dragging then
            if self:findReferencePoint() and game.materialAvailable >= materialValue(game.selectedAsteroid) then
                -- start dragging
                self.dragging = true
                self.unspawnedAsteroid = Asteroid(game.selectedAsteroid)
            end
        end
        
        if self.dragging then
            if game.selectedAsteroid ~= self.unspawnedAsteroid.size then
                self.unspawnedAsteroid = Asteroid(game.selectedAsteroid)
            end
            self.unspawnedAsteroid.position = self:mouse()
            self.unspawnedAsteroid:update(dt)
        end
    elseif self.dragging and valid then
        -- stop dragging
        self.dragging = false

        game.materialAvailable = game.materialAvailable - materialValue(self.unspawnedAsteroid.size)
        local ref = self:getPointByDistance(self.reference_point)
        local d = ref - self:mouse()
        local speed = (6 - self.unspawnedAsteroid.size) * 0.15
        speed = speed * game.power / 20
        self.unspawnedAsteroid.velocity = d * speed
        self.unspawnedAsteroid.lifetime = 0
        game.world:addNewAsteroid(self.unspawnedAsteroid)
        self.unspawnedAsteroid:startBeingTransparent()
        self.unspawnedAsteroid = nil
        resources.audio.sling:play()
    end
end

