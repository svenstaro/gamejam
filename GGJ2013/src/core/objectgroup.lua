-- game state

require("core/helper")

ObjectGroup = class("ObjectGroup")

function ObjectGroup:__init()
	self.objects = {}
end

function ObjectGroup:add(object)
	table.insert(self.objects, object)
	object.group = self
	object:onAdd(self)
end

function ObjectGroup:remove(object)
	object:onRemove(self)
	if table.removeValue(self.objects, object) then
		object.group = nil
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
