extern crate "nalgebra" as na;
extern crate "ncollide2df64" as nc;

use na::{Iso2,Vec2};
use nc::geom::Cuboid;
use nc::bounding_volume::AABB;
use nc::bounding_volume::HasAABB;
use nc::bounding_volume::HasBoundingVolume;
use nc::bounding_volume::BoundingVolume;

pub struct Player {
    shape: Cuboid,
    position: Iso2<f64>
}

impl Player {
    pub fn new(shape: Cuboid, initial_position: Vec2<f64>) -> Player {
        Player {
            shape: shape,
            position: Iso2::new(initial_position, na::zero())
        }
    }
}

impl HasBoundingVolume<AABB> for Player {
    fn bounding_volume(&self) -> AABB {
        self.shape.aabb(&self.position)
    }
}
