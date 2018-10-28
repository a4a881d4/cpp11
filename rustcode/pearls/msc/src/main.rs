mod normal {
	pub fn msc(t:&String) -> usize {
		let vt = t.clone().into_bytes();
		println!("{:?}",vt.iter().filter(|x| **x < 'g' as u8).count());
		println!("tail[u8] = {:?}",tail(vt[..]));
		1
	}
	fn tail(xs : &[u8]) -> &[u8] {
		match xs.len() {
			0 => [],
			1 => [],
			_ => xs[1..],
		}
	}
}
fn main() {
    let table = String::from("generating");
    println!("msc({}) = {}",&table,normal::msc(&table));
}
