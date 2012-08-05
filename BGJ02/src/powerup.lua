require("util/helper")
require("util/vector")
require("entity")
require("toast")

Powerup = class("Powerup", Entity)

-- rewards:
--   power: more speed when shooting asteroids
--   material: increase material/max material
--   multiplier: add +1 multiplier to all scores

function Powerup:__init(position, reward)
    Entity.__init(self)
    self.position = position

    self.reward = reward
    self.dieAt = math.random() * 4 + 13 -- 13 to 17 seconds

    self.physicsObject = {}
end

function Powerup:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(5)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Powerup:update(dt)
    PolygonEntity.update(self, dt)

    local left = self.dieAt - self.lifetime

    if left <= 0 then
        self:kill()
    end
end

function Powerup:draw()
    local left = self.dieAt - self.lifetime

    if left > 2 or math.sin(self.lifetime * 30) > -0.4 then
        local particle_image_size = Vector(resources.images.particle:getWidth(), resources.images.particle:getHeight())
        local particle_size = 0.5
        if self.reward == "multiplier" then
            love.graphics.setColor(255, 128, 0, 100)
            love.graphics.draw(resources.images.particle,
                               self.position.x -((particle_image_size.x * particle_size) / 2),
                               self.position.y -((particle_image_size.y * particle_size) / 2),
                               0,
                               particle_size)
            love.graphics.setColor(255, 128, 0)
        elseif self.reward == "power" then
            love.graphics.setColor(50, 80, 255, 100)
            love.graphics.draw(resources.images.particle,
                               self.position.x -((particle_image_size.x * particle_size) / 2),
                               self.position.y -((particle_image_size.y * particle_size) / 2),
                               0,
                               particle_size)
            love.graphics.setColor(50, 80, 255)
        elseif self.reward == "material" then
            love.graphics.setColor(20, 240, 20, 100)
            love.graphics.draw(resources.images.particle,
                               self.position.x -((particle_image_size.x * particle_size) / 2),
                               self.position.y -((particle_image_size.y * particle_size) / 2),
                               0,
                               particle_size)
            love.graphics.setColor(20, 240, 20)
        end

        local n = 3
        if self.reward == "power" then
            n = 4
        elseif self.reward == "material" then
            n = 5
        end

        love.graphics.circle("line", self.position.x, self.position.y, 2 + 3 * math.abs(math.sin(self.lifetime * 5)), n)
    end
end

function Powerup:perform()
    if self.reward == "multiplier" then
        game.multiplier = game.multiplier + 1
        game.multiplierTimer = MULTIPLIER_DELAY

        self:makeToast("+1 multiplier", {255, 128, 0})
    elseif self.reward == "power" then
        game.power = game.power + 1

        self:makeToast("+1 power", {50, 80, 255})
    elseif self.reward == "material" then
        local add = math.random(1, 4)
        game.materialAvailable = game.materialAvailable + add
        MAX_MATERIAL = MAX_MATERIAL + add

        self:makeToast("+" .. add .. " material", {20, 240, 20})
    end
    self:kill()
end
