## Gloss
## Copyright (C) 2009 Paul Hudson (http://www.tuxradar.com/gloss)

## Gloss is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public License
## as published by the Free Software Foundation; either version 3
## of the License, or (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU Lesser General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

from __future__ import division

import math
import os
import pygame
import random
import signal
import sys
import threading
import time
import traceback
import weakref

from OpenGL.GL import *
OpenGL.ERROR_CHECKING = False
from OpenGL.GL.EXT.framebuffer_object import *
from OpenGL.GLU import *

from pygame.locals import *


class GlossGame(object):
	def __init__(self, name):
		Gloss.active_game = self

		Gloss.game_name = name
		Gloss.screen_resolution = 1280,720
		Gloss.full_screen = False
		Gloss.enable_multisampling = False

		# set empty event handlers
		self.on_mouse_motion = None
		self.on_mouse_down = None
		self.on_mouse_up = None
		self.on_key_down = None
		self.on_key_up = None
		self.on_quit = None
 		self.on_joy_axis_motion = None
 		self.on_joy_ball_motion = None
 		self.on_joy_button_down = None
 		self.on_joy_button_up = None
 		self.on_joy_hat_motion = None

	def preload_content(self):
		pass

	def load_content(self):
		pass

	def draw_loading_screen(self):
		pass

	def draw(self):
		pass

	def update(self):
		pass

	def gloss_initialise(self):
		os.environ['SDL_VIDEO_CENTERED'] = '1'
		#pygame.mixer.pre_init(44100, 16, 2, 4096)
		pygame.init()

 		pygame.joystick.init()
 		for i in range(pygame.joystick.get_count()):
 			joystick = pygame.joystick.Joystick(i)
 			joystick.init()
 			Gloss.joysticks.append(joystick)

		Gloss.game_clock = pygame.time.Clock()

		pygame.display.set_caption(Gloss.game_name)

		if Gloss.enable_multisampling:
			pygame.display.gl_set_attribute(pygame.locals.GL_MULTISAMPLEBUFFERS, 1)
			pygame.display.gl_set_attribute(pygame.locals.GL_MULTISAMPLESAMPLES, 4)

		if Gloss.full_screen:
			pygame.display.set_mode(Gloss.screen_resolution, pygame.OPENGL | pygame.DOUBLEBUF | pygame.FULLSCREEN)
		else:
			pygame.display.set_mode(Gloss.screen_resolution, pygame.OPENGL | pygame.DOUBLEBUF)

		if Gloss.screen_resolution == (0, 0):
			scr = pygame.display.get_surface()
			Gloss.screen_resolution = scr.get_size()

		Gloss.MaxTextureSize = glGetInteger(GL_MAX_TEXTURE_SIZE);

		Gloss.clear(Color.CORNFLOWER_BLUE)
		pygame.display.flip()
		Gloss.clear(Color.CORNFLOWER_BLUE)

		glEnable(GL_CULL_FACE)
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
		
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST)
		glEnable(GL_LINE_SMOOTH)

		glEnable(GL_TEXTURE_2D)

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		glEnable(GL_BLEND)

		glEnable(GL_LIGHTING)
		glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)
		glEnable (GL_COLOR_MATERIAL)
		Gloss.set_scene_tint(Color.WHITE)

		glMatrixMode(GL_PROJECTION)
		glLoadIdentity()
		glOrtho(0, Gloss.screen_resolution[0], Gloss.screen_resolution[1], 0, -100, 100)

		Gloss.viewport_size = Gloss.screen_resolution

		glMatrixMode(GL_MODELVIEW)
		glLoadIdentity()

		self.preload_content()
		self.draw_loading_screen()
		pygame.display.flip()

		Gloss.tick_count = 0

		self.load_content()

	def gloss_internal_update(self):
		for particlesystem in reversed(Gloss.auto_particle_systems):
			# don't update this particle system if it's frozen
			if particlesystem.frozen:
				continue

			if particlesystem.update() is False:
				Gloss.auto_particle_systems.remove(particlesystem)
				particlesystem.alive = False

				if particlesystem.on_finish is not None:
					particlesystem.on_finish(particlesystem)

	def gloss_internal_draw(self):
		# if we take a screenshot after picking, we get the picking view rather than the real view
		# to fix this, save_screenshot() looks for Gloss.redraw_needed, and, if it's true, redraws the screen
		if Gloss.picking:
			Gloss.redraw_needed = True
		else:
			Gloss.redraw_needed = False

		self.draw()

	def run(self):
		self.gloss_initialise()

		Gloss.game_is_running = True

		while Gloss.game_is_running:
			Gloss.elapsed_seconds = Gloss.game_clock.tick(60) / 1000
			Gloss.total_seconds += Gloss.elapsed_seconds
			
			if Gloss.elapsed_seconds > 0.02:
				Gloss.running_slowly = True
			else:
				Gloss.running_slowly = False

			# tick_count is used in lots of places, so read it only once per update
			Gloss.tick_count = pygame.time.get_ticks()
		
			for event in pygame.event.get():
				if event.type == MOUSEMOTION:
					if self.on_mouse_down is not None:
						self.on_mouse_motion(event)

				if event.type == MOUSEBUTTONDOWN:
					if self.on_mouse_down is not None:
						self.on_mouse_down(event)

				if event.type == MOUSEBUTTONUP:
					if self.on_mouse_up is not None:
						self.on_mouse_up(event)

					if Gloss.sprite_click_tracking:
						Gloss.select_object(event.pos)

				if event.type == KEYDOWN:
					if event.mod & KMOD_LCTRL == KMOD_LCTRL:
						if event.key == K_ESCAPE:
							# Left Ctrl+Escape held - quit
							Gloss.game_is_running = False

					if self.on_key_down is not None:
						self.on_key_down(event)

				if event.type == KEYUP:
					if self.on_key_up is not None:
						self.on_key_up(event)

 				if event.type == JOYAXISMOTION:
 					if self.on_joy_axis_motion is not None:
 						self.on_joy_axis_motion(event)
 
 				if event.type == JOYBALLMOTION:
 					if self.on_joy_ball_motion is not None:
 						self.on_joy_ball_motion(event)
 
 				if event.type == JOYHATMOTION:
 					if self.on_joy_hat_motion is not None:
 						self.on_joy_hat_motion(event)
 
 				if event.type == JOYBUTTONDOWN:
 					if self.on_joy_button_down is not None:
 						self.on_joy_button_down(event)
 
 				if event.type == JOYBUTTONUP:
 					if self.on_joy_button_up is not None:
 						self.on_joy_button_up(event)

				if event.type == QUIT:
					Gloss.game_is_running = False

			# this all has to be done after the above events so that any changes from events
			# take place as soon as possible
			self.gloss_internal_update()
			self.update()

			self.gloss_internal_draw()

			pygame.display.flip()

		if self.on_quit is not None:
			self.on_quit()

		pygame.quit()

	def quit(self):
		Gloss.game_is_running = False

