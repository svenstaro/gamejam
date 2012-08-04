require("util/helper")

World = class("World")

function World:__init()
    self.entities = {}

    love.physics.setMeter(64)
    self.physicsWorld = love.physics.newWorld(0, 0, false)
    self.physicsObjects = {}
end

function World:add(entity)
    table.insert(self.entities, entity)
    entity.world = self

    if entity.physicsObject ~= nil then
        entity:enablePhysics()
    end
end

function World:clear()
    for k,v in self.phyicsObjects do
        v:destroy()
    end

    self.entities = {}
end

function World:remove(entity)
    for k,v in pairs(self.entities) do
        if v == entity then 
            self.entities[k] = nil 
            entity.world = nil
            entity.physicsObject:destroy()
        end
    end
end

function World:update(dt)
    for k, v in pairs(self.entities) do
        v:update(dt)
    end

    self.physicsWorld:update(dt)
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
