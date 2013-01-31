-- intro

require("core/gamestate")
require("core/resources")
require("core/objectgroup")
require("objects/player")
require("objects/door")
require("objects/file")
require("objects/trigger")
require("objects/level")
require("objects/enemy")
require("objects/sprite")

MainState = class("MainState", GameState)

function MainState:__init()
    self.checkpoint = {}
end

function MainState:onPush()
    self.lifetime = 0
    self.centerX = 0
    self.centerY = 0
    self.currentLevel = 0

    self.objects = {}
    self.levels = {}

    for i = 0, 4 do
        self:loadLevel(i)
    end

    self.player = Player()
    self.nextLevelSpawn = "spawn_01"
    self:setLevel(0)

    self.levelFade = 0.5
    self.nextLevel = 0

    self.canvas = nil
end

function MainState:parseLevel(i)
    if type(i) == "number" then return i end
    -- relative level switching ("+1")
    local fc = string.sub(i, 0, 1)
    if fc == "+" or fc == "-" then
        i = self.currentLevel + tonumber(i)
    end
    return i
end

function MainState:fadeToLevel(i, spawnName)
    self.levelFade = 1.0
    self.nextLevel = i
    self.nextLevelSpawn = spawnName
end

function MainState:setLevel(i)
    -- make sure to transfer the player into the new level
    self:world():remove(self.player)
    self.currentLevel = self:parseLevel(i)
    if self.nextLevelSpawn then
        local spawn = self:world():byName(self.nextLevelSpawn)
        if not spawn then
            print("WARNING! Level Spawn " .. self.nextLevelSpawn .. " in Level " .. self.currentLevel .. " not found.")
        else
            self.player.x = spawn.x + spawn.w / 2
            self.player.y = spawn.y + spawn.h / 2

            self.checkpoint = {self.currentLevel, self.nextLevelSpawn}
        end
        self.nextLevelSpawn = nil
    end
    self:world():add(self.player)
    self:playLevelMusic(i)
end

function MainState:respawn()
    self.nextLevelSpawn = self.checkpoint[2]
    self:setLevel(self.checkpoint[1])
    self.levelFade = 0.5
end

function MainState:playLevelMusic(i)
    local playlist = {
        { "dark_factory", 0.1 },
        { "SoftElectro1", 0.1 },
        { "cave_theme", 0.1 },
        { "AmbientSound1", 0.1 },
        { "AmbientSound1", 0.1 },
        { "SoftElectro2", 0.1 },
        { "AmbientSound2", 0.1 },
        { "AmbientSound2", 0.1 },
        { "cave_theme", 0.1 }
    }

    local l = self.levels[self.currentLevel]

    local music = resources.music[playlist[i + 1][1]]
    music:setVolume(playlist[i + 1][2])
    music:setPosition(l.width / 2, 0, l.height / 2)
    music:play()
end

function MainState:loadLevel(i)
    self.objects[i] = ObjectGroup()

    self.objects[i].physicsWorld = love.physics.newWorld(0, 0, false)
    self.objects[i].physicsWorld:setCallbacks(function(a, b, coll) self.objects[i]:beginContact(a, b, coll) end,
                                   function(a, b, coll) self.objects[i]:endContact(a, b, coll) end,
                                   function(a, b, coll) self.objects[i]:preSolve(a, b, coll) end,
                                   function(a, b, coll) self.objects[i]:postSolve(a, b, coll) end)

    local level = Level("level" .. i, self.objects[i])
    self.levels[i] = level
    self.objects[i]:add(level)

    -- HERE STARTS MAGIC
    local o = self.objects[i]
    if i == 0 then
        o:byName("door_08").locked = true
        o:byName("trigger_01").visible = true
        o:byName("trigger_02").visible = true
        o:byName("trigger_01").angle = math.pi * 0
        o:byName("trigger_02").angle = math.pi * 0.5
        o:byName("trigger_01").onEnter = function()
            o:byName("trigger_02").enabled = true
            o:byName("door_08").locked = false
            -- TODO: Hammer graphics
        end

        o:byName("trigger_02").onEnter = function()
            o:byName("door_09").locked = false
            -- TODO: Button graphics
        end
    elseif i == 1 then
        o:byName("button_01").onEnter = function()
            o:byName("door_06").locked = false
            o:byName("door_07").locked = false
            o:byName("door_08").locked = false
            o:byName("monsterspawn_01").visible = true
            o:byName("hazard_01").enabled = true
            -- TODO
            -- o:byName("hazard_01_sprite").visible = true
            o:byName("door_05").locked = true
            --o:byName("monstersound_01").audible = false
            o:byName("trigger_02").enabled = true

            -- TODO: Button graphics
        end

        o:byName("trigger_01").onEnter = function()
            o:byName("door_08").locked = true
        end

        o:byName("trigger_02").onEnter = function()
            o:byName("door_02").locked = true
            o:byName("monsterspawn_01"):run()
        end
    elseif i == 2 then
        o:byName("trigger_01").onEnter = function()
            o:byName("door_00").locked = false
        end

        o:byName("button_01").onEnter = function()
            o:byName("door_05").locked = false
        end
    elseif i == 3 then
        o:byName("button_01").onEnter = function()
            o:byName("door_02").locked = false
            o:byName("door_03").locked = false
            self.objects[2]:byName("door_03").locked = false
            self.objects[4]:byName("door_01").locked = false
        end

        o:byName("button_02").onEnter = function()
            self.objects[2]:byName("door_03").locked = false
        end
    elseif i == 4 then
        o:byName("button_01").onEnter = function()
            for i = 1, 7 do
                o:byName("hazard_0" .. i).enabled = true
                o:byName("hazard_0" .. i .. "_sprite").visible = true
            end
            self.objects[2]:byName("door_06").locked = false
        end
    elseif i == 5 then
    elseif i == 7 then

    end


    -- HERE ENDS MAGIC