class Color(object):
	def __init__(self, r, g, b, a = 1.0):
		self.r = r
		self.g = g
		self.b = b
		self.a = a

	@staticmethod
	def from_bytes(r, g, b, a = 255):
		return Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0)

	@staticmethod
	def from_html(col):
		pygamecol = pygame.color.Color(col)
		return Color.from_pygame(pygamecol)

	@staticmethod
	def from_pygame(col):
		return Color(col[0] / 255, col[1] / 255, col[2] / 255, col[3] / 255)

	@staticmethod
	def lerp(colorfrom, colorto, amount):
		return Color(Gloss.lerp(colorfrom.r, colorto.r, amount), Gloss.lerp(colorfrom.g, colorto.g, amount), Gloss.lerp(colorfrom.b, colorto.b, amount), Gloss.lerp(colorfrom.a, colorto.a, amount))

	@staticmethod
	def lerp2(colorfrom, colorto, amount):
		return Color(Gloss.lerp2(colorfrom.r, colorto.r, amount), Gloss.lerp2(colorfrom.g, colorto.g, amount), Gloss.lerp2(colorfrom.b, colorto.b, amount), Gloss.lerp2(colorfrom.a, colorto.a, amount))

	@staticmethod
	def multi_lerp(values, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)
		if result == 1.0:
			return values[len(values) - 1]
		
		num_items = len(values)
		position = int(math.floor(amount * (num_items - 1))) # what was the minimum item we were up to?
		individual_lerp = 1.0 / (num_items - 1) # how much is each step?
		current_lerp = amount - position * individual_lerp # how far through the current step are we?
		lerp_amount = current_lerp / individual_lerp # how much is that compared to a full step
		
		return Color.lerp(values[position], values[position + 1], lerp_amount)

	@staticmethod
	def smooth_step(colorfrom, colorto, amount):
		return Color(Gloss.smooth_step(colorfrom.r, colorto.r, amount), Gloss.smooth_step(colorfrom.g, colorto.g, amount), Gloss.smooth_step(colorfrom.b, colorto.b, amount), Gloss.smooth_step(colorfrom.a, colorto.a, amount))

	@staticmethod
	def smooth_step2(colorfrom, colorto, amount):
		return Color(Gloss.smooth_step2(colorfrom.r, colorto.r, amount), Gloss.smooth_step2(colorfrom.g, colorto.g, amount), Gloss.smooth_step2(colorfrom.b, colorto.b, amount), Gloss.smooth_step2(colorfrom.a, colorto.a, amount))


