use std::collections::HashMap;
use std::vec;
use component::{
    Component,
    Component2,
    IsComponent,
};

#[deriving(Hash, Eq, PartialEq)]
enum ComponentName {
    component,
    component2
}

pub struct Entity {
    components: HashMap<ComponentName, Box<IsComponent+'static>>,
    id : int
}

impl Entity {
    pub fn new(id: int) -> Entity{
        Entity { components: HashMap::new(), id: id }
    }

    pub fn add (&mut self, name: ComponentName, cmpt: Box<IsComponent+'static>) {
        self.components.insert(name, cmpt);
    }
}


