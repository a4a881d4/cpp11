let rec last x = 
	match x with
	| [] -> None
	| [a] -> Some(a)
	| _ :: t -> last t in 
last [1;2;3];;

let rec last_two x = 
	match x with 
	| [] -> None
	| [a] -> None
	| [a;b] -> Some(a,b)
	| _ :: t -> last_two t

let rec at k x =
	match x with 
	| [] -> None
	| a :: t -> if k==0 then Some(a) else at (k-1) t;;

let rec length x =
	match x with
	| [] -> 0
	| _ :: t -> (length t) + 1;;

let rev x = 
	let rec aux m n = 
		match n with
		| [] -> m
		| a :: t -> aux (a::m) t in
	aux [] x;;

(* let rec is_palindrome = fuction
	| [] -> true
	| [a] -> true
	| a :: t :: [b] -> if a==b then true else is_palindrome t;; *)

let is_palindrome list = 
	list = rev list;;

type 'a node =
    | One of 'a 
    | Many of 'a node list;;

let compressA list =
	let rec aux acc = function
	| [] -> acc
	| a :: t -> match acc with 
				| [] -> aux [a] t
				| b :: s -> if a=b then aux acc t else aux (a::acc) t
	in rev(aux [] list);;

let rec compress = function
    | a :: (b :: _ as t) -> if a = b then compress t else a :: compress t
    | smaller -> smaller;;

compress ["a";"a";"a";"a";"b";"c";"c";"a";"a";"d";"e";"e";"e";"e"];;

let pack list =
	let rec aux current acc = function
	| [] -> acc
	| [x] -> (x::current)::acc
	| a::(b::_ as t) -> if a = b then aux (a::current) acc t else aux [] ((a::current)::acc) t
	in rev(aux [] [] list);;

pack ["a";"a";"a";"a";"b";"c";"c";"a";"a";"d";"e";"e";"e";"e"];;

let encode list =
	let pl = pack list in
	let ll a = (length a, List.hd a) in
	List.map ll pl;;
encode ["a";"a";"a";"a";"b";"c";"c";"a";"a";"d";"e";"e";"e";"e"];;

type 'a rle =
    | One of 'a
    | Many of int * 'a;;
let rencode list =
	List.map (fun (k,a) -> if k=1 then One a else Many (k,a)) (encode list);;
rencode ["a";"a";"a";"a";"b";"c";"c";"a";"a";"d";"e";"e";"e";"e"];;

let duplicate list =
	List.flatten (List.map (fun x -> [x;x]) list);;

let rec rep x k = if k=0 then [] else x :: (rep x (k-1));;	
let rec replicate list n = 
	match list with
	| [] -> []
	| x :: t -> List.append (rep x n) (replicate t n);;


drop ["a";"b";"c";"d";"e";"f";"g";"h";"i";"j"] 3;;
let drop list n = 
	let rec aux acc k = function
	| [] -> acc
	| x :: t -> if k<2 then List.append (rev acc) t else aux (x::acc) (k-1) t in
	aux [] n list;;

let split list n =
	let rec aux k acc = function
	| [] -> (rev acc,[])
	| x :: t as y -> if k=0 then (rev (acc),y) else aux (k-1) (x::acc) t
in aux n [] list;;

slice ["a";"b";"c";"d";"e";"f";"g";"h";"i";"j"] 2 6;;

let rec rtake n acc = function
	| [] -> acc
	| x :: t -> if n=0 then acc else rtake (n-1) (x::acc) t;;

let slice list m n =
	rtake (n-m+1) [] (rtake n [] list);;

rotate ["a"; "b"; "c"; "d"; "e"; "f"; "g"; "h"] 3;;

let rotate list n =
	let (x,y) = if n>=0 then split list n else split list ((List.length list)+n)
in List.append y x;;
remove_at 1 ["a";"b";"c";"d"];;

let rec remove_at n = function
| [] -> []
| x::t -> if n=0 then t else x :: remove_at (n-1) t;;

insert_at "alfa" 1 ["a";"b";"c";"d"];;

let rec insert_at s n = function
| [] -> []
| x::t -> if n=0 then s::(x::t) else x::insert_at s (n-1) t;;

range 4 9;;

let rec range m n =
	if m=n then [m] else if m>n then m :: range (m-1) n else m :: range (m+1) n;;

let rec lotto_select n m =
	if n=0 then [] else ((Random.int m)+1) :: lotto_select (n-1) m;;

extract 2 ["a";"b";"c";"d"];;

let rec extract n = function
	| [] -> []
	| x :: t -> if n=1 then List.map (fun a->[a]) (x::t) 
	else (extract n t) @ List.map (fun a -> x::a) (extract (n-1) t);;

group ["a";"b";"c";"d"] [2;1];;
let select_one list =
	let rec aux acc = function
	| [] -> []
	| t::x -> ([t],acc@x)::aux (t::acc) x 
in aux [] list;;
select_one ["a";"b";"c";"d"];;

let select_two list =
	let e1 = select_one list in
	List.flatten (List.map (fun (x,y) -> (List.map (fun ([a],b)-> (a::x,b)) (select_one y))) e1);;

let select_three list =
	let e1 = select_two list in
	List.flatten (List.map (fun (x,y) -> (List.map (fun ([a],b)-> (a::x,b)) (select_one y))) e1);;

select_two ["a";"b";"c";"d"];;

select_three ["a";"b";"c";"d"];;

let rec select_N n list =
	if n=1 then
		select_one list
	else
	let en = select_N (n-1) list in
	List.flatten (List.map (fun (x,y) -> (List.map (fun ([a],b)-> (a::x,b)) (select_one y))) en);;
select_N 3 ["a";"b";"c";"d"];;



let length_sort list = 
	List.sort (fun x y -> (length y)-(length x)) list;;

length_sort [ ["a";"b";"c"]; ["d";"e"]; ["f";"g";"h"]; ["d";"e"];
                ["i";"j";"k";"l"]; ["m";"n"]; ["o"] ];;

let is_prime n =
	if n<3 then true else 
	List.for_all (fun x -> not ((n mod x)=0) ) (range 2 (n-1));;

let rec gcd m n = 
	let p = m mod n in
	match p with
	| 0 -> n
	| 1 -> 1
	| _ -> gcd n p;;

let coprime m n =
	(gcd m n) = 1;;

let phi m = 
	List.fold_left (fun sum x-> if coprime m x then sum+1 else sum) 0 (range 1 m);;

let factors m =
	let rec aux d n =
		if d > n then [] else if (n mod d)=0 then d :: aux d (n/d) else aux (d+1) n
	in encode (aux 2 m);;

let phi_improved m =
	let rec pow x = function
	| 0 -> 1
	| n -> x*(pow x (n-1)) in
	let lm = factors m in
	List.fold_left (fun mm (n,p) -> mm*(p-1)*(pow p (n-1))) 1 lm;;
let timeit f a =
    let t0 = Unix.gettimeofday() in
    ignore(f a);
    let t1 = Unix.gettimeofday() in
    t1 -. t0;;

let all_prime m =
 	let rec aux = function
 	| [] -> []
 	| x::t -> x::aux (List.filter (fun y -> (y mod x)<>0) t)
 	in aux (range 2 m);;