class Point(object):
	@staticmethod
	def add(point1, point2):
		return (point1[0] + point2[0], point1[1] + point2[1])

	def bounce_both(point1, point2, amount, overshoot = 20):
		return (Gloss.bounce_both(point1[0], point2[0], amount, overshoot), Gloss.bounce_both(point1[1], point2[1], amount, overshoot))

	def bounce_in(point1, point2, amount, overshoot = 20):
		return (Gloss.bounce_in(point1[0], point2[0], amount, overshoot), Gloss.bounce_in(point1[1], point2[1], amount, overshoot))

	def bounce_out(point1, point2, amount, overshoot = 20):
		return (Gloss.bounce_out(point1[0], point2[0], amount, overshoot), Gloss.bounce_out(point1[1], point2[1], amount, overshoot))

	@staticmethod
	def distance(point1, point2):
		return math.sqrt(Point.distance_squared(point1, point2))

	@staticmethod
	def distance_squared(point1, point2):
		return (point1[0] - point2[0]) * (point1[0] - point2[0]) + (point1[1] - point2[1]) * (point1[1] - point2[1])

	@staticmethod
	def length(point):
		return math.sqrt(Point.length_squared(point))

	@staticmethod
	def length_squared(point):
		return (point[0] * point[0]) + (point[1] * point[1])

	@staticmethod
	def lerp(point1, point2, amount):
		return (Gloss.lerp(point1[0], point2[0], amount), Gloss.lerp(point1[1], point2[1], amount))

	@staticmethod
	def lerp2(point1, point2, amount):
		return (Gloss.lerp2(point1[0], point2[0], amount), Gloss.lerp2(point1[1], point2[1], amount))

	@staticmethod
	def multi_lerp(values, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)
		if result == 1.0:
			return values[len(values) - 1]
		
		num_items = len(values)
		position = int(math.floor(amount * (num_items - 1))) # what was the minimum item we were up to?
		individual_lerp = 1.0 / (num_items - 1) # how much is each step?
		current_lerp = amount - position * individual_lerp # how far through the current step are we?
		lerp_amount = current_lerp / individual_lerp # how much is that compared to a full step
		
		return Point.lerp(values[position], values[position + 1], lerp_amount)

	@staticmethod
	def multiply(point1, to):
		if to.__class__.__name__ == "tuple":
			return (point1[0] * to[0], point1[1] * to[1])
		else:
			return (point1[0] * to, point1[1] * to)

	@staticmethod
	def normalize(point):
		length = self.length()
		factor = 1.0 / length
		return (point[0] * factor, point[1] * factor)

	@staticmethod
	def smooth_step(point1, point2, amount):
		return (Gloss.smooth_step(point1[0], point2[0], amount), Gloss.smooth_step(point1[1], point2[1], amount))

	@staticmethod
	def smooth_step2(point1, point2, amount):
		return (Gloss.smooth_step2(point1[0], point2[0], amount), Gloss.smooth_step2(point1[1], point2[1], amount))

	@staticmethod
	def subtract(point1, point2):
		return (point1[0] - point2[0], point1[1] - point2[1])


