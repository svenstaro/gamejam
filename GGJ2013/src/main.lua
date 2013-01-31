require("states/introstate")
require("states/mainstate")
require("states/menustate")
require("states/creditsstate")
require("states/filestate")
require("states/gameoverstate")
require("states/winstate")
require("core/resources")
require("core/gamestack")
require("core/settings")

resources = Resources("data/")
settings = Settings()
settings:load()
debug = settings:get("debug", false)
fullscreen = settings:get("fullscreen", true)

function reset()
    -- start game
    intro = IntroState()
    menu = MenuState()
    credits = CreditsState()
    file = FileState()
    main = MainState()
    gameover = GameOverState()
    stack = GameStack()
    winstate = WinState()
    stack:push(debug and main or intro)
end

function love.load()
    if fullscreen then
        setFullscreen(true)
    end

    math.randomseed(os.time())
    love.audio.setDistanceModel("linear")

    -- load images
    resources:addImage("heart", "gfx/heart.png")
    resources:addImage("level_base", "levels/tilesets/base.png")
    resources:addImage("level_baseDark", "levels/tilesets/baseDark.png")
    resources:addImage("level_decorations", "levels/tilesets/decorations.png")
    resources:addImage("level_decorations2", "levels/tilesets/decorations2.png")
    resources:addImage("level_decorations3", "levels/tilesets/decorations3.png")
    resources:addImage("level_ground", "levels/tilesets/ground.png")
    resources:addImage("player_head", "gfx/player_head.png")
    resources:addImage("player_anim", "gfx/player_anim.png")
    resources:addImage("file", "gfx/file.png")
    resources:addImage("zombie", "gfx/zombie.png")
    resources:addImage("slime", "gfx/slime.png")
    resources:addImage("akte", "gfx/akte.png")
    resources:addImage("fire", "gfx/fire.png")
    resources:addImage("crab", "gfx/crab.png")
    resources:addImage("spike", "gfx/spike.png")
    resources:addImage("doorBot", "gfx/door_bot.png")
    resources:addImage("doorTop", "gfx/door_top.png")
    resources:addImage("doorlight_green", "gfx/doorlight_green.png")
    resources:addImage("doorlight_red", "gfx/doorlight_red.png")
    resources:addImage("ggj_logo", "gfx/ggj_logo.png")
    resources:addImage("headphones", "gfx/headphones.png")

    resources:addImage("failure", "gfx/failure.png")
    resources:addImage("trigger", "gfx/trigger.png")
    resources:addImage("triggerOn", "gfx/triggerOn.png")
    resources:makeGradientImage("fill", {255, 255, 255}, {255, 255, 255})

    -- load fonts
    resources:addFont("tiny", "DejaVuSans.ttf", 10)
    resources:addFont("normal", "DejaVuSans.ttf", 20)
    resources:addFont("huge", "DejaVuSans.ttf", 40)
    resources:addFont("handSmall", "SwankyandMooMoo.ttf", 19)
    resources:addFont("handBig", "SwankyandMooMoo.ttf", 32)
    resources:addFont("handHuge", "SwankyandMooMoo.ttf", 64)
    -- load sounds
    resources:addSound("door_slide", "sfx/door_slide.ogg")
    resources:addSound("walk_high", "sfx/walking_footstep_1.ogg")
    resources:addSound("walk_normal", "sfx/walking_footstep_2.ogg")
    resources:addSound("walk_low", "sfx/walking_footstep_3.ogg")
    resources:addSound("monster_random_1", "sfx/monster_random_1.ogg")
    resources:addSound("monster_random_2", "sfx/monster_random_2.ogg")
    resources:addSound("monster_random_3_knurren", "sfx/monster_random_3_knurren.ogg")
    resources:addSound("monster_random_4", "sfx/monster_random_4.ogg")
    resources:addSound("final_monster_1", "sfx/final_monster_1.ogg", "stream")
    resources:addSound("final_monster_2", "sfx/final_monster_2.ogg", "stream")
    resources:addSound("knurren_bad_boy_2", "sfx/knurren_bad_boy_2.ogg", "stream")

    -- load background sounds
    resources:addSound("heartbeat", "sfx/heartbeat.ogg")
    resources:addSound("heartbeat_pulse", "sfx/heartbeat_pulse.ogg")
    resources:addMusic("dark_factory", "sfx/dark-factory.ogg", "stream")
    resources:addMusic("cave_theme", "sfx/cave-theme.ogg", "stream")
    resources:addMusic("AmbientSound1", "sfx/bgm_AmbientSound.ogg", "stream")
    resources:addMusic("AmbientSound2", "sfx/bgm_AmbientSounds002.ogg", "stream")
    resources:addMusic("AmbientSound3", "sfx/bgm_AmbientSounds006.ogg", "stream")
    resources:addMusic("SoftElectro1", "sfx/THA-emptychasms.ogg", "stream")
    resources:addMusic("SoftElectro2", "sfx/THA-simplythink.ogg", "stream")

    -- load shaders
    resources:addShader("darkness", "shaders/darkness.glsl")
    resources:addShader("distort", "shaders/distort.glsl")

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
