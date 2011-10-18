#!/usr/bin/env python2
import pygame
import random
import os
import sys
from copy import copy
from vec2d import vec2d

class Level(object):
	def __init__(self, lvlno, screen):
		self.lvlno = lvlno
		self.image = "".join(["level", str(self.lvlno), ".png"])
		self.image = os.path.join("img", self.image)
		self.image = pygame.image.load(self.image).convert_alpha()

		self.hitmask = pygame.mask.from_surface(self.image)

		self.screen = screen

	def has_won(self, player):
		if player.pos[0] < 0:
			return True


class Enemy(object):
	def __init__(self, type, screen, init_pos, player, rotation=0, rotspeed=0, move_x=0, move_y=0):
		self.type = type
		self.image = "".join([type, ".png"])
		self.image = os.path.join("img", self.image)
		self.base_image = pygame.image.load(self.image).convert_alpha()
		#self.image = pygame.transform.rotozoom(self.base_image, rotation, 1.1)
		self.image = pygame.transform.rotate(self.base_image, rotation)
		self.image = pygame.transform.flip(self.image, True, False)
		
		self.screen = screen

		self.init_pos = init_pos
		self.pos = copy(init_pos)

		self.hitmasksurf = pygame.Surface(screen.get_size()).convert_alpha()

		self.rotspeed = rotspeed
		self.rotation = rotation
		self.move_x = move_x
		self.move_y = move_y
		
	def collide(self):
		self.hitmasksurf.fill((0,0,0,0))
		self.hitmasksurf.blit(self.image, self.pos)
		self.hitmask = pygame.mask.from_surface(self.hitmasksurf)
		return check_collision(player, self)

	def blit(self):
		draw_pos = self.pos
		if self.move_x != 0:
			if self.move_x > 0:
				self.pos[0] += 2
			if self.move_x < 0:
				self.pos[0] -= 2
			if abs(self.init_pos[0] - self.pos[0]) >= abs(self.move_x):
				self.move_x = -self.move_x
				self.image = pygame.transform.flip(self.image, True, False)
		if self.move_y != 0:
			if self.move_y > 0:
				self.pos[1] += 2
			if self.move_y < 0:
				self.pos[1] -= 2
			if abs(self.init_pos[1] - self.pos[1]) >= abs(self.move_y):
				self.move_y = -self.move_y
		if self.rotspeed != 0:
			self.rotation += self.rotspeed
			self.image = pygame.transform.rotate(self.base_image, self.rotation)
			self.pos = self.image.get_rect().move(
				self.init_pos[0] - self.image.get_width() / 2,
				self.init_pos[1] - self.image.get_height() / 2)

		self.screen.blit(self.image, self.pos)


class Powerup(object):
	def __init__(self, type, screen, init_pos, player):
		self.type = type
		self.image = "".join([type, ".png"])
		self.image = os.path.join("img", self.image)
		self.image = pygame.image.load(self.image).convert_alpha()

		self.screen = screen

		self.pos = init_pos

		self.hitmasksurf = pygame.Surface(screen.get_size()).convert_alpha()

	def collide(self):
		self.hitmasksurf.fill((0,0,0,0))
		self.hitmasksurf.blit(self.image, self.pos)
		self.hitmask = pygame.mask.from_surface(self.hitmasksurf)
		return check_collision(player, self)

	def blit(self):
		self.screen.blit(self.image, self.pos)


