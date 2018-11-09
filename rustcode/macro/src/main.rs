
macro_rules! hashmap {
	($($key : expr => $val : expr), *) => {{
		let mut map = ::std::collections::HashMap::new();
		$( map.insert($key,$val); )*
		map
	}}
}

fn main() {
	let m = hashmap!['A'=>0,'B'=>1,'C'=>2];
    println!("Hello, world! {:?}",m);
    for (x,i) in m.iter() {
    	println!("{}->{}",x,i);
    }
}
