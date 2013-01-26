require("states/introstate")
require("states/mainstate")
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
    resources:addImage("player", "gfx/player.png")
    resources:addImage("player_anim", "gfx/player_anim.png")
    resources:addImage("file", "gfx/file.png")
    
    -- load fonts
    resources:addFont("tiny", "DejaVuSans.ttf", 10)
    resources:addFont("normal", "DejaVuSans.ttf", 20)

    -- load sounds
    resources:addAudio("door_slide", "sfx/door_slide.ogg")

    -- load shaders
    resources:addShader("darkness", "shaders/darkness.glsl")

    resources:load()

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