class Player(object):
	def __init__(self, type, screen, init_pos, init_direction, hp=3):
		self.type = type
		self.image = "".join([type, "hp-", str(hp), ".png"])
		self.image = os.path.join("img", self.image)
		self.base_image = pygame.image.load(self.image).convert_alpha()
		self.image = self.base_image

		self.screen = screen

		self.pos = vec2d(init_pos)

		self.direction = vec2d(init_direction).normalized()

		self.speed = 0
		self.rotspeed = 0

		#self.hitmask = pygame.mask.from_surface(self.image)
		self.collidebox = pygame.Surface((10, 10)).convert_alpha()
		self.hitmask = pygame.mask.from_surface(self.collidebox)

		self.hp = hp

	def update(self, tick_time, accel=0, rotation=0, collision=False):
		self.image = pygame.transform.rotozoom(self.base_image, -self.direction.angle, 1.1)
	
		# position acceleration
		if accel == 1:
			self.speed = self.speed + accel
		# position deacceleration
		self.speed = self.speed * 0.995
		if accel == -1:
			self.speed = self.speed * 0.94

		if self.speed >= 3:
			self.speed = 3
		if self.speed <= 0.1:
			self.speed = 0;

		displacement = vec2d(
			self.direction.x * self.speed,
			self.direction.y * self.speed)

		# rotation acceleration
		self.rotspeed = self.rotspeed + rotation/4.0
		# rotation deacceleration
		self.rotspeed = self.rotspeed * 0.98

		if self.rotspeed >= 2.0 and rotation == 1:
			self.rotspeed = 2.0
		if self.rotspeed <= 0.1 and rotation == 1:
			self.rotspeed = 0
		if self.rotspeed <= -2.0 and rotation == -1:
			self.rotspeed = -2.0
		if self.rotspeed >= -0.1 and rotation == -1:
			self.rotspeed = 0

		rotation = rotation + self.rotspeed

		self.direction.rotate(rotation)

		if collision:
			displacement = displacement * -5
		if collision >= 50:
			self.pos = (1200,300)

		self.pos -= displacement

	def blit(self):
		draw_pos = self.image.get_rect().move(
			self.pos.x - self.image.get_width() / 2,
			self.pos.y - self.image.get_height() / 2)
		self.screen.blit(self.image, draw_pos, self.image.get_rect())



def check_collision(obj1, obj2):
	"""
	Can check if two objects collide with pixel accuracy.
	Only obj1 can be moving.
	Both objs need two attributes:
	hitmask (mask.from_surface) and 
	pos (list or tuple of coordinates).
	"""
	return obj2.hitmask.overlap_area(obj1.hitmask, (int(obj1.pos[0]),
                                                    int(obj1.pos[1])))


RES = (1280,720)
DEBUG = False
DEBUG_VIEW = False
os.environ['SDL_VIDEO_WINDOW_POS'] = 'center'
pygame.init()
screen = pygame.display.set_mode(RES)
level = Level(1, screen)
player = Player("narwhal", screen, (1200 , 300), (180, 0))
FPS = 40
fps_clock = pygame.time.Clock() # clock object for FPS
pygame.key.set_repeat(FPS, FPS/2)
enemies = []
powerups = []

if DEBUG:
	collidebox = pygame.Surface((10, 10))
	collidebox.fill((255,0,0))
	test2 = pygame.Surface(RES)
	test2array = pygame.surfarray.array3d(test2)

