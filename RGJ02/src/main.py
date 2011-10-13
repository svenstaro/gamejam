#! /usr/bin/env python
from gloss import *
import random

WIN_SIZE = (800, 600)
DEV = False

class Game(GlossGame):
	def preload_content(self):
		self.meteor_mode = "normal" # Current shape of meteor
		self.on_key_down = handle_key_down # Key handler
		self.on_key_up = handle_key_up # Key handler
		self.score = 0 # Current score
		self.speed = 1.0 # Is updated according to elapsed time
		self.block_size = 0 # Image block size for bg tiles
		self.blockade_area = [] # Site of blockade area between walls
		self.blockades = [] # Current on-screen blockades
		self.time_since_last_blockade = 0.0 # Resets on new blocakade spawn
		self.time_between_blockades = 5.0 # Increased difficulty decreases this
		self.player_pos = [WIN_SIZE[0] / 2, -250] # Out-of-screen default pos
		self.player_final_pos = 250 # Final position for intro
		self.player_energy = 100.0 # Death on player_energy <= 0
		self.current_level = 0
		self.damage_effect_value = 0 # For hit indicator
		self.refill_effect_value = 0 # For refill indicator
		self.current_game_time = 0.0 # Time a particular level has been running
		self.game_over = False
		random.seed() # Make awesome random numbers

	def draw_loading_screen(self):
		pass

	def load_content(self):
		pygame.mixer.music.load("../snd/THA - Back In The Day.ogg")
		if not DEV: # Do not play annoying music during testing!
			pygame.mixer.music.play(-1)

		# Assemble screen-filling scroll graphics into surface in order to
		# spare lots of draw calls
		bg_surface = pygame.Surface((WIN_SIZE[0], WIN_SIZE[1] * 2))

		# Load background tile stuff and blit it onto the main Surface
		sfc_bg_gray = pygame.image.load("../gfx/bg_gray.png").convert()
		required_bg_tiles_x = bg_surface.get_width() / sfc_bg_gray.get_width()
		required_bg_tiles_y = bg_surface.get_height() / sfc_bg_gray.get_height()
		for bg_tile_x in xrange(0, required_bg_tiles_x):
			for bg_tile_y in xrange(0, required_bg_tiles_y):
				position_x = bg_tile_x * sfc_bg_gray.get_width()
				position_y = bg_tile_y * sfc_bg_gray.get_height()
				bg_surface.blit(sfc_bg_gray, (position_x, position_y))

		# Load wall tile stuff and blit it onto the main Surface
		# We only need to blit left and right walls here
		sfc_wall_gray = pygame.image.load("../gfx/wall_gray.png").convert()
		required_wall_tiles_y = bg_surface.get_height() / sfc_wall_gray.get_height()
		for wall_tile_left in xrange(0, required_wall_tiles_y):
			position_y = wall_tile_left * sfc_wall_gray.get_height()
			bg_surface.blit(sfc_wall_gray, (0, position_y))
		for wall_tile_right in xrange(0, required_wall_tiles_y):
			position_y = wall_tile_right * sfc_wall_gray.get_height()
			bg_surface.blit(sfc_wall_gray,
					(bg_surface.get_width() - sfc_wall_gray.get_width(), position_y))

		# Create Gloss Texture and Surface for background Surface
		self.tex_bg = Texture(bg_surface)
		self.spt_bg = Sprite(self.tex_bg)
		self.block_size = sfc_wall_gray.get_width()

		# Figure out maximum blockade with (area between both walls)
		block_img = pygame.image.load("../gfx/block_fire.png")
		self.blockade_area = [WIN_SIZE[0] - 2 * block_img.get_width(),
								block_img.get_height()]

		# Load block graphics and assemble them into a Surface each
		fire_block_surface = pygame.Surface(self.blockade_area)
		fire_block_img = pygame.image.load("../gfx/block_fire.png").convert()
		ice_block_surface = pygame.Surface(self.blockade_area)
		ice_block_img = pygame.image.load("../gfx/block_ice.png").convert()
		light_block_surface = pygame.Surface(self.blockade_area)
		light_block_img = pygame.image.load("../gfx/block_light.png").convert()
		dark_block_surface = pygame.Surface(self.blockade_area)
		dark_block_img = pygame.image.load("../gfx/block_dark.png").convert()
		up_block_surface = pygame.Surface(self.blockade_area)
		up_block_img = pygame.image.load("../gfx/block_up.png").convert()
		down_block_surface = pygame.Surface(self.blockade_area)
		down_block_img = pygame.image.load("../gfx/block_down.png").convert()
		plus_block_surface = pygame.Surface(self.blockade_area)
		plus_block_img = pygame.image.load("../gfx/block_plus.png").convert()
		minus_block_surface = pygame.Surface(self.blockade_area)
		minus_block_img = pygame.image.load("../gfx/block_minus.png").convert()
		required_block_tiles_x = \
			self.blockade_area[0] / fire_block_img.get_width()
		for block_tile_x in xrange(0, required_block_tiles_x):
			position_x = block_tile_x * fire_block_img.get_width()
			fire_block_surface.blit(fire_block_img, (position_x, 0))
			ice_block_surface.blit(ice_block_img, (position_x, 0))
			light_block_surface.blit(light_block_img, (position_x, 0))
			dark_block_surface.blit(dark_block_img, (position_x, 0))
			up_block_surface.blit(up_block_img, (position_x, 0))
			down_block_surface.blit(down_block_img, (position_x, 0))
			plus_block_surface.blit(plus_block_img, (position_x, 0))
			minus_block_surface.blit(minus_block_img, (position_x, 0))

		self.tex_block_fire = Texture(fire_block_surface)
		self.tex_block_ice = Texture(ice_block_surface)
		self.tex_block_light = Texture(light_block_surface)
		self.tex_block_dark = Texture(dark_block_surface)
		self.tex_block_up = Texture(up_block_surface)
		self.tex_block_down = Texture(down_block_surface)
		self.tex_block_plus = Texture(plus_block_surface)
		self.tex_block_minus = Texture(minus_block_surface)

		# Load meteor graphics
		self.tex_meteor_normal = Texture("../gfx/meteor_normal.png")
		self.spt_meteor_normal = Sprite(self.tex_meteor_normal,
										position = self.player_pos)
		self.tex_meteor_fire = Texture("../gfx/meteor_fire.png")
		self.spt_meteor_fire = Sprite(self.tex_meteor_fire,
										position = self.player_pos)
		self.tex_meteor_ice = Texture("../gfx/meteor_ice.png")
		self.spt_meteor_ice = Sprite(self.tex_meteor_ice,
										position = self.player_pos)
		self.tex_meteor_light = Texture("../gfx/meteor_light.png")
		self.spt_meteor_light = Sprite(self.tex_meteor_light,
										position = self.player_pos)
		self.tex_meteor_dark = Texture("../gfx/meteor_dark.png")
		self.spt_meteor_dark = Sprite(self.tex_meteor_dark,
										position = self.player_pos)
		self.tex_meteor_up = Texture("../gfx/meteor_up.png")
		self.spt_meteor_up = Sprite(self.tex_meteor_up,
										position = self.player_pos)
		self.tex_meteor_down = Texture("../gfx/meteor_down.png")
		self.spt_meteor_down = Sprite(self.tex_meteor_down,
										position = self.player_pos)
		self.tex_meteor_plus = Texture("../gfx/meteor_plus.png")
		self.spt_meteor_plus = Sprite(self.tex_meteor_plus,
										position = self.player_pos)
		self.tex_meteor_minus = Texture("../gfx/meteor_minus.png")
		self.spt_meteor_minus = Sprite(self.tex_meteor_minus,
										position = self.player_pos)

		# Load energy bar
		self.tex_energy_bar = Texture("../gfx/energy_bar.png")
		self.spt_energy_bar = Sprite(self.tex_energy_bar,
				position = (WIN_SIZE[0] - self.block_size / 2, WIN_SIZE[1] / 2))

		# Load particles
		self.tex_part_normal = Texture("../gfx/particle_normal.png")
		self.part_normal = ParticleSystem(self.tex_part_normal,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 4.0, lifespan = -1,
							wind = (0, -100), minspeed = 5, maxspeed = 100)

		self.tex_part_fire = Texture("../gfx/particle_fire.png")
		self.part_fire = ParticleSystem(self.tex_part_fire,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 4.0, lifespan = -1,
							wind = (0, -100), minspeed = 5, maxspeed = 100)
		self.part_fire.additive = True

		self.tex_part_ice = Texture("../gfx/particle_ice.png")
		self.part_ice = ParticleSystem(self.tex_part_ice,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 4.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 100)
		self.part_ice.additive = True

		self.tex_part_light = Texture("../gfx/particle_light.png")
		self.part_light = ParticleSystem(self.tex_part_light,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 100, growth = 4.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 10)
		self.part_light.additive = True

		self.tex_part_dark = Texture("../gfx/particle_dark.png")
		self.part_dark = ParticleSystem(self.tex_part_dark,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 4.0, lifespan = -1,
							wind = (0, -30), minspeed = 5, maxspeed = 50)

		self.tex_part_up = Texture("../gfx/particle_up.png")
		self.part_up = ParticleSystem(self.tex_part_up,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 100, growth = 4.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 100)
		self.part_up.additive = True

		self.tex_part_down = Texture("../gfx/particle_down.png")
		self.part_down = ParticleSystem(self.tex_part_down,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 100, growth = 4.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 100)
		self.part_down.additive = True

		self.tex_part_plus = Texture("../gfx/particle_plus.png")
		self.part_plus = ParticleSystem(self.tex_part_plus,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 1.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 100)
		self.part_plus.additive = True

		self.tex_part_minus = Texture("../gfx/particle_minus.png")
		self.part_minus = ParticleSystem(self.tex_part_minus,
							position = self.player_pos, initialparticles = 10,
							creationspeed = 10, growth = 1.0, lifespan = -1,
							wind = (0, 0), minspeed = 5, maxspeed = 100)
		self.part_minus.additive = True

		# Load help
		self.tex_help = Texture("../gfx/help.png")
		self.spt_help = Sprite(self.tex_help, position = (0, 50))

		# Load font
		self.fnt_main = SpriteFont("../gfx/impact.ttf", 40)

	def draw(self):
		# Shake screen so the pixels fall out
		Gloss.clear()

		# Draw background Sprite and its scrolling comrade
		self.spt_bg.draw()

		# Draw all blockades
		for blockade in self.blockades:
			blockade.draw()

		# Draw player graphic according to current mode
		if self.meteor_mode == "normal":
			self.part_normal.draw()
			self.spt_meteor_normal.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "fire":
			self.part_fire.draw()
			self.spt_meteor_fire.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "ice":
			self.part_ice.draw()
			self.spt_meteor_ice.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "light":
			self.part_light.draw()
			self.spt_meteor_light.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "dark":
			self.part_dark.draw()
			self.spt_meteor_dark.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "up":
			self.part_up.draw()
			self.spt_meteor_up.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "down":
			self.part_down.draw()
			self.spt_meteor_down.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "plus":
			self.part_plus.draw()
			self.spt_meteor_plus.draw(position = self.player_pos,
										origin = None)
		if self.meteor_mode == "minus":
			self.part_minus.draw()
			self.spt_meteor_minus.draw(position = self.player_pos,
										origin = None)

		# Display introduction
		if self.current_level is 0:
			# I already have no idea what I just did here
			if 2.0 < Gloss.total_seconds < 9.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(Gloss.total_seconds / 7.0) - 2.0 / 7.0)
				self.fnt_main.draw("This is what happened one day in the",
									position = (40, WIN_SIZE[1] / 2 - 40),
									color = Color(val, val, val, val),
									letterspacing = val)
			if 4.0 < Gloss.total_seconds < 16.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(Gloss.total_seconds / 12.0) - 4.0 / 12.0)
				self.fnt_main.draw("Tower Of Opposites",
									position = (40, WIN_SIZE[1] / 2 + 10),
									color = Color(val * random.random(),
													val * random.random(),
													val * random.random(),
													val),
									letterspacing = val, scale = 2.0 +
									random.randint(0, 10) / 100.0)
			if 9.0 < Gloss.total_seconds < 20.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(Gloss.total_seconds / 11.0) - 9.0 / 11.0)
				self.fnt_main.draw("Suddenly, there was a shattering noise...",
									position = (40, WIN_SIZE[1] / 2 - 40),
									color = Color(val, val, val, val),
									letterspacing = val, scale = 1.0)
			if 16.0 < Gloss.total_seconds < 24.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(Gloss.total_seconds / 8.0) - 16.0 / 8.0)
				self.fnt_main.draw("A ball of fire and smoke came falling down",
									position = (40, WIN_SIZE[1] / 2 + 20),
									color = Color(val, val, val, val),
									letterspacing = val, scale = 1.0)
		else:
			if 0.0 < self.current_game_time < 3.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(self.current_game_time / 3.0) - 0.0 / 5.0)
				self.fnt_main.draw("Level "+str(self.current_level),
									position = (WIN_SIZE[0] / 2 - 100,
												WIN_SIZE[1] / 2 - 250),
									color = Color(val, val, val, val),
									scale = 2.0 * val,
									letterspacing = val)
			if self.current_level is 1:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Be the opposite! Easy for now...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level is 2:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Some more opposites...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level is 3:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Getting more difficult...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level is 4:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Well, this is quite hard...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level is 5:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("SHIT...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level is 5:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Sweet zombie jesus...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if self.current_level > 5:
				if 0.0 < self.current_game_time < 5.0:
					val = Gloss.smooth_step2(0.0, 1.0,
						(self.current_game_time / 5.0) - 0.0 / 5.0)
					self.fnt_main.draw("Now you will burn...",
										position = (40, WIN_SIZE[1] / 2 - 40),
										color = Color(val, val, val, val),
										letterspacing = val)
			if 4.0 < self.current_game_time < 5.0:
				val = Gloss.smooth_step2(0.0, 1.0,
					(self.current_game_time / 1.0) - 4.0 / 1.0)
				self.fnt_main.draw("START",
									position = (WIN_SIZE[0] / 2 - 100,
												WIN_SIZE[1] / 2 - 100),
									color = Color(val, val, val, val),
									scale = 2.0 * val,
									letterspacing = val)
			# Display score
			self.fnt_main.draw("Score: "+str(self.score),
								position = (20, 10), color = Color.WHITE)
			# Display level
			self.fnt_main.draw("Level: "+str(self.current_level),
								position = (20, 530), color = Color.WHITE)

			# Draw help
			self.spt_help.draw()

			# Draw energy bar
			Gloss.draw_box(position = (WIN_SIZE[0] - self.block_size / 2,
										WIN_SIZE[1] / 2),
							width = 38, height = self.player_energy * 5 - 10,
							origin = None,
							color = Color.lerp(Color(0.0, 1.0, 0.0, 0.9),
												Color(0.2, 0.0, 0.0, 0.9),
												(1 / self.player_energy)*30))
			self.spt_energy_bar.draw(origin = None)

		# Display warning if game is running at less than 60 FPS
		if Gloss.running_slowly and DEV:
			self.fnt_main.draw("SLOW", position = (WIN_SIZE[0] - 100, 0),
								color = Color.RED)

		if DEV:
			self.fnt_main.draw("Speed: "+str(self.speed), position = (20, 400),
								color = Color.RED)
			self.fnt_main.draw("Time betweeen blockades: "+\
								str(self.time_between_blockades),
								position = (20, 440), color = Color.RED)
			self.fnt_main.draw("Current game time: "+\
								str(self.current_game_time),
								position = (20, 480), color = Color.RED)

	def update(self):
		# Do input stuff
		key = pygame.key.get_pressed()
		if key[K_q]:
			self.meteor_mode = "fire"
		if key[K_a]:
			self.meteor_mode = "ice"
		if key[K_w]:
			self.meteor_mode = "light"
		if key[K_s]:
			self.meteor_mode = "dark"
		if key[K_e]:
			self.meteor_mode = "up"
		if key[K_d]:
			self.meteor_mode = "down"
		if key[K_r]:
			self.meteor_mode = "plus"
		if key[K_f]:
			self.meteor_mode = "minus"
		if not key[K_q] and not key[K_a] and not key[K_w] and not key[K_s] and\
			not key[K_e] and not key[K_d] and not key[K_r] and not key[K_f]:
			self.meteor_mode = "normal"

		# Update particle system positions
		self.part_normal.position = self.player_pos
		self.part_fire.position = self.player_pos
		self.part_ice.position = self.player_pos
		self.part_light.position = self.player_pos
		self.part_dark.position = self.player_pos
		self.part_up.position = self.player_pos
		self.part_down.position = self.player_pos
		self.part_plus.position = self.player_pos
		self.part_minus.position = self.player_pos

		# Move background Sprite
		if self.spt_bg.position[1] <= 0 - self.spt_bg.texture.height / 2:
			self.spt_bg.move_to(None, 0)
		self.spt_bg.move(0, -self.speed * Gloss.elapsed_seconds * 100)

		# Play introduction
		if self.current_level is 0:
			if 20.0 < Gloss.total_seconds < 30.0:
				val = Gloss.bounce_out(-self.player_final_pos,
										self.player_final_pos,
								(Gloss.total_seconds / 10.0) - 20.0 / 10.0)
				self.player_pos[1] = val
			elif Gloss.total_seconds >= 30.0:
				self.current_level = 1
		else:
			self.player_pos = [WIN_SIZE[0] / 2, 250]

		# Check for collisions
		for blockade in self.blockades:
			blockade_center = (blockade.position[0] + blockade.texture.width / 2,
							blockade.position[1] + blockade.texture.height / 2)
			if Point.distance(self.player_pos, blockade_center) < 20:
				if blockade.id == "fire" and self.meteor_mode == "ice":
					self.refill_energy()
				elif blockade.id == "ice" and self.meteor_mode == "fire":
					self.refill_energy()
				elif blockade.id == "light" and self.meteor_mode == "dark":
					self.refill_energy()
				elif blockade.id == "dark" and self.meteor_mode == "light":
					self.refill_energy()
				elif blockade.id == "up" and self.meteor_mode == "down":
					self.refill_energy()
				elif blockade.id == "down" and self.meteor_mode == "up":
					self.refill_energy()
				elif blockade.id == "plus" and self.meteor_mode == "minus":
					self.refill_energy()
				elif blockade.id == "minus" and self.meteor_mode == "plus":
					self.refill_energy()
				else:
					self.damage_effect_value = 1
					self.player_energy -= 50 * Gloss.elapsed_seconds
					if self.player_energy <= 0:
						self.player_energy = 10 * Gloss.elapsed_seconds
						self.game_over = True
						# The player died, restart level
						self.reset_scene()

		# Update current game time
		self.current_game_time += Gloss.elapsed_seconds

		if self.current_level is 0:
			self.speed = 1.0
			self.current_game_time = 0.0
		elif self.current_game_time > 5.0: # Wait 5 seconds before starting level
			# Check if level is won (after 40 secs of survival)
			if self.current_game_time >= 40.0:
				self.reset_scene()
				self.current_level += 1

			# Hit indicator
			if self.damage_effect_value > 0:
				self.damage_effect_value -= Gloss.elapsed_seconds
				color = Gloss.smooth_step(1, 0, self.damage_effect_value)
				Gloss.set_scene_tint(Color(1, color, color))
			elif self.damage_effect_value <= 0:
				self.damage_effect_value = 0.0

			# Refill indicator
			if self.refill_effect_value > 0:
				self.refill_effect_value -= Gloss.elapsed_seconds
				color = Gloss.smooth_step(1, 0.6, self.refill_effect_value)
				Gloss.set_scene_tint(Color(color, 1, color))
			elif self.refill_effect_value <= 0:
				self.refill_effect_value = 0.0

			# Reset screen
			if self.damage_effect_value <= 0 and self.refill_effect_value <= 0:
				Gloss.set_scene_tint(Color.WHITE)

			# Increase speed
			if self.current_game_time > 5.0:
				self.speed += \
					self.current_level / (500.0 * self.current_game_time + 1)
			else:
				self.speed = 1.0

			# Decrease time between blockades (difficutly)
			if self.current_game_time > 5.0:
				self.time_between_blockades -= \
						(self.current_level / 2.0) / (100.0 * self.current_game_time + 1)
			else:
				self.time_between_blockdes = 5.0

			# Update score
			self.score = int(self.current_game_time * 250)

			# Update blockade time
			self.time_since_last_blockade += Gloss.elapsed_seconds

			# Create blockade randomly but in proportion to current score
			if self.time_since_last_blockade > self.time_between_blockades and \
													random.randint(0, 300) < 30:
				if self.current_level is 1:
					blockade_types = ["fire", "ice"]
				if self.current_level is 2:
					blockade_types = ["fire", "ice", "light", "dark"]
				if self.current_level is 3:
					blockade_types = ["fire", "ice", "light", "dark",
										"up", "down"]
				if self.current_level is 4:
					blockade_types = ["fire", "ice", "light", "dark",
										"up", "down", "plus", "minus"]
				if self.current_level >= 5:
					blockade_types = ["fire", "ice", "light", "dark",
										"up", "down", "plus", "minus"]
				choice = random.choice(blockade_types)
				pos = (self.block_size, WIN_SIZE[1])
				if choice == "fire":
					blockade = Sprite(self.tex_block_fire, pos)
					blockade.id = "fire"
					self.blockades.append(blockade)
				elif choice == "ice":
					blockade = Sprite(self.tex_block_ice, pos)
					blockade.id = "ice"
					self.blockades.append(blockade)
				elif choice == "light":
					blockade = Sprite(self.tex_block_light, pos)
					blockade.id = "light"
					self.blockades.append(blockade)
				elif choice == "dark":
					blockade = Sprite(self.tex_block_dark, pos)
					blockade.id = "dark"
					self.blockades.append(blockade)
				elif choice == "up":
					blockade = Sprite(self.tex_block_up, pos)
					blockade.id = "up"
					self.blockades.append(blockade)
				elif choice == "down":
					blockade = Sprite(self.tex_block_down, pos)
					blockade.id = "down"
					self.blockades.append(blockade)
				elif choice == "plus":
					blockade = Sprite(self.tex_block_plus, pos)
					blockade.id = "plus"
					self.blockades.append(blockade)
				elif choice == "minus":
					blockade = Sprite(self.tex_block_minus, pos)
					blockade.id = "minus"
					self.blockades.append(blockade)

				self.time_since_last_blockade = 0.0

			# Move current blockades and delete out-of-screen ones
			new_blockades = [] # List containing non-out-of-range blockades
			for blockade in self.blockades:
				blockade.move(0, -self.speed * Gloss.elapsed_seconds * 100)
				if blockade.position[1] > 0 - self.blockade_area[1]:
					new_blockades.append(blockade)
			self.blockades = new_blockades

	def refill_energy(self):
		if self.player_energy <= 100:
			self.player_energy += 0.10
		self.refill_effect_value = 1

	def reset_scene(self):
		game.current_game_time = 0.0
		game.speed = 1.0
		game.blockades = []
		game.player_energy = 100.0
		game.score = 0
		game.time_between_blockades = 5.0
		game.damage_effect_value = 0.0
		Gloss.set_scene_tint(Color.WHITE)

# Actual game object
game = Game("Tower Of Opposites")

# Event handlers
def handle_key_down(event):
	if event.key is K_ESCAPE:
		game.quit()
	if event.key is K_SPACE and game.current_level is 0:
		game.current_level = 1
	if event.key is K_SPACE and game.current_level is not 0:
		# Reset game state
		game.reset_scene()
# Original key input didn't work so well
#	if event.key is K_q:
#		game.meteor_mode = "fire"
#	elif event.key is K_w:
#		game.meteor_mode = "ice"

def handle_key_up(event):
	pass
# Original key input didn't work so well
#	if event.key is K_q or event.key is K_w:
#		game.meteor_mode = "normal"

Gloss.screen_resolution = WIN_SIZE
game.run()
