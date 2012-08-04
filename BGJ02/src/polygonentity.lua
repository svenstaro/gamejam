require("entity")
require("util/vector")

PolygonEntity = class("PolygonEntity", Entity)

function PolygonEntity:__init()
    Entity.__init(self)
    self:clear()
end

function PolygonEntity:clear()
    self.points = {}
end

function PolygonEntity:addPoint(point)
    table.insert(self.points, point)
end

function PolygonEntity:getDrawPoints()
    pts = {}
    for k, v in ipairs(self.points) do
        p = v:rotated(self.rotation) + self.position
        table.insert(pts, p.x)
        table.insert(pts, p.y)
    end
    return pts
end

function PolygonEntity:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.polygon("line", self:getDrawPoints())
end
