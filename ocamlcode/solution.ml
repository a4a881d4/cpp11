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
