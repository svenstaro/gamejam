-- game state

require("core/helper")

ObjectGroup = class("ObjectGroup")

function areUserData(uA, uB, typeA, typeB)
    return (uA.__name == typeA and uB.__name == typeB) or (uA.__name == typeB and uB.__name == typeA)
end

function ObjectGroup:beginContact(a, b, coll)
    local uA = a:getUserData()
    local uB = b:getUserData()

    if areUserData(uA, uB, "Player", "WallTile") then
        if uA.__name == "Player" then
            uA:makeToast("BAM", {200, 10, 10, 200})
        else
            uB:makeToast("BAM", {200, 10, 10, 200})
        end
    end
end

function ObjectGroup:endContact(a, b, coll)
end

function ObjectGroup:preSolve(a, b, coll)
end

function ObjectGroup:postSolve(a, b, coll)
end

function ObjectGroup:__init()
	self.objects = {}

	self.physicsWorld = nil
	self.physicsObjects = {}
end

function ObjectGroup:add(object)
	table.insert(self.objects, object)
	object.group = self
	object:onAdd(self)

	if object.physicsObject ~= nil then
		object:enablePhysics()
	end
end

function ObjectGroup:remove(object)
	object:onRemove(self)
	if table.removeValue(self.objects, object) then
		object.group = nil
		if object.physicsObject and object.physicsBody.body then object.physicsBody.body:destroy() end
	end
end

function ObjectGroup:draw()
	-- sort them by Z, or by X if equal
	table.sort(self.objects, function(a, b) return (a.z == b.z) and a.y < b.y or a.z < b.z end)

	for i, o in pairs(self.objects) do
		o:draw()
	end
end

function ObjectGroup:update(dt)
    if self.physicsWorld then
        self.physicsWorld:update(dt)
    end

	for i, o in pairs(self.objects) do
		o:update(dt)
	end
end

function ObjectGroup:ofType(type)
	local tmp = {}
	for i, o in pairs(self.objects) do
		if o.__name == type then
			table.insert(tmp, o)
		end
	end
	return tmp
end

function ObjectGroup:byName(name)
	for i, o in pairs(self.objects) do
		if o.name == name then
			return o
		end
	end
	return nil
end