class Gloss(object):
 	VERSION = 0.9

	PI_OVER_2 = math.pi / 2.0
	PI_OVER_4 = math.pi / 4.0
	TWO_PI = math.pi * 2.0

	Color.WHITE = Color(1, 1, 1, 1)
	Color.RED = Color(1, 0, 0, 1)
	Color.GREEN = Color(0, 1, 0, 1)
	Color.BLUE = Color(0, 0, 1, 1)
	Color.BLACK = Color(0, 0, 0, 1)
	Color.CORNFLOWER_BLUE = Color(0.392156863, 0.584313725, 0.929411765, 1) # used for clearing the screen to a safe value
	Color.PURPLE = Color(0.5, 0, 0.5, 1) # used for clearing rendertargets to a safe value
	Color.TRANSPARENT_WHITE = Color(1, 1, 1, 0)

	Point.ZERO = (0, 0)

	picking = False
	enable_texturing = True
	running_slowly = False
	
	total_seconds = 0

	sprite_click_tracking = False # set to True when any sprite has an OnClick method attached
	last_clicked_sprite = None # the sprite that was clicked most recently

	joysticks = []
	auto_particle_systems = []
	sprites = []

	@staticmethod
	def bounce_both(value1, value2, amount, overshoot = 20):
		overshoot *= 0.25949
	
		value2 -= value1
		amount /= 0.5
	
		if (amount < 1):
			return value2 / 2 * (amount * amount * ((overshoot + 1) * amount - overshoot)) + value1
		else:
			amount -= 2
			return value2 / 2 * (amount * amount * ((overshoot + 1) * amount + overshoot) + 2) + value1

	@staticmethod
	def bounce_in(value1, value2, amount, overshoot = 20):
		overshoot *= 0.170158
		value2 -= value1

		return value2 * amount * amount * ((overshoot + 1) * amount - overshoot) + value1;
		
	@staticmethod
 	def bounce_out(value1, value2, amount, overshoot = 20):
		overshoot *= 0.170158
		amount = amount / 1.0 - 1
		value2 -= value1
        	return value2 * (amount * amount * ((overshoot + 1) * amount + overshoot) + 1) + value1;
		
	@staticmethod
	def draw_box(position = (0, 0), width = 128, height = 128, rotation = 0.0, origin = (0, 0), scale = 1, color = Color.WHITE):
		glPushMatrix()
			
		glColor4f(color.r, color.g, color.b, color.a)
		glDisable(GL_TEXTURE_2D)
		
		boxwidth = width * scale
		boxheight = height * scale

		if origin is None:
			originx = boxwidth / 2
			originy = boxheight / 2
		else:
			originx = origin[0] * scale
			originy = origin[1] * scale
		
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		glBegin(GL_QUADS)
		glVertex2f(-originx, boxheight - originy)
		glVertex2f(boxwidth - originx, boxheight - originy)
		glVertex2f(boxwidth - originx, -originy)
		glVertex2f(-originx, -originy)

		glEnd()
		
		glEnable(GL_TEXTURE_2D)
		glPopMatrix()
	
	@staticmethod
	def draw_line(start, finish, color = Color.WHITE, width = 1.0):
		glPushMatrix()
		glColor4f(color.r, color.g, color.b, color.a)
		glLineWidth(width)
		
		glDisable(GL_TEXTURE_2D)
		
		glBegin(GL_LINES)
		glVertex2f(start[0], start[1])
		glVertex2f(finish[0], finish[1])
		glEnd()
		
		glEnable(GL_TEXTURE_2D)
		glPopMatrix()

	@staticmethod
	def draw_lines(lines, color = Color.WHITE, width = 1.0, join = False):
		glPushMatrix()
		glColor4f(color.r, color.g, color.b, color.a)
		glLineWidth(width)
		
		glDisable(GL_TEXTURE_2D)

		if join:
			glBegin(GL_LINE_LOOP)
		else:
			glBegin(GL_LINE_STRIP)			
		
		for line in lines:
			glVertex2f(line[0], line[1])
			
		glEnd()
		
		glEnable(GL_TEXTURE_2D)
		glPopMatrix()
		
	@staticmethod
	def draw_triangle(points = [(0, 0), (-50, 100), (50, 100)], position = (0,0), rotation = 0.0, origin = (0, 0), scale = 1, color = Color.WHITE):
		glPushMatrix()
			
		glColor4f(color.r, color.g, color.b, color.a)
		glDisable(GL_TEXTURE_2D)
		glDisable(GL_CULL_FACE) # don't give people the hassle of thinking about clockwise vs anti-clockwise
		
		if origin is None:
			x1 = points[0][0]
			x2 = points[1][0]
			x3 = points[2][0]
			y1 = points[0][1]
			y2 = points[1][1]
			y3 = points[2][1]

			originx = (x1 + x2 + x3) / 3.0
			originy = (y1 + y2 + y3) / 3.0
		else:
			originx = origin[0]
			originy = origin[1]
		
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		glBegin(GL_TRIANGLES)
		glVertex2f((points[0][0] - originx) * scale, (points[0][1] - originy) * scale)
		glVertex2f((points[1][0] - originx) * scale, (points[1][1] - originy) * scale)
		glVertex2f((points[2][0] - originx) * scale, (points[2][1] - originy) * scale)
		glEnd()
		
		glEnable(GL_TEXTURE_2D)
		glEnable(GL_CULL_FACE)
		glPopMatrix()

	@staticmethod
	def fill(texture = None, color = Color.WHITE, top = Color.CORNFLOWER_BLUE, bottom = Color.CORNFLOWER_BLUE, vertical = True):
		# never draw backgrounds when picking
		if Gloss.picking:
			return

		if texture is not None:
			width = Gloss.viewport_size[0]
			height = Gloss.viewport_size[1]

			glPushMatrix()

			glColor4f(color.r, color.g, color.b, color.a)
			glBindTexture(GL_TEXTURE_2D, texture.surface)

			glBegin(GL_TRIANGLE_STRIP)
			glTexCoord2f(0, texture.height_ratio); glVertex2f(0, height)
			glTexCoord2f(texture.width_ratio, texture.height_ratio); glVertex2f(width, height)
			glTexCoord2f(0, 1); glVertex2f(0, 0)
			glTexCoord2f(texture.width_ratio, 1); glVertex2f(width, 0)
			glEnd()
			
			glPopMatrix()
		else:
			# if we're still here, draw the polygon without texture using gradient colors
			if Gloss.enable_texturing:
				glDisable(GL_TEXTURE_2D)

			width = Gloss.viewport_size[0]
			height = Gloss.viewport_size[1]

			glPushMatrix()

			if vertical:
				glBegin(GL_TRIANGLE_STRIP)
				glColor4f(bottom.r, bottom.g, bottom.b, bottom.a); glVertex2f(0, height)
				glColor4f(bottom.r, bottom.g, bottom.b, bottom.a); glVertex2f(width, height)
				glColor4f(top.r, top.g, top.b, top.a); glVertex2f(0, 0)
				glColor4f(top.r, top.g, top.b, top.a); glVertex2f(width, 0)
				glEnd()
			else:
				glBegin(GL_TRIANGLE_STRIP)
				glColor4f(top.r, top.g, top.b, top.a); glVertex2f(0, height)
				glColor4f(bottom.r, bottom.g, bottom.b, bottom.a); glVertex2f(width, height)
				glColor4f(top.r, top.g, top.b, top.a); glVertex2f(0, 0)
				glColor4f(bottom.r, bottom.g, bottom.b, bottom.a); glVertex2f(width, 0)
				glEnd()

			if Gloss.enable_texturing:
				glEnable(GL_TEXTURE_2D)
				
			glPopMatrix()

	@staticmethod
	def clamp(value, minval, maxval):
		if value > maxval:
			value = maxval
		elif value < minval:
			value = minval

		return value

	@staticmethod
	def clear(color = Color.CORNFLOWER_BLUE):
		glClearColor(color.r, color.g, color.b, color.a)
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

	@staticmethod
	def hermite(value1, tangent1, value2, tangent2, amount):
		AmountSquared = amount * amount
		AmountCubed = AmountSquared * amount


		if amount is 0.0:
			result = value1
		elif amount is 1.0:
			result = value2
		else:
			result = (2 * value1 - 2 * value2 + tangent2 + tangent1) * AmountCubed + (3 * value2 - 3 * value1 - 2 * tangent1 - tangent2) * AmountSquared + tangent1 * amount + value1

		return result

	@staticmethod
	def lerp(value1, value2, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)
		return value1 + (value2 - value1) * result

	@staticmethod
	def lerp2(value1, value2, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)

		if result > 0.5:
			return value2 + (value1 - value2) * ((result - 0.5) * 2)
		else:
			return value1 + (value2 - value1) * (result * 2)
			
	@staticmethod
	def multi_lerp(values, amount):
		# this method lets people lerp through multiple numbers, eg (0, 1, 2) with 
		# lerp amount 0.5 would return 1. It works by figuring out where in the values list
		# we're up to, seeing how far we've moved towards the next position.
		
		result = Gloss.clamp(amount, 0.0, 1.0)
		if result == 1.0:
			return values[len(values) - 1]
		
		num_items = len(values)
		position = int(math.floor(amount * (num_items - 1))) # what was the minimum item we were up to?
		individual_lerp = 1.0 / (num_items - 1) # how much is each step?
		current_lerp = amount - position * individual_lerp # how far through the current step are we?
		lerp_amount = current_lerp / individual_lerp # how much is that compared to a full step
		
		return Gloss.lerp(values[position], values[position + 1], lerp_amount)

	@staticmethod
	def next_po2(value):
		logbase2 = (math.log(value) / math.log(2))
		return int(round(pow(2, math.ceil(logbase2))))

	@staticmethod
	def smooth_step(value1, value2, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)
		result = Gloss.hermite(value1, 0.0, value2, 0.0, result)
		return result

	@staticmethod
	def smooth_step2(value1, value2, amount):
		result = Gloss.clamp(amount, 0.0, 1.0)

		if result > 0.5:
			result = Gloss.hermite(value2, 0.0, value1, 0.0, (result - 0.5) * 2)
		else:
			result = Gloss.hermite(value1, 0.0, value2, 0.0, result * 2)

		return result

	@staticmethod
	def rand_float(value1, value2):
		return random.random() * (value2 - value1) + value1


	@staticmethod
	def save_screenshot(filename):
		if Gloss.redraw_needed:
			Gloss.active_game.gloss_internal_draw()

		glFinish()
		glPixelStorei(GL_PACK_ALIGNMENT, 4)
		glPixelStorei(GL_PACK_ROW_LENGTH, 0)
		glPixelStorei(GL_PACK_SKIP_ROWS, 0)
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0)

		data = glReadPixels(0, 0, Gloss.viewport_size[0], Gloss.viewport_size[1], GL_RGBA, GL_UNSIGNED_BYTE)

		surface = pygame.image.fromstring(data, (Gloss.viewport_size[0], Gloss.viewport_size[1]), 'RGBA', 1)
		pygame.image.save(surface, filename)

	@staticmethod
	def set_scene_tint(color):
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ((color.r, color.g, color.b, color.a)) )

	@staticmethod
	def to_radians(degrees):
		return degrees * 0.017453292519943295769236907685

	@staticmethod
	def enable_picking():
		Gloss.picking = True
		glDisable(GL_LIGHTING)
		glDisable(GL_TEXTURE_2D)
		Gloss.enable_texturing = False

	@staticmethod
	def disable_picking():
		Gloss.picking = False
		glEnable(GL_LIGHTING)
		glEnable(GL_TEXTURE_2D)
		Gloss.enable_texturing = True
	
	@staticmethod
	def select_object(pos):
		Gloss.enable_picking()
		Gloss.active_game.gloss_internal_draw()

		pixels = glReadPixelsub(pos[0], Gloss.viewport_size[1] - pos[1], 1, 1, GL_RGBA)

		r = pixels[0][0][0]
		g = pixels[0][0][1]
		b = pixels[0][0][2]

		Gloss.disable_picking()

		for sprite in Gloss.sprites:
			if (sprite.pick_color[0] == r) and sprite.pick_color[1] == g and sprite.pick_color[2] == b:
				if sprite._on_click is not None:
					# a function has been attached here - call it!
					sprite._on_click(sprite)

				Gloss.last_clicked_sprite = sprite

				return sprite

		return None


