-- sprite has animation, can be switched, disabled, makes sounds

require("core/helper")
require("core/object")
require("src/external/AnAL")

Sprite = class("Sprite", Object)

function Sprite:__init(image)
    Object.__init(self)
    self.scale = 1
    self:setImage(image)
    self:setSounds(nil)
end

function Sprite:setImage(image)
    if not image then
        self:setAnimation(nil)
    else
        self:setAnimation(image, image:getWidth(), image:getHeight(), 0, 1)
    end
end

function Sprite:setAnimation(image, width, height, speed, count)
    if not image then
        self.visible = false
        self.animation = nil
    else
        self.visible = true
        self.animation = newAnimation(image, width, height, speed, count)
        self.width = width
        self.height = height
    end
end

function Sprite:setSounds(sounds, interval, random)
    if not sounds then
        self.audible = false
        self.sounds = {}
    else
        self.audible = true
        self.sounds = sounds
        self.soundInterval = interval or 10
        self.soundRandom = random or 10
        self:startSounds()
    end
end

function Sprite:startSounds()
    if not self.timeUntilSound then self.timeUntilSound = 0 end
    self.timeUntilSound = self.timeUntilSound + self.soundInterval + math.random(-1.0, 1.0) * self.soundRandom * self.soundInterval
end

function Sprite:update(dt)
    if self.audible and #self.sounds > 0 then
        self.timeUntilSound = self.timeUntilSound - dt
        if self.timeUntilSound <= 0 then
            -- play a sound
            local sound = self.sounds[math.random(1, #self.sounds)]
            local source = resources:makeSound(sound)
            source:setPosition(self.x, 0, self.y)
            source:play()
            self:startSounds()
        end
    end

    self.animation:update(dt)
end

function Sprite:draw()
    if self.visible then
        love.graphics.setColor(255,255,255)
        self.animation:draw(self.x, self.y, self.angle, self.scale, self.scale, self.width / 2, self.height / 2)
    end
end
