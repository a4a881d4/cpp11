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

fn main() {
	let c = Circle { radius : 2f64 };
    println!("Area of Circle ( radius = {} ) : {}",c.radius,c.area());
    println!("Circumference of Circle ( radius = {} ) : {}",c.radius,c.circumference());
    let b = Box::new(Circle { radius: 4f64 }) as Box<Round>;
    println!("Area of Circle ( radius = {} ) : {}",b.get_radius(),b.area());
    println!("Circumference of Circle ( radius = {} ) : {}",b.get_radius(),b.circumference());
}
