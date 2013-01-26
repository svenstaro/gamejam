require("objects/particleeffect")

Steam = class("Steam", ParticleEffect)

function Steam:setEffect()
	self.particleSystem = love.graphics.newParticleSystem(self.image, 100)

	self.particleSystem:setEmissionRate(15)
	self.particleSystem:setLifetime(0.9)
	self.particleSystem:setParticleLife(7)
	self.particleSystem:setDirection(self.angle)
	self.particleSystem:setSpread(0.5)
	self.particleSystem:setSpeed(10, 15)
	self.particleSystem:setSizes(2, 0.5)
	self.particleSystem:setSizeVariation(0.3)
	self.particleSystem:setColors(190,190,190,255, 
								  200,200,200,255,
								  222,222,222,100,
								  255,255,255,0)

	self.particleSystem:stop()
end
