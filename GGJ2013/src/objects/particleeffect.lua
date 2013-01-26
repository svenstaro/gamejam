require("core/object")

ParticleEffect = class("ParticleEffect", Object)

function ParticleEffect:__init(angle)
	self.x = 0
	self.y = 0
	self.z = 6

	self.image = resources.images.fill
	self.angle = angle or math.pi * 0.5

	self.particleSystem = nil
	self:setEffect()
end

function ParticleEffect:setEffect()
	-- TODO: override in your derivation and set self.particleSystem
end

function ParticleEffect:update(dt)
	print("update pe")
	self.particleSystem:setPosition(self.x, self.y)
	self.particleSystem:start()
	self.particleSystem:update(dt)
end

function ParticleEffect:draw()
	love.graphics.draw(self.particleSystem, 0, 0)
end