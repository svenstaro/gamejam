require("util/vector")
require("polygonentity")

Arena =  class("Arena", PolygonEntity)

function Arena:__init()
    PolygonEntity.__init(self)
    self.nearest_point = Vector()
    self.being_changed = false -- if the mouse is down, this arena is being changed
    self.size = Vector(600, 400)
    self:addPoint(Vector(-self.size.x/2,    -self.size.y/2))
    self:addPoint(Vector(-self.size.x/2,    self.size.y/2))
    self:addPoint(Vector(self.size.x/2,     self.size.y/2))
    self:addPoint(Vector(self.size.x/2,     -self.size.y/2))
end

function Arena:draw()
    love.graphics.polygon("line", self:getDrawPoints())
    love.graphics.setColor(100, 255, 100)
    love.graphics.circle("fill", self:getDrawPoints()[self.nearest_point_index * 2 - 1],
                                 self:getDrawPoints()[self.nearest_point_index * 2], 20)

    if self.being_changed then
        love.graphics.setColor(255, 100, 100)
        love.graphics.circle("fill", self:getDrawPoints()[self.nearest_point_index * 2 - 1],
                                     self:getDrawPoints()[self.nearest_point_index * 2], 20)
    end
end

function Arena:update()
    -- get nearest point to mouse
    self.nearest_point = self.points[1] -- just a wild first guess
    self.nearest_point_index = 1 -- ditto
    for index,value in ipairs(self.points) do
        if value:dist(getMouseVector()) < self.nearest_point:dist(getMouseVector()) then
            self.nearest_point = value
            self.nearest_point_index = index
        end
    end

    if love.mouse.isDown("l") then
        self.being_changed = true
        self.points[self.nearest_point_index] = getMouseVector()
    else
        self.being_changed = false
    end
end

function Arena:keypressed(k, u)

end
