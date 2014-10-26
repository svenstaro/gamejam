extern crate "nalgebra" as na;
extern crate "ncollide2df64" as nc;
extern crate opengl_graphics;

use na::{Pnt2,Iso2,Vec2,Transform,Translation,Translate};
use nc::geom::Cuboid;
use nc::bounding_volume::AABB;
use nc::bounding_volume::HasAABB;
use nc::bounding_volume::HasBoundingVolume;
use nc::bounding_volume::BoundingVolume;

use opengl_graphics::Gl;

use piston::{
    RenderArgs,
    UpdateArgs
};

use graphics::{
    Context,
    Draw,
    AddRectangle,
    AddColor
};

pub struct Enemies {
    shape: Cuboid,
    positions: Vec<Iso2<f64>>
}

impl Enemies {
    pub fn new(shape: Cuboid) -> Enemies {
        Enemies {
            shape: shape,
            positions: Vec::new()
        }
    }

    pub fn spawn_enemy(&mut self, initial_position: Vec2<f64>) {
        self.positions.push(Iso2::new(initial_position, na::zero()))
    }

    pub fn render(&mut self, context: &Context, gl: &mut Gl) {
        for pos in self.positions.iter() {
            let size = self.shape.half_extents() * Vec2 {x: 2.0, y: 2.0};
            context
                .rect(pos.translation().x, pos.translation().y, size.x, size.y)
                .rgba(0.9, 0.9, 0.95, 1.0)
                .draw(gl);
        }
    }

    pub fn update(&mut self, args: &UpdateArgs) {
        for position in self.positions.iter_mut() {
            let new_position = position.translation().x * args.dt + 1.0;
            position.append_translation(&Vec2::new(new_position, 0.0));
        }
    }
}
