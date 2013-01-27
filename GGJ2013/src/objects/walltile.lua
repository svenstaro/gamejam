-- an object representing a piece of wall, not being drawn, but required for physics

require("core/object")

WallTile = class("WallTile", Object)

function WallTile:__init(x, y, castShadow)
    self.x = x
    self.y = y
    self.z = 100
    self.blocking = true -- to toggle whether the wall is there or not

    self.castShadow = castShadow

    self.physicsObject = {}
end

function WallTile:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.group.physicsWorld, self.x + 32, self.y + 32, "static")
    self.physicsObject.shape = love.physics.newRectangleShape(64, 64)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.group.physicsObjects, self.physicsObject)
end

function WallTile:disablePhysics()
    self.physicsObject.body:destroy()
end

function WallTile:update(dt)
end

function WallTile:draw()
    if debug then
        love.graphics.setColor(255, 255, 0)
        love.graphics.rectangle("line", self.x, self.y, 64, 64)
    end
end
