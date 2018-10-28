mod normal {
	pub fn msc(t:&String) -> usize {
		&*t.to_vec().len()
	}
}
fn main() {
    let table = String::from("generating");
    println!("msc({}) = {}",&table,normal::msc(&table));
}
