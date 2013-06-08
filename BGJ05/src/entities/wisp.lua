require("util/helper")
require("util/vector")

require("scene/entity")

Wisp = class("Wisp", Entity)

function Wisp:__init()
    Entity.__init(self)
end

function Wisp:onAdd()
    --self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    --self.physicsObject.shape = love.physics.newPolygonShape(0, -10, 7, 10, 0, 3, -3, 10)
    --self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    --self.physicsObject.fixture:setSensor(true)
    --self.physicsObject.fixture:setUserData(self)
    --table.insert(self.world.physicsObjects, self.physicsObject)
end

function Wisp:onUpdate(dt)
end

function Wisp:onDraw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.circle("fill", self.position.x, self.position.y, 20)
end
