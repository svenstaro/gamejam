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

impl HasBoundingVolume<AABB> for Player {
    pub fn bounding_volume(&self) -> AABB {
        self.shape.aabb(&self.position)
    }
}