class Texture(object):
	def __init__(self, source):
		self.surface = None

		if (source.__class__.__name__ == "str") or (source.__class__.__name__ == "unicode"):
			try:
				surface = pygame.image.load(source)
			except:
				print("Unable to load texture " + source + ".")
				sys.exit(1)
		else:
			surface = source
 
		width = surface.get_width()
		height = surface.get_height()

		self.width = width
		self.height = height
		self.half_width = self.width / 2
		self.half_height = self.height / 2

		po2width = Gloss.next_po2(width)
		po2height = Gloss.next_po2(height)

		if (po2width > Gloss.MaxTextureSize or po2height > Gloss.MaxTextureSize):
			print "Fatal error: texture at " + path + " is bigger than the maximum supported texture size of " + str(Gloss.MaxTextureSize)
			sys.exit(1)

		self.width_ratio = (self.width / po2width)
		self.height_ratio = 1 - (self.height / po2height)

		if (width != po2width or height != po2height):
			tmpsurface = pygame.Surface((po2width, po2height), SRCALPHA, 32)
			tmpsurface.blit(surface, (0,0))
			surface = tmpsurface

		data = pygame.image.tostring(surface, "RGBA", 1)
	
		self.surface = glGenTextures(1)
		glBindTexture(GL_TEXTURE_2D, self.surface)
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, po2width, po2height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)

	def __del__(self):
		if self.surface is not None:
			# before we try freeing this memory, be careful: Python may have unloaded the OpenGL module by now, in which case just bail out
			if glDeleteTextures is not None:
				glDeleteTextures(self.surface)
				self.surface = None

	def draw(self, position = (0, 0), rotation = 0.0, origin = (0, 0), scale = 1, color = Color.WHITE):
		# never draw textures if picking
		if Gloss.picking:
			return

		texwidth = self.width * scale
		texheight = self.height * scale

		if origin is None:
			originx = texwidth / 2
			originy = texheight / 2
		else:
			originx = origin[0] * scale
			originy = origin[1] * scale

		glPushMatrix()
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		glColor4f(color.r, color.g, color.b, color.a)
	
		glBindTexture(GL_TEXTURE_2D, self.surface)

		glBegin(GL_TRIANGLE_STRIP)
		glTexCoord2f(0, self.height_ratio); glVertex2f(-originx, texheight - originy)
		glTexCoord2f(self.width_ratio, self.height_ratio); glVertex2f(texwidth - originx, texheight - originy)
		glTexCoord2f(0, 1); glVertex2f(-originx, -originy)
		glTexCoord2f(self.width_ratio, 1); glVertex2f(texwidth - originx, -originy)
		glEnd()
		
		glPopMatrix()

