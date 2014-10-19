extern crate "nalgebra" as na;
extern crate "ncollide2df64" as nc;

use na::{Iso2,Vec2};
use nc::geom::Cuboid;
use nc::bounding_volume::AABB;
use nc::bounding_volume::HasAABB;
use nc::bounding_volume::HasBoundingVolume;
use nc::bounding_volume::BoundingVolume;

use piston::{
    RenderArgs,
    UpdateArgs
};

use graphics::{
    Context,
    AddRectangle,
    AddColor,
};

pub struct Enemies {
    shape: Cuboid,
    positions: Vec<Iso2<f64>>
}

impl Enemies {
    pub fn spawn_enemy(&self, initial_position: Vec2<f64>) {
        self.positions.push(Iso2::new(initial_position, na::zero()))
    }

    pub fn render(&mut self, &mut context: &Context) {
        //for enemy in self.positions
    }

    pub fn update(&mut self, args: &UpdateArgs) {
    }
}

impl HasBoundingVolume<AABB> for Enemies {
    fn bounding_volume(&self, i: uint) -> AABB {
        self.shape.aabb(&self.positions[i])
    }
}
