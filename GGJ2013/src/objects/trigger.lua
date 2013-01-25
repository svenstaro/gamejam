-- triggers some callback when the player enters an area

require("core/object")

Trigger = class("Trigger", Object)

function Trigger:__init(x, y, w, h)
    self.x = x
    self.y = y
    self.z = 1
    self.w = w
    self.h = h

    self.inside = false

    self.onEnter = function() end
    self.onLeave = function() end
    self.onChange = function() end
end

function Trigger:update(dt)
    local player = self.group:ofType("Player")[1]

    local inside = player.x >= self.x and player.y >= self.y and player.x <= self.x + self.w and player.y <= self.y + self.h

    if inside ~= self.inside then
        if inside then
            self.onEnter()
        else
            self.onLeave()
        end
        self.onChange()
    end

    self.inside = inside
end

function Trigger:draw()
    if debug then
        love.graphics.setColor(0, 255, 0)
        love.graphics.rectangle("line", self.x, self.y, self.w, self.h)
    end
end