class Sprite(object):
	next_id = 1
	pick_r = 0
	pick_g = 0
	pick_b = 0

	def __init__(self, texture, position = None):
		self.id = str(Sprite.next_id)
		Sprite.next_id += 1

		self.texture = texture

		if position is None:
			self.position = (0, 0)
		else:
			self.position = position

		Sprite.pick_r += 1
		if Sprite.pick_r > 255:
			Sprite.pick_r = 0
			Sprite.pick_g += 1
			if Sprite.pick_g > 255:
				Sprite.pick_g = 0
				Sprite.pick_b += 1

		self.pick_color = (Sprite.pick_r, Sprite.pick_g, Sprite.pick_b, 255)

		self._on_click = None

		Gloss.sprites.append(weakref.ref(self))

	def __del__(self):
		# remove any dead weak references from the sprite list		
		for i in reversed(range(len(Gloss.sprites))):
			if Gloss.sprites[i]() is None:
				del Gloss.sprites[i]
				
	def get_on_click(self):
		return self._on_click

	def set_on_click(self, value):
		self._on_click = value
		Gloss.sprite_click_tracking = True

	on_click = property(get_on_click, set_on_click)


	def draw(self, position = None, rotation = 0.0, origin = (0, 0), scale = 1, color = Color.WHITE):
		texwidth = self.texture.width * scale
		texheight = self.texture.height * scale

		if origin is None:
			originx = texwidth / 2
			originy = texheight / 2
		else:
			originx = origin[0] * scale
			originy = origin[1] * scale

		if position is None:
			position = self.position

		glPushMatrix()
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		if Gloss.picking is False:
			glColor4f(color.r, color.g, color.b, color.a)
		else:
			glColor3ub(self.pick_color[0], self.pick_color[1], self.pick_color[2])

		glBindTexture(GL_TEXTURE_2D, self.texture.surface)

		glBegin(GL_TRIANGLE_STRIP)
		glTexCoord2f(0, self.texture.height_ratio); glVertex2f(-originx, texheight - originy)
		glTexCoord2f(self.texture.width_ratio, self.texture.height_ratio); glVertex2f(texwidth - originx, texheight - originy)
		glTexCoord2f(0, 1); glVertex2f(-originx, -originy)
		glTexCoord2f(self.texture.width_ratio, 1); glVertex2f(texwidth - originx, -originy)
		glEnd()
		
		glPopMatrix()

	def move(self, x, y):
		self.position = (self.position[0] + x, self.position[1] + y)

	def move_to(self, x = None, y = None):
		if x is None:
			if y is None:
				return

			self.position = (self.position[0], y)
			return
		
		if y is None:
			self.position = (x, self.position[1])
		else:
			self.position = (x, y)

