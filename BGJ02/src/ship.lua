require("util/helper")
require("util/vector")
require("polygonentity")

Ship = class("Ship", PolygonEntity)

function Ship:__init()
    PolygonEntity.__init(self)
    self.velocity = Vector(20, 10)

    self:addPoint(Vector(0, -10))
    self:addPoint(Vector(10, 10))
    self:addPoint(Vector(0, 0))
    self:addPoint(Vector(-10, 10))
end

function Ship:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.polygon("line", self:getDrawPoints())
end
