-- game state

require("util/helper")

GameState = class("GameState")

function GameState:__init()end

function GameState:start()end
function GameState:stop()end
function GameState:onUpdate(dt)end
function GameState:onDraw()end
function GameState:onKeyPressed(k, u)end
function GameState:onMousePressed(x, y, button)end