class ParticleSystem(object):
	additive = False
	
	def __init__(self, texture, position = None, lifespan = -1, creationspeed = None, initialparticles = 50, particlelifespan = 1000, minspeed = 50, maxspeed = 250, minrotation = 0, maxrotation = 0, minscale = 1.0, maxscale = 1.0, growth = 0.0, wind = None, drag = None, startcolor = Color.WHITE, endcolor = Color.TRANSPARENT_WHITE, onfinish = None, name = ""):
		Gloss.auto_particle_systems.append(self)

		self.name = name
		self.on_finish = onfinish

		self.texture = texture

		if position is None:
			self.position = (0, 0)
		else:
			self.position = position

		self.created_time = Gloss.tick_count
		self.last_particle_created_time = Gloss.tick_count

		self.frozen = False # used to stop particle systems being automatically updated
		self.alive = True # set to be False when destroyed

		self.lifespan = lifespan
		self.creation_speed = creationspeed
		self.initial_particles = initialparticles
		self.particle_lifespan = particlelifespan

		self.start_color = startcolor
		self.end_color = endcolor
		self.particle_speed_min = minspeed
		self.particle_speed_max = maxspeed
		self.particle_rotation_min = minrotation
		self.particle_rotation_max = maxrotation
		self.particle_scale_min = minscale
		self.particle_scale_max = maxscale
		self.particle_growth = growth
		self.particle_drag = drag
		self.particle_wind = wind

		self.particles = []

		for i in range(initialparticles):
			self.create_particle()

	def __del__(self):
		self.particles = [] # destroy all particles now

	def draw(self):
		if self.alive is False:
			return
			
		# never draw particle systems if picking
		if Gloss.picking:
			return

		if self.additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE)
			
		for particle in self.particles:
			self.texture.draw(position = particle.position, rotation = particle.rotation, origin = None, scale = particle.scale + (particle.anim_pos * self.particle_growth), color = particle.color)

		if self.additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

	def update(self):
		# kill off old particle systems
		if (self.lifespan == -1):
			# long-term particle systems also need to be auto-removed if they haven't done anything for a little while
			if len(self.particles) == 0:
				return False
		else:
			# if we have finished our life and all our particles are dead, return false so that the game can remove us from the list of particle systems
			if self.created_time + self.lifespan < Gloss.tick_count:
				return False


		# do we need to create a new particle?
		if (self.creation_speed is not None and self.last_particle_created_time + self.creation_speed < Gloss.tick_count):
			self.create_particle()
		
		#cache the current wind and drag amounts to save calculation in the particle loop	
		if self.particle_wind is not None:
			current_wind = Point.multiply(self.particle_wind, Gloss.elapsed_seconds)
			
		if self.particle_drag is not None:
			current_drag = Gloss.elapsed_seconds * self.particle_drag

		for i in reversed(range(len(self.particles))):
			particle = self.particles[i]
			
			if particle.created_time + self.particle_lifespan < Gloss.tick_count:
				del self.particles[i]

			particle.position = Point.add(particle.position, Point.multiply(particle.velocity, Gloss.elapsed_seconds))

			if self.particle_wind is not None:
				particle.position = Point.add(particle.position, current_wind)

			particle.anim_pos = Gloss.clamp((Gloss.tick_count - particle.created_time) / self.particle_lifespan, 0.0, 1.0)
			particle.color = Color.lerp(self.start_color, self.end_color, particle.anim_pos)

			if self.particle_drag is not None:
				particle.velocity = Point.subtract(particle.velocity, Point.multiply(particle.velocity, current_drag))

				if (Point.length_squared(particle.velocity) <= 0):
					particle.velocity = Point.ZERO

		return True

	def create_particle(self):
		particle = Particle()

		particle.created_time = Gloss.tick_count
		particle.position = self.position
		particle.rotation = Gloss.rand_float(self.particle_rotation_min, self.particle_rotation_max)
		particle.scale = Gloss.rand_float(self.particle_scale_min, self.particle_scale_max)

		angle = random.random() * Gloss.TWO_PI
		particle.velocity = Point.multiply((math.cos(angle), math.sin(angle)), Gloss.rand_float(self.particle_speed_min, self.particle_speed_max))

		self.last_particle_created_time = Gloss.tick_count

		self.particles.append(particle)

class Particle:
	def __init__(self):
		self.anim_pos = 0.0
		self.color = Color.WHITE


class RenderTarget:
	def __init__(self, width = 512, height = 512):
		self.buffer = None
		self.surface = None
		self.width = width
		self.height = height
		self.half_width = width / 2.0
		self.half_height = height / 2.0

		# create the framebuffer
		temp = glGenFramebuffersEXT(1)
		self.buffer = temp
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, self.buffer)

		# create a texture for rendering
		temp = glGenTextures(1)
		self.surface = temp

		glActiveTexture(GL_TEXTURE0)
		glBindTexture(GL_TEXTURE_2D, self.surface)

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, None)

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, self.surface, 0)

		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)
		if status != GL_FRAMEBUFFER_COMPLETE_EXT:
			return

		Gloss.clear(Color.PURPLE)

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0) # we don't need this activated yet, so unbind it for now

	# start rendering to this framebuffer
	def activate(self):
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, self.buffer)
		Gloss.viewport_size = self.width, self.height
		glViewport(0, 0, self.width, self.height)

		glMatrixMode(GL_PROJECTION)
		glPushMatrix()
		glLoadIdentity()
		glOrtho(0, self.width, self.height, 0, -100, 100)
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glLoadIdentity()

	# stop rendering to this framebuffer
	def deactivate(self):
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0)
		Gloss.viewport_size = Gloss.screen_resolution
		glViewport(0, 0, Gloss.screen_resolution[0], Gloss.screen_resolution[1])

		glMatrixMode(GL_PROJECTION)
		glPopMatrix()
		glMatrixMode(GL_MODELVIEW)
		glPopMatrix()


	# destroy this framebuffer and free allocated resources.
	def __del__(self):
		glDeleteFramebuffersEXT(1, [self.buffer])
		glDeleteTextures(1, [self.surface])
		self.buffer = None
		self.surface = None

	# draw the contents of the buffer to the screen
	def draw(self, position, width = None, height = None, rotation = 0.0, origin = (0, 0), scale = 1, color = Color.WHITE):
		# always draw this texture, even when picking
		glEnable(GL_TEXTURE_2D)

		if width is None: width = self.width
		if height is None: height = self.height

		texheight = height * scale
		texwidth = width * scale

		if origin is None:
			originx = texwidth / 2
			originy = texheight / 2
		else:
			originx = origin[0] * scale
			originy = origin[1] * scale

		glPushMatrix()
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		if Gloss.picking is False:
			glColor4f(color.r, color.g, color.b, color.a)
		else:
			# when picking, use pure white so that all textures inside this are visible
			glColor4f(1, 1, 1, 1)

		glBindTexture(GL_TEXTURE_2D, self.surface)

		glBegin(GL_TRIANGLE_STRIP)
		glTexCoord2f(0, 0); glVertex2f(-originx, texheight - originy)
		glTexCoord2f(1, 0); glVertex2f(texwidth - originx, texheight - originy)
		glTexCoord2f(0, 1); glVertex2f(-originx, -originy)
		glTexCoord2f(1, 1); glVertex2f(texwidth - originx, -originy)
		glEnd()

		# disable texturing if we're picking
		if Gloss.enable_texturing is False:
			glDisable(GL_TEXTURE_2D)
			
		glPopMatrix()

