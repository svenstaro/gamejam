pub struct Component {
    name : String
}

pub struct Component2 {
    name : String
}

trait IsComponent {
    pub fn getName(&self) -> String;
}

impl IsComponent for Component {
    pub fn getName(&self) -> String {
        self.name
    }
}

impl IsComponent for Component2 {
    pub fn getName(&self) -> String {
        self.name
    }
}

