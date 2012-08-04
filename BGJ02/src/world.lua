require("util/helper")

World = class("World")

function World:__init()
    self.entities = {}
end

function World:add(entity)
    table.insert(self.entities, entity)
    entity.world = self
end

function World:clear()
    self.entities = {}
end

function World:remove(entity)
    for k,v in pairs(self.entities) do
        if v == entity then 
            self.entities[k] = nil 
            entity.world = nil
        end
    end
end

function World:update(dt)
    for k, v in pairs(self.entities) do
        v:update(dt)
    end
end

function World:draw()
    for k, v in pairs(self.entities) do
        v:draw()
    end
end

function World:findByType(typename) 
    l = {}
    for k, v in pairs(self.entities) do
        if v.__name == typename then table.insert(l, v) end
    end
    return l
end