gameover = False
while not gameover:
	if pygame.mixer.music.get_busy() != True:
		pygame.mixer.music.load("music.ogg")
		pygame.mixer.music.rewind()
		pygame.mixer.music.play()

	tick_time = fps_clock.tick(FPS) # milliseconds since last frame
	pygame.display.set_caption("The Narwhal Bacons At Midnight - FPS: %.2f" % (fps_clock.get_fps()))
	screen.fill([30,30,30])

	if level.has_won(player):
		level = Level(level.lvlno+1, screen)
		player.pos = (1200, 300)
		player.speed = 0
		player.rotspeed = 0
		powerups = []
		enemies = []

		# now account for per level stuff in the ugliest way possible!
		if level.lvlno == 1:
			powerups = []
		if level.lvlno == 2:
			powerups.append(Powerup("bacon", screen, [730,230], player))
		if level.lvlno == 3:
			enemies.append(Enemy("shark", screen, [510, 450], player, 0, move_x=150, move_y=5))
			powerups.append(Powerup("cake", screen, [523, 220], player))
		if level.lvlno == 4:
			enemies.append(Enemy("poolsclosed", screen, [740, 250], player, 0))
			enemies.append(Enemy("poolsclosed", screen, [246, 435], player, 190))
			enemies.append(Enemy("poolsclosed", screen, [510, 510], player, 270))
			enemies.append(Enemy("shark", screen, [500, 300], player, 0, move_x=100, move_y=50))
		if level.lvlno == 5:
			enemies.append(Enemy("poolsclosed", screen, [510, 390], player, 190))
			enemies.append(Enemy("shark", screen, [800, 500], player, 0, move_x=120, move_y=50))
			enemies.append(Enemy("shark", screen, [250, 400], player, 0, move_x=80))
			enemies.append(Enemy("shark", screen, [250, 500], player, 0, move_x=60))
		if level.lvlno == 6:
			enemies.append(Enemy("shark", screen, [850, 500], player, 0, move_x=120, move_y=60))
			enemies.append(Enemy("shark", screen, [250, 450], player, 0, move_x=80, move_y=50))
			enemies.append(Enemy("shark", screen, [600, 140], player, 0, move_x=60, move_y=40))
			enemies.append(Enemy("shoopdawoop", screen, [620, 250], player, 0))
		if level.lvlno == 7:
			enemies.append(Enemy("leek", screen, [550, 420], player, 0, rotspeed=4))
		if level.lvlno == 8:
			enemies.append(Enemy("leek", screen, [780, 400], player, 95, rotspeed=-2))
			enemies.append(Enemy("leek", screen, [400, 420], player, 0, rotspeed=-1.2))
		if level.lvlno == 9:
			powerups = []

	collisions = check_collision(player, level)
	if collisions:
		player.update(tick_time, accel=0, rotation=0, collision=collisions)
	else:
		player.update(tick_time, accel=0, rotation=0, collision=False)
	
	player.blit()
	for enemy in enemies:
		if enemy.collide():
			hp = player.hp - 1
			del(player)
			if hp == 0:
				hp = 3
				del level
				level = Level(1, screen)
				enemies = []
			player = Player("narwhal", screen, (1200, 300), (180, 0), hp)
			
		enemy.blit()

	for powerup in powerups:
		if powerup.collide():
			powerups.remove(powerup)
		powerup.blit()

	screen.blit(level.image, (0,0))

	if DEBUG:
		#print player.pos
		#print level.hitmask.overlap_area(player.hitmask, (player.pos))
		#print player.hitmask.get_bounding_rects()
		#print pygame.mouse.get_pos()
		screen.blit(collidebox, player.pos)
		
		if DEBUG_VIEW:
			try:
				#powersups[0].image.lock()
				for x in xrange(powerups[0].hitmask.get_width()):
					for y in xrange(powerups[0].hitmask.get_height()):
						if powerups[0].hitmask.get_at((x,y)) == 0:
							test2array[x][y] = (255,0,0)
						else:
							test2array[x][y] = (0,255,0)
				test2 = pygame.surfarray.make_surface(test2array)
				test2.set_alpha(100)
				screen.blit(test2, (0,0))
				print "done"
				#powersups[0].image.unlock()
			except: 
				pass

	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			gameover = True
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_UP:
				player.update(FPS, 1, 0)
			if event.key == pygame.K_RIGHT:
				player.update(FPS, 0, 1)
			if event.key == pygame.K_DOWN:
				player.update(FPS, -1, 0)
			if event.key == pygame.K_LEFT:
				player.update(FPS, 0, -1)
			if event.key == pygame.K_ESCAPE:
				gameover = True
		elif event.type == pygame.KEYUP:
			if event.key in (pygame.K_UP,pygame.K_DOWN):
				player.update(FPS, 0, 0)
			if event.key in (pygame.K_RIGHT,pygame.K_LEFT):
				player.update(FPS, 0, 0)
	pygame.display.update()

#if __name__ == '__main__':
#	sys.exit(main())
