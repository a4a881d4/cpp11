trait Shape {
	fn area(&self) -> f64;
	fn circumference(&self) -> f64;
}

struct Circle {
	radius : f64,
}

impl Shape for Circle {
	fn area(&self) -> f64 {
		std::f64::consts::PI * self.radius * self.radius
	}
	fn circumference(&self) -> f64 {
		(std::f64::consts::PI * self.radius).double()
	}
}

trait Double {
	fn double(&self) -> Self;
}

impl Double for f64 {
	fn double(&self) -> f64 {
		*self * 2f64
	}
}

trait Round {
	fn get_radius(&self) -> f64;
}

impl Shape for Round {
	fn area(&self) -> f64 { std::f64::consts::PI * self.get_radius() * self.get_radius()}
	fn circumference(&self) -> f64 {
		(std::f64::consts::PI * self.get_radius()).double()
	}
}

impl Round for Circle {
	fn get_radius(&self) -> f64 { self.radius }
}
mod dir {
	pub enum Direction {
		East,West,South,North
	}
	pub fn print(x : &Direction) {
		match x {
			Direction::East => {
				println!(">");
			}
			Direction::North => {
				println!("^");
			}
			Direction::West => {
				println!("<");
			}
			Direction::South => {
				println!("_");
			}
		}
	}
}
fn main() {
	let c = Circle { radius : 2f64 };
    println!("Area of Circle ( radius = {} ) : {}",c.radius,c.area());
    println!("Circumference of Circle ( radius = {} ) : {}",c.radius,c.circumference());
    let b = Box::new(Circle { radius: 4f64 }) as Box<Round>;
    println!("Area of Circle ( radius = {} ) : {}",b.get_radius(),b.area());
    println!("Circumference of Circle ( radius = {} ) : {}",b.get_radius(),b.circumference());
    let v = [[0_i32;5];5];
    println!("vector of vector {:?}",v);
    for (i,x) in v.iter().enumerate() {
    	println!("vecor[{}] = {:?}",i,x);
    }
    // let v = [0_i32;9];
    for i in 0_i32..=10 {
    	println!("v[{}] = {:?}",i,v.get(i as usize));
    }
    use dir::Direction as Dir;

    let v :[Dir;5] = [
	      Dir::West
	    , Dir::South
	    , Dir::North
	    , Dir::East
	    , Dir::South
	    ];
	for i in v.iter() {
		dir::print(i);
	}
}
