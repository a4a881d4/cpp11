mod normal {
	#[derive(Debug)]
	struct Pair(u8,usize);

	pub fn msc(t:&String) -> usize {
		let vt = t.clone().into_bytes();
		let mut v : Vec<Pair> = Vec::new();
		svec(&mut v,&vt[..]);
		println!("{:?}",v);
		v.iter().fold(0,|m,ref x| if m>x.1 {m} else {x.1})
	}
	fn scount(x : &u8, xs :&[u8]) -> usize {
		xs.iter().filter(|y| **y > *x).count() as usize
	}
	fn svec(v : &mut Vec<Pair>, xs : &[u8]) {
		if let Some((x,ys)) = xs.split_first() {
			v.push(Pair(*x,scount(x,ys)));
			svec(v,ys);
		}
	}
	
}
fn main() {
    let table = String::from("generating");
    println!("msc({}) = {}",&table,normal::msc(&table));
}