end

function MainState:world()
    return self.objects[self.currentLevel]
end

function MainState:getOffset()
    return math.round(love.graphics.getWidth() / 2 - self.centerX),
        math.round(love.graphics.getHeight() / 2 - self.centerY)
end

function MainState:screenToWorld(x, y)
    local ox, oy = self:getOffset()
    local scale = 1
    return (x - ox) / scale, (y - oy) / scale
end

function MainState:worldToScreen(x, y)
    local ox, oy = self:getOffset()
    return x + ox, y + oy
end

function MainState:getMousePosition()
    return self:screenToWorld(love.mouse.getPosition())
end

function MainState:draw()
    local w, h = love.graphics.getWidth(), love.graphics.getHeight()
    if not self.canvas or self.canvas:getWidth() ~= w or self.canvas:getHeight() ~= h then
        self.canvas = love.graphics.newCanvas(w, h)
    end
    love.graphics.setCanvas(self.canvas)
    love.graphics.setBackgroundColor(0, 0, 0)
    love.graphics.clear()

    love.graphics.push()
    love.graphics.translate(self:getOffset())

    self:world():draw()
    love.graphics.pop()

    -- draw darkness
    resources:sendShaderValue("darkness", "range", 46 * 2.5 * (1 + 0.05 * (math.sin(self.lifetime * 2))))
    resources:sendShaderValue("darkness", "blur", 128)
    resources:sendShaderValue("darkness", "width", love.graphics.getWidth())
    resources:sendShaderValue("darkness", "height", love.graphics.getHeight())

    resources:sendShaderValue("darkness", "shadowmap", self.player.shadowMap)
    if not debug then
        love.graphics.setPixelEffect(resources.shaders.darkness)
        love.graphics.setColor(0, 0, 0)
        love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
        love.graphics.setPixelEffect()
    end

    love.graphics.setColor(255, 255, 255)
    if debug then
        love.graphics.setFont(resources.fonts.tiny)
        love.graphics.print(love.timer.getFPS() .. " FPS", 10, 10)
    end

    love.graphics.setFont(resources.fonts.handBig)
    if activeActionObject then
        local t = "[E] " .. activeActionObject.actionText
        love.graphics.print(t, love.graphics.getWidth() / 2 -  love.graphics.getFont():getWidth(t) / 2, love.graphics.getHeight() - 100)
    end


    -- distortion shader
    if not debug then
        resources:sendShaderValue("distort", "lifetime", self.lifetime)
        resources:sendShaderValue("distort", "distortion", 1)
        resources:sendShaderValue("distort", "danger", 0) -- TODO determine danger
        love.graphics.setPixelEffect(resources.shaders.distort)
    end
    love.graphics.setColor(255, 255, 255)
    love.graphics.setCanvas()
    love.graphics.draw(self.canvas, 0, 0)
    love.graphics.setPixelEffect()

    if self.levelFade > 0 then
        love.graphics.setColor(0, 0, 0, math.sin(math.pi * self.levelFade) * 255)
        love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
    end
end

function MainState:update(dt)
    self.lifetime = self.lifetime + dt

    self.player:bakeShadows(self:world().physicsWorld)
    self:world():update(dt)

    if self.levelFade > 0 then
        if self.nextLevel and self.levelFade < 0.5 then
            self:setLevel(self.nextLevel)
            self.nextLevel = nil
        end

        self.levelFade = self.levelFade - dt * 0.6
        if self.levelFade < 0 then self.levelFade = 0 end
    end
end

function MainState:keypressed(k, u)
    if k == "escape" then
        stack:pop()
    elseif k == "f" then
        file.number = "21494"
        stack:push(file)
    elseif k == "e" then
        if activeActionObject then
            activeActionObject:onAction()
            activeActionObject = nil
        end
    elseif k == "up" then
        self:fadeToLevel("+1")
    elseif k == "down" then
        self:fadeToLevel("-1")
    end
end
