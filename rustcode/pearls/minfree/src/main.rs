/*
minfree :: [Integer] -> Integer
minfree xs = head([0..] \\ xs)

(\\) :: Eq a => [a] -> [a] -> [a]
us \\ vs = filter (notElem' vs) us

notElem' :: Eq a => [a] -> a -> Bool
notElem' a n = notElem n a
*/

mod normal{
	pub fn minfree(xs : &[usize]) -> usize {
		let n = xs.len();
		let mut t : Vec<bool> = vec![false; n+1];
		for x in xs {
			let ux = *x;
			if ux<=n {
				t[ux] = true;
			}
		}
		let mut i : usize = 0;
		let r = loop{
			if !t[i] {break i}
			i += 1;
			if i>n {break i}
		};
		r
	}
}

/*
minfree :: [Integer] -> Integer
minfree xs = minfrom 0 xs

minfrom :: Integer -> [Integer] -> Integer
minfrom a xs = head ([a ..] \\ xs)

(\\) :: Eq a => [a] -> [a] -> [a]
us \\ vs = filter (notElem' vs) us

notElem' :: Eq a => [a] -> a -> Bool
notElem' a n = notElem n a

minfree :: [Integer] -> Integer

minfree xs = minfrom 0 (toInteger (length xs), xs)

minfrom :: Integer -> (Integer, [Integer]) -> Integer

minfrom a (n, xs)   | n == 0        = a
                    | m == b - a    = minfrom b (n - m, vs)
                    | otherwise     = minfrom a (m, us)
                      where (us, vs)    = partition (< b) xs
                            b           = a + 1 + n `div` 2
                            m           = toInteger (length us)

*/
mod speed {
	pub fn minfree(xs : &[usize]) -> usize {
		let ixs = xs.iter().collect::<Vec<_>>();
		minfrom(0,xs.len(),&ixs)
	}

	fn minfrom(a : usize, n : usize, ixs :&Vec<&usize>) -> usize {
		if n == 0 || ixs.len()==0 { a } 
		else {
			let b = a+1+n/2;
			let us : Vec<&usize> = ixs.iter().filter(|x| ***x<b).map(|x| *x).collect();
			println!("us({},{},{}) = {:?}",a,b,n,us);
			let vs : Vec<&usize> = ixs.iter().filter(|x| ***x>=b).map(|x| *x).collect();
			println!("vs = {:?}",vs);
			let m = us.len();
			if m == b-a { minfrom(b,n-m,&vs) }
			else { minfrom(a,m,&us) }
		}
	}
}

fn main() {
    let a = [08, 23, 09, 00, 12, 11, 01, 10, 13, 07, 41, 04, 14, 21, 05, 17, 03, 19, 02, 06];
    let ref ra = &a;
    println!("{:?}",ra);
    println!("{:?}",ra.iter().filter(|x| **x<10).collect::<Vec<_>>());
    println!("{:?}",normal::minfree(&a));
    println!("{:?}",speed::minfree(&a));
}