class SpriteFont(object):
	def __init__(self, filename, size = 18, bold = False, underline = False, startcharacter = 32, endcharacter = 126):
	        self.font = pygame.font.Font(filename, size)
		self.font.set_underline(bold)
		self.font.set_bold(underline)

	        self.characters = {}

		for letter in range(startcharacter, endcharacter + 1):
		    self.characters[chr(letter)] = SpriteFontLetter(self.font, chr(letter))

		self.line_height = self.characters["A"].height

	def draw(self, text = "Hello, Gloss!", position = (0, 0), rotation = 0.0, scale = 1.0, color = Color.WHITE, letterspacing = 0, linespacing = 0):
		# don't draw text when picking
		if Gloss.picking:
			return

		currentwidth = 0 # used to reset to the start of the line when breaking

		if position is None:
			position = self.position

		letterspacing *= scale
		linespacing *= scale

		glPushMatrix()
		glTranslatef(position[0], position[1], 0)

		if rotation is not 0.0:
			glRotatef(rotation, 0, 0, 1)

		glColor4f(color.r, color.g, color.b, color.a)

		lines = text.splitlines()

		for line in lines:
			for letter in line:
				lettertexture = self.characters[letter]

				texheight = lettertexture.height * scale
				texwidth = lettertexture.width * scale

				glBindTexture(GL_TEXTURE_2D, lettertexture.surface)

				glBegin(GL_TRIANGLE_STRIP)
				glTexCoord2f(0, lettertexture.height_ratio); glVertex2f(0, texheight)
				glTexCoord2f(lettertexture.width_ratio, lettertexture.height_ratio); glVertex2f(texwidth, texheight)
				glTexCoord2f(0, 1); glVertex2f(0, 0)
				glTexCoord2f(lettertexture.width_ratio, 1); glVertex2f(texwidth, 0)
				glEnd()

				glTranslatef(texwidth + letterspacing, 0, 0)
				currentwidth += texwidth + letterspacing

			# carriage return + line feed
			glTranslatef(-currentwidth, texheight + linespacing, 0)
			currentwidth = 0
			
		glPopMatrix()

	def measure_string(self, text, scale = 1.0, letterspacing = 0, linespacing = 0):
		currentwidth = 0 # used to track the width of the current line
		maxwidth = 0 # used to track the width of the longest line
		maxheight = 0 # used to track the height of the longest line

		letterspacing *= scale
		linespacing *= scale

		lines = text.splitlines()

		for line in lines:
			for letter in line:
				lettertexture = self.characters[letter]
				texheight = lettertexture.height * scale
				texwidth = lettertexture.width * scale
				currentwidth += texwidth + letterspacing

			# carriage return + line feed
			if currentwidth > maxwidth:
				maxwidth = currentwidth

			currentwidth = 0

			maxheight += texheight + linespacing

		return maxwidth,maxheight

class SpriteFontLetter(object):
	def __init__(self, font, letter):
		surface = font.render(letter, True, (255,255,255))

		width = surface.get_width()
		height = surface.get_height()

		self.width = width
		self.height = height
		self.half_width = self.width / 2
		self.half_height = self.height / 2

		po2width = Gloss.next_po2(width)
		po2height = Gloss.next_po2(height)

		if (po2width > Gloss.MaxTextureSize or po2height > Gloss.MaxTextureSize):
			print "Fatal error: texture at " + path + " is bigger than the maximum supported texture size of " + str(Gloss.MaxTextureSize)
			sys.exit(1)

		self.width_ratio = (self.width / po2width)
		self.height_ratio = 1 - (self.height / po2height)

		if (width != po2width or height != po2height):
			tmpsurface = pygame.Surface((po2width, po2height), SRCALPHA, 32)
			tmpsurface.blit(surface, (0,0))
			surface = tmpsurface

		letter = pygame.image.tostring(surface, 'RGBA', 1)

		self.surface = glGenTextures(1)
		glBindTexture(GL_TEXTURE_2D, self.surface)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, po2width, po2height, 0, GL_RGBA, GL_UNSIGNED_BYTE, letter)
