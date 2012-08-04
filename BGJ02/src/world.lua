require("util/helper")

World = class("World")

function World:beginContact(a, b, coll)
    print(a:getUserData().__name)
    print(b:getUserData().__name)
    if (a:getUserData().__super == "Ship" and b:getUserData().__name == "Asteroid") or
        (a:getUserData().__name == "Asteroid" and b:getUserData().__super == "Ship") then
        print("ship-asteroid collision")
    end
    if (a:getUserData().__name == "Bullet" and b:getUserData().__name == "Asteroid") or
        (a:getUserData().__name == "Asteroid" and b:getUserData().__name == "Bullet") then
        print("bullet-asteroid collision")
    end
end

function World:endContact(a, b, coll)
end

function World:preSolve(a, b, coll)
end

function World:postSolve(a, b, coll)
end

function World:__init()
    self.entities = {}

    love.physics.setMeter(64)
    self.physicsWorld = love.physics.newWorld(0, 0, false)
    self.physicsWorld:setCallbacks(function(a, b, coll) self:beginContact(a, b, coll) end,
                                   function(a, b, coll) self:endContact(a, b, coll) end,
                                   function(a, b, coll) self:preSolve(a, b, coll) end,
                                   function(a, b, coll) self:postSolve(a, b, coll) end)
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
    for k,v in pairs(self.physicsObjects) do
        v:destroy()
    end

    self.entities = {}
end

function World:remove(entity)
    for k,v in pairs(self.entities) do
        if v == entity then 
            self.entities[k] = nil 
            entity.world = nil
            if entity.physicsObject and entity.physicsObject.body then entity.physicsObject.body:destroy() end
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
