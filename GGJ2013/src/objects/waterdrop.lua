require("object/particleeffect")

WaterDrop = class("WaterDrop", ParticleEffect)

function WaterDrop:setEffect()
	self.particleSystem = love.graphics.newParticleSystem(self.image, 10)

	self.particleSystem:setEmissionRate(5)
	self.particleSystem:setLifetime(0.7)
	self.particleSystem:setParticleLife(1.5)
	self.particleSystem:setDirection(0)
	self.particleSystem:setSpread(8)
	self.particleSystem:setSpeed(5, 10)
	self.particleSystem:setSizes(2, 0.5)
	self.particleSystem:setSizeVariation(0.3)
	self.particleSystem:setColors(89,139,205,255,
		27,82,155,255,
		27,155,151,255,
		57,210,205,255)

	self.particleSystem:stop()
end