fn add1(t:(i32,i32)) -> i32 {
	t.0 + t.1
}

fn add2(x :i32,y:i32) -> i32 {
	x + y
}

fn add3((x,y):(i32,i32)) -> i32 {
	x + y
}

fn max(x :i32,y:i32) -> i32 {
	if x > y {x} else {y}
}

fn min(x :i64,y : i64) -> i64 {
	let a = if x > y {y} else {x};
	a
}

fn factor(x:i64) -> i64 {
	if x == 1 {
		1
	} else {
		x * factor(x-1)
	}
}

fn t_range(n : i32) {
	let x = 0..n;
	for i in x {
		print!("{}\t",i);
	}
	println!("");
}

fn t_string(sin : &str) {
	let mut s = String::from(sin);
	s.push(' ');
	s.push_str("World.");
	println!("{}",s);
}

fn main() {
	println!("1 + 2 = {} {} {}",add1((1,2)), add2(1,2), add3((1,2)));
	println!("max(1,2) = {}",max(1,2));
	let fnmin = min;
	println!("min(1,2) = {}",fnmin(1,2));
	println!("5! = {}",factor(5));
	for arg in std::env::args() {
		println!("Arg: {}",arg);
	}
	let v : [i32; 5] = [1,2,3,4,5];
	println!("max({},{}) = {}", v[0],v[1],max(v[0],v[1]));
	t_range(5);
	for i in 0..5 {
		let s = &v[i..];
		println!("{:?}",s);
	}
	t_string("Hello");

}