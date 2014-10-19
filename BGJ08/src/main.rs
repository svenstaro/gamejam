extern crate graphics;
extern crate piston;
extern crate sdl2_game_window;
extern crate opengl_graphics;
extern crate shader_version;
extern crate "nalgebra" as na;
extern crate "ncollide2df64" as nc;

use sdl2_game_window::WindowSDL2;
use opengl_graphics::Gl;
use shader_version::opengl::OpenGL_2_1;
use na::{Iso2,Vec2};
use nc::geom::Cuboid;

use piston::{
    Window,
    Render,
    RenderArgs,
    Update,
    UpdateArgs
};

use graphics::{
    Context,
    AddRectangle,
    AddColor,
    Draw,
    RelativeTransform2d,
};

use player::Player;
use enemy::Enemies;

mod player;
mod enemy;

pub struct App {
    gl: Gl,       // OpenGL drawing backend.
    player: Player,
    enemies: Enemies
}

impl App {
    fn render<W: Window>(&mut self, _: &mut W, args: &RenderArgs) {
        // Set up a context to draw into.
        let context = &Context::abs(args.width as f64, args.height as f64);

        // Clear the screen.
        context.rgba(0.1,0.1,0.1,1.0).draw(&mut self.gl);

        context
            .trans((args.width / 2) as f64, (args.height / 2) as f64)
            .rect(0.0, 0.0, 100.0, 50.0)
            .rgba(1.0, 0.0, 0.0,1.0)
            .trans(-50.0, -25.0)
            .draw(&mut self.gl);

        self.enemies.render(context, &mut self.gl)
    }

    fn update<W: Window>(&mut self, _: &mut W, args: &UpdateArgs) {
        self.enemies.update(args)
        //self.rotation += 2.0 * args.dt;
    }
}

fn main() {

    // Create an SDL window.
    let mut window = WindowSDL2::new(
        piston::shader_version::opengl::OpenGL_2_1,
        piston::WindowSettings::default()
    );

    // Some settings for how the game should be run.
    let event_settings = piston::EventSettings {
        updates_per_second: 60,
        max_frames_per_second: 60
    };

    // Create a new game and run it.
    let mut app = App { 
        gl: Gl::new(OpenGL_2_1),
        player: Player::new(Cuboid::new(Vec2::new(3.0, 1.0)), Vec2::new(0.0, 0.0)),
        enemies: Enemies::new(Cuboid::new(Vec2::new(3.0, 1.0)))
    };

    app.enemies.spawn_enemy(Vec2 {x: 15.0, y: 30.0});

    // TODO: Change this back to a for loop after rust is fixed.
    let mut event_iter = piston::EventIterator::new(&mut window, &event_settings);
    loop {
        let e = match event_iter.next() {
            Some(e) => e,
            None => { break; }
        };
        match e {
            Render(_args) => app.render(event_iter.window, &_args),
            Update(_args) => app.update(event_iter.window, &_args),
            _ => {  }
        }
    }

    /*
     * This is broken due to a bug in rustc.
     * For more information, please read:
     * https://github.com/PistonDevelopers/piston/issues/641

    for e in event_iter {
        match e {
            Render(_args) => app.render(event_iter.window, &_args),
            Update(_args) => app.update(event_iter.window, &_args),
            _ => {  }
        }
    }*/
}

