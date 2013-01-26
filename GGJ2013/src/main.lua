require("states/introstate")
require("states/mainstate")
require("states/menustate")
require("states/creditsstate")
require("states/filestate")
require("core/resources")
require("core/gamestack")
require("core/settings")

resources = Resources("data/")
settings = Settings()
settings:load()

debug = settings:get("debug", false)
fullscreen = settings:get("fullscreen", false)

function reset()
    -- start game
    intro = IntroState()
    menu = MenuState()
    credits = CreditsState()
    file = FileState()
    main = MainState()
    stack = GameStack()
    stack:push(debug and main or intro)
end

function love.load()
    if fullscreen then
        setFullscreen(true)
    end

    math.randomseed(os.time())

    -- load images
    resources:addImage("level_decorations", "levels/tilesets/decorations.png")
    resources:addImage("level_base", "levels/tilesets/base.png")
    resources:addImage("player_head", "gfx/player_head.png")
    resources:addImage("player_anim", "gfx/player_anim.png")
    resources:addImage("file", "gfx/file.png")
    resources:addImage("zombie", "gfx/zombie.png")
    resources:addImage("slime", "gfx/slime.png")
    resources:addImage("fire", "gfx/fire.png")
    resources:addImage("spike", "gfx/spike.png")
    resources:addImage("heart", "gfx/heart.png")
    resources:addImage("bug", "gfx/mob-animation.png")
    resources:addImage("doorBot", "gfx/door_bot.png")
    resources:addImage("doorTop", "gfx/door_top.png")
    resources:addImage("ggj_logo", "gfx/ggj_logo.png")
    resources:addImage("headphones", "gfx/headphones.png")
    resources:makeGradientImage("fill", {255, 255, 255}, {255, 255, 255})

    -- load fonts
    resources:addFont("tiny", "DejaVuSans.ttf", 10)
    resources:addFont("normal", "DejaVuSans.ttf", 20)
    resources:addFont("huge", "DejaVuSans.ttf", 40)

    -- load sounds
    resources:addAudio("door_slide", "sfx/door_slide.ogg")
    resources:addAudio("walk_high", "sfx/walk_high.ogg")
    resources:addAudio("walk_low", "sfx/walk_low.ogg")
    resources:addAudio("heartbeat", "sfx/heartbeat.ogg")
    resources:addAudio("heartbeat_pulse", "sfx/heartbeat_pulse.ogg")

    -- load shaders
    resources:addShader("darkness", "shaders/darkness.glsl")

    resources:load()

    love.physics.setMeter(64)

    reset()
end

function love.update(dt)
    stack:update(dt)
end

function love.draw()
    stack:draw()

    -- love.graphics.setFont(resources.fonts.tiny)
    -- love.graphics.print("FPS: " .. love.timer.getFPS(), 5, 5)
end

function setFullscreen(fs)
    if fs then
        -- save old window size
        windowedSizeX, windowedSizeY = love.graphics.getMode()

        modes = love.graphics.getModes()
        table.sort(modes, function(a, b) return a.width*a.height < b.width*b.height end)
        local nativeResolution = modes[#modes]
        love.graphics.setMode(nativeResolution["width"], nativeResolution["height"], true)
    else
        love.graphics.setMode(windowedSizeX, windowedSizeY, false)
    end
    fullscreen = fs
end

function love.keypressed(k, u)
    stack:keypressed(k, u)

    if k == "f11" then
        setFullscreen(not fullscreen)
    elseif k == "f9" then
        debug = not debug
    end
end

function love.mousepressed(x, y, button)
    stack:mousepressed(x, y, button)
end

function love.quit()
    settings:set("fullscreen", fullscreen)
    settings:set("debug", debug)
    settings:save()
end
