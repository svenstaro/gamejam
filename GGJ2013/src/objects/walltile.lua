-- an object representing a piece of wall, not being drawn, but required for physics

require("core/object")

WallTile = class("WallTile", Object)

function WallTile:__init(x, y)
    self.x = x
    self.y = y
    self.z = 100
    self.blocking = true -- to toggle whether the wall is there or not
end

function WallTile:update(dt)
    -- dat physx magick!?
end

function WallTile:draw()
    if debug then
        love.graphics.setColor(255, 255, 0)
        love.graphics.rectangle("line", self.x, self.y, 64, 64)
    end
end
