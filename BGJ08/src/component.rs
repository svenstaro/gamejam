pub struct Component {
    name : String
}

pub struct Component2 {
    name : String
}

pub trait IsComponent {
    fn get_name(self) -> String;
}

impl IsComponent for Component {
    fn get_name(self) -> String {
        self.name
    }
}

impl IsComponent for Component2 {
    fn get_name(self) -> String {
        self.name
    }
}

