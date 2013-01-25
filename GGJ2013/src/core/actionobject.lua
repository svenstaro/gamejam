require("core/object")
require("objects/trigger")

ActionObject = class("ActionObject", Object)

activeActionObject = nil

function ActionObject:__init()
    self.trigger = CircleTrigger(self.x, self.y, 32)
    self.trigger.onEnter = function() self:enter() end
    self.trigger.onLeave = function() self:leave() end

    self.actionText = "Use Item"
end

function ActionObject:onAdd(group)
    group:add(self.trigger)
end

function ActionObject:onRemove(group)
    group:remove(self.trigger)
end

function ActionObject:enter()
    activeActionObject = self
end

function ActionObject:leave()
    activeActionObject = nil
end

function ActionObject:onAction()
    print("Using Action Object " .. self.__name .. " (" .. self.actionText .. ")")
end
