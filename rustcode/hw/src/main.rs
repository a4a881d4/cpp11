
fn main() {
    let an_integer = 1u32;
    let a_boolean = true;
    let unit = ();

    // copy `an_integer` into `copied_integer`
    let copied_integer = an_integer;

    println!("An integer: {:?}", copied_integer);
    println!("A boolean: {:?}", a_boolean);
    println!("Meet the unit value: {:?}", unit);

    // The compiler warns about unused variable bindings; these warnings can
    // be silenced by prefixing the variable name with an underscore
    let _unused_variable = 3u32;

    let _noisy_unused_variable = 2u32;
    // FIXME ^ Prefix with an underscore to suppress the warning

    let _immutable_binding = 1;
    let mut mutable_binding = 1;

    println!("Before mutation: {}", mutable_binding);

    // Ok
    mutable_binding += 1;

    println!("After mutation: {}", mutable_binding);

        // Declare a variable binding
    let a_binding;

    {
        let x = 2;

        // Initialize the binding
        a_binding = x * x;
    }

    println!("a binding: {}", a_binding);

    let another_binding;

    // Error! Use of uninitialized binding
    // println!("another binding: {}", another_binding);
    // FIXME ^ Comment out this line

    another_binding = 1;

    println!("another binding: {}", another_binding);
    let decimal = 65.4321_f32;

    // Error! No implicit conversion
    // let integer: u8 = decimal;
    // FIXME ^ Comment out this line

    // Explicit conversion
    let integer = decimal as u8;
    let character = integer as char;

    println!("Casting: {} -> {} -> {}", decimal, integer, character);

    // when casting any value to an unsigned type, T,
    // std::T::MAX + 1 is added or subtracted until the value
    // fits into the new type

    // 1000 already fits in a u16
    println!("1000 as a u16 is: {}", 1000 as u16);

    // 1000 - 256 - 256 - 256 = 232
    // Under the hood, the first 8 least significant bits (LSB) are kept,
    // while the rest towards the most significant bit (MSB) get truncated.
    println!("1000 as a u8 is : {}", 1000 as u8);
    // -1 + 256 = 255
    println!("  -1 as a u8 is : {}", (-1i8) as u8);

    // For positive numbers, this is the same as the modulus
    println!("1000 mod 256 is : {}", 1000 % 256);

    // When casting to a signed type, the (bitwise) result is the same as
    // first casting to the corresponding unsigned type. If the most significant
    // bit of that value is 1, then the value is negative.

    // Unless it already fits, of course.
    println!(" 128 as a i16 is: {}", 128 as i16);
    // 128 as u8 -> 128, whose two's complement in eight bits is:
    println!(" 128 as a i8 is : {}", 128 as i8);

    // repeating the example above
    // 1000 as u8 -> 232
    println!("1000 as a u8 is : {}", 1000 as u8);
    // and the two's complement of 232 is -24
    println!(" 232 as a i8 is : {}", 232 as i8);

    // Suffixed literals, their types are known at initialization
    let x = 1u8;
    let y = 2u32;
    let z = 3f32;

    // Unsuffixed literal, their types depend on how they are used
    let i = 1;
    let f = 1.0;

    // `size_of_val` returns the size of a variable in bytes
    println!("size of `x` in bytes: {}", std::mem::size_of_val(&x));
    println!("size of `y` in bytes: {}", std::mem::size_of_val(&y));
    println!("size of `z` in bytes: {}", std::mem::size_of_val(&z));
    println!("size of `i` in bytes: {}", std::mem::size_of_val(&i));
    println!("size of `f` in bytes: {}", std::mem::size_of_val(&f));

    // Because of the annotation, the compiler knows that `elem` has type u8.
    let elem = 5u8;

    // Create an empty vector (a growable array).
    let mut vec = Vec::new();
    // At this point the compiler doesn't know the exact type of `vec`, it
    // just knows that it's a vector of something (`Vec<_>`).

    // Insert `elem` in the vector.
    vec.push(elem);
    // Aha! Now the compiler knows that `vec` is a vector of `u8`s (`Vec<u8>`)
    // TODO ^ Try commenting out the `vec.push(elem)` line

    println!("{:?}", vec);

// `NanoSecond` is a new name for `u64`.
type NanoSecond = u64;
type Inch = u64;

// Use an attribute to silence warning.
#[allow(non_camel_case_types)]
type u64_t = u64;
// TODO ^ Try removing the attribute

    // `NanoSecond` = `Inch` = `u64_t` = `u64`.
    let nanoseconds: NanoSecond = 5 as u64_t;
    let inches: Inch = 2 as u64_t;

    // Note that type aliases *don't* provide any extra type safety, because
    // aliases are *not* new types
    println!("{} nanoseconds + {} inches = {} unit?",
             nanoseconds,
             inches,
             nanoseconds + inches);
    test0();

    let parsed: i32 = "5".parse().unwrap();
    let turbo_parsed = "10".parse::<i32>().unwrap();

    let sum = parsed + turbo_parsed;
    println!{"Sum: {:?}", sum};

    test1();
    {
        let x = 5u32;

	    let y = {
	        let x_squared = x * x;
	        let x_cube = x_squared * x;

	        // This expression will be assigned to `y`
	        x_cube + x_squared + x
	    };

	    let z = {
	        // The semicolon suppresses this expression and `()` is assigned to `z`
	        2 * x;
	    };

	    println!("x is {:?}", x);
	    println!("y is {:?}", y);
	    println!("z is {:?}", z);
    }
    for k in -10..15 {
    	test2(k);
    }
    test3();

    println!("test4 return 4 : {}",test4());

    test5();

    test6();

    for k in 0..20 {
    	test7(k);
    }
    
    for k in [(0,-2),(-2,0),(1,1),(0,0)].iter() {
    	test8(k);
    }

    test9();
    testa();
    testb();
    testc();
    testd();
    teste();
    testf();

    test00_0f();
}

use std::convert::From;

#[derive(Debug)]
struct Number {
    value: i32,
}

impl From<i32> for Number {
    fn from(item: i32) -> Self {
        Number { value: item }
    }
}

fn test0() {
    let num = Number::from(30);
    println!("My number is {:?}", num);

    let num1 : Number = 5.into();
    println!("My number is {:?}", num1);
}

use std::string::ToString;

struct Circle {
    radius: i32
}

impl ToString for Circle {
    fn to_string(&self) -> String {
        format!("Circle of radius {:?}", self.radius)
    }
}

fn test1() {
    let circle = Circle { radius: 6 };
    println!("{}", circle.to_string());
}

fn test2(n : i32) {

	if n < 0 {
	    print!("{} is negative", n);
	} else if n > 0 {
	    print!("{} is positive", n);
	} else {
	    print!("{} is zero", n);
	}

	let big_n =
	    if n < 10 && n > -10 {
	        println!(", and is a small number, increase ten-fold");

	        // This expression returns an `i32`.
	        10 * n
	    } else {
	        println!(", and is a big number, half the number");

	        // This expression must return an `i32` as well.
	        n / 2
	        // TODO ^ Try suppressing this expression with a semicolon.
	    };
	//   ^ Don't forget to put a semicolon here! All `let` bindings need it.

	println!("{} -> {}", n, big_n);

}

fn test3() {
    'outer: loop {
        println!("Entered the outer loop");

        'inner: loop {
            println!("Entered the inner loop");

            // 这只是中断内部的循环
            //break;

            // 这会中断外层循环
            break 'outer;
        }

        println!("This point will never be reached");
    }

    println!("Exited the outer loop");
}

fn test4() -> i32 {
    let mut counter = 0;

    let result = loop {
        counter += 1;

        if counter == 10 {
            break counter * 2;
        }
    };

    result
}

fn test5() {
    // 计数器变量
    let mut n = 1;

    // 当 `n` 小于 101 时进入循环操作
    while n < 101 {
        if n % 15 == 0 {
            println!("fizzbuzz");
        } else if n % 3 == 0 {
            println!("fizz");
        } else if n % 5 == 0 {
            println!("buzz");
        } else {
            println!("{}", n);
        }

        // 计数器值加1
        n += 1;
    }
}

fn test6() {
	for n in 1..101 {
        if n % 15 == 0 {
            println!("fizzbuzz");
        } else if n % 3 == 0 {
            println!("fizz");
        } else if n % 5 == 0 {
            println!("buzz");
        } else {
            println!("{}", n);
        }
    }
}
fn test7(number : i32) {
    print!("number {} is ", number);
    match number {
        // 匹配单个值
        1 => println!("One!"),
        // 匹配多个值
        2 | 3 | 5 | 7 | 11 => println!("This is a prime"),
        // 匹配一个闭区间范围
        13...19 => println!("A teen"),
        // 处理其他情况
        _ => println!("Ain't special"),
    }

    let boolean = if number%2 == 0 {true} else {false};
    // match 也是一个表达式
    let binary = match boolean {
        // match 分支必须覆盖所有可能的值
        false => 0,
        true => 1,
        // 试一试 ^ 试着将其中一条分支注释掉
    };

    println!("{} -> {}", boolean, binary);	

    match number {
        0             => println!("I'm not born yet I guess"),
        // 不能直接 `匹配（match）` 1 ... 12，但是孩子是几岁呢？
        // 相反，将 1 ... 12 序列绑定到 `n` 。现在年龄就可以读取了。
        n @ 1  ... 12 => println!("I'm a child of age {:?}", n),
        n @ 13 ... 18 => println!("I'm a teen of age {:?}", n),
        // 没有绑定。返回结果。
        n             => println!("I'm an old person of age {:?}", n),
    }
}

fn test8(pair : &(i32,i32)) {
    
    println!("Tell me about {:?}", pair);
    // match 可以解构一个元组
    match pair {
        // 绑定到第二个元素
        (0, y) => println!("First is `0` and `y` is `{:?}`", y),
        (x, 0) => println!("`x` is `{:?}` and last is `0`", x),
        _      => println!("It doesn't matter what they are"),
        // `_` 表示不将值绑定到变量
    }
    match pair {
        (x, y) if x == y => println!("These are twins"),
        // ^ `if condition`(if 条件)部分是一个守卫
        (x, y) if x + y == 0 => println!("Antimatter, kaboom!"),
        (x, _) if x % 2 == 1 => println!("The first one is odd"),
        _ => println!("No correlation..."),
    }	
}
fn test9() {
    // 获得一个 `i32` 类型的引用。`&` 表示获取一个引用。
    let reference = &4;
    let ref _is_a_reference = 3;
	println!("help {:?} {:?} {:?}",reference,*reference,_is_a_reference);
    match reference {
        // 如果 `reference` 是对 `&val` 进行模式匹配，则会产生如下比较行为：
        // `&i32`
        // `&val`
        // ^ 我们看到，如果匹配的 `&` 都去掉了，那么就是 `i32` 赋给 `val`。
        &val => println!("Got a value via destructuring: {:?}", val),
    }

    // 为了避免 `&` 的使用，需要在匹配前解引用。
    match *reference {
        val => println!("Got a value via dereferencing: {:?}", val),
    }

    // 如果没有一个引用头部（以 & 开头）会是怎样？ `reference` 是一个 `&`，
    // 因为右边已经是一个引用。
    // 下面这个不是引用，因为右边不是。
    let _not_a_reference = 3;

    // Rust 对这种情况提供了 `ref`。它更改了赋值行为，使得可以对具体值
    // 创建引用。这将得到一个引用。
    
    // 相应地，定义两个非引用的值，通过 `ref` 和 `mut` 可以取得引用。
    let value = 5;
    let mut mut_value = 6;

    // 使用 `ref` 关键字来创建引用。
    match value {
        ref r => println!("Got a reference to a value: {:?}", r),
    }

    // 类似地使用 `ref mut`。
    match mut_value {
        ref mut m => {
            // 获得一个引用。在增加内容之前，要先得到解引用（Gotta
            // dereference it before we can add anything to it）。
            *m += 10;
            println!("We added 10. `mut_value`: {:?}", m);
        },
    }
}
fn testa() {
    struct Foo { x: (u32, u32), y: u32 }

    // 解构结构体的成员
    let foo = Foo { x: (1, 2), y: 3 };
    let Foo { x: (a, b), y } = foo;

    println!("a = {}, b = {},  y = {} ", a, b, y);

    // 可以解构结构体并重命名变量，成员顺序是不重要的

    let Foo { y: i, x: j } = foo;
    println!("i = {:?}, j = {:?}", i, j);

    // 也可以忽略某些变量
    let Foo { y, .. } = foo;
    println!("y = {}", y);	
}
fn testb() {
    let number = Some(7);
    let letter: Option<i32> = None;
    let emoticon: Option<i32> = None;

    // `if let` 结构解读：若 `let` 将 `number` 解构成 `Some(i)`，则执行
    // 语句块（`{}`）
    if let Some(i) = number {
        println!("Matched {:?}!", i);
    }

    // 如果要指明失败情形，就使用 else：
    if let Some(i) = letter {
        println!("Matched {:?}!", i);
    } else {
        // 解构失败。换到失败情形（Change to the failure case）。
        println!("Didn't match a number. Let's go with a letter!");
    };

    // 提供一个改变的失败条件（Provide an altered failing condition）。
    let i_like_letters = false;

    if let Some(i) = emoticon {
        println!("Matched {:?}!", i);
    // 解构失败。执行 `else if` 条件来判断轮到的失败分支是否需要执行
    } else if i_like_letters {
        println!("Didn't match a number. Let's go with a letter!");
    } else {
        // 条件执行错误。这是默认的分支：
        println!("I don't like letters. Let's go with an emoticon :)!");
    };

    let mut optional = Some(0);

    // 分析：当 `let` 将 `optional` 解构成 `Some(i)` 时，就
    // 执行语句块（`{}`）。否则中断退出（`break`）。
    while let Some(i) = optional {
        if i > 9 {
            println!("Greater than 9, quit!");
            optional = None;
        } else {
            println!("`i` is `{:?}`. Try again.", i);
            optional = Some(i + 1);
        }
        // ^ 使用的缩进更少，并且不用显式地处理失败情况。
    }
}

fn testc() {
    // 我们在这里使用函数，并在后面的其他位置定义它
    fizzbuzz_to(100);
}

// 函数返回一个布尔（boolean）值
fn is_divisible_by(lhs: u32, rhs: u32) -> bool {
    // 极端情况，提前返回（Corner case, early return）
    if rhs == 0 {
        return false;
    }

    // 这是一个表达式，这里可以不用 `return` 关键字
    lhs % rhs == 0
}

// 函数不返回值，而实际上是返回一个单元类型 `()`
fn fizzbuzz(n: u32) -> () {
    if is_divisible_by(n, 15) {
        println!("fizzbuzz");
    } else if is_divisible_by(n, 3) {
        println!("fizz");
    } else if is_divisible_by(n, 5) {
        println!("buzz");
    } else {
        println!("{}", n);
    }
}

// 当函数返回 `()` 时，可以从标记中删除返回类型
fn fizzbuzz_to(n: u32) {
    for n in 1..n + 1 {
        fizzbuzz(n);
    }
}

struct Point {
    x: f64,
    y: f64,
}

// 实现的代码块，所有的 `Point` 方法都在这里给出
impl Point {
    // 这是一个静态方法（static method）
    // 静态方法不需要通过实例来调用
    // 这类方法一般用作构造器（constructor）
    fn origin() -> Point {
        Point { x: 0.0, y: 0.0 }
    }

    // 另外一个静态方法，带有两个参数：
    fn new(x: f64, y: f64) -> Point {
        Point { x: x, y: y }
    }
}

struct Rectangle {
    p1: Point,
    p2: Point,
}

impl Rectangle {
    // 这是实例方法（instance method）
    // `&self` 是 `self: &Self` 的语法糖（sugar），其中 `Self` 是所调用对象
    // 的类型。在这个例子中 `Self` = `Rectangle`
    fn area(&self) -> f64 {
        // `self` 通过点运算符来访问结构体字段
        let Point { x: x1, y: y1 } = self.p1;
        let Point { x: x2, y: y2 } = self.p2;

        // `abs` 是一个 `f64` 类型的方法，返回调用者的绝对值
        ((x1 - x2) * (y1 - y2)).abs()
    }

    fn perimeter(&self) -> f64 {
        let Point { x: x1, y: y1 } = self.p1;
        let Point { x: x2, y: y2 } = self.p2;

        2.0 * ((x1 - x2).abs() + (y1 - y2).abs())
    }

    // 这个方法要求调用者对象是可变的
    // `&mut self` 为 `self: &mut Self` 的语法糖
    fn translate(&mut self, x: f64, y: f64) {
        self.p1.x += x;
        self.p2.x += x;

        self.p1.y += y;
        self.p2.y += y;
    }
}

// `Pair` 含有的资源：两个堆分配的整型
struct Pair(Box<i32>, Box<i32>);

impl Pair {
    // 这个方法“消费”调用者对象的资源
    // `self` 为 `self: Self` 的语法糖
    fn destroy(self) {
        // 解构 `self`
        let Pair(first, second) = self;

        println!("Destroying Pair({}, {})", first, second);

        // `first` 和 `second` 离开作用域后释放
    }
}

fn testd() {
    let rectangle = Rectangle {
        // 静态方法使用双重冒号来调用
        p1: Point::origin(),
        p2: Point::new(3.0, 4.0),
    };

    // 实例方法通过点运算符来调用
    // 注意第一个参数 `&self` 是隐式传递的，比如：
    // `rectangle.perimeter()` === `Rectangle::perimeter(&rectangle)`
    println!("Rectangle perimeter: {}", rectangle.perimeter());
    println!("Rectangle area: {}", rectangle.area());

    let mut square = Rectangle {
        p1: Point::origin(),
        p2: Point::new(1.0, 1.0),
    };

    // 报错！ `rectangle` 是不可变的，但这方法需要一个可变对象
    //rectangle.translate(1.0, 0.0);
    // 试一试 ^ 将此行注释去掉

    // 正常运行！可变对象可以调用可变方法
    square.translate(1.0, 1.0);

    let pair = Pair(Box::new(1), Box::new(2));

    pair.destroy();

    // 报错！前面的 `destroy` 调用“消费了” `pair`
    // pair.destroy();
    // 试一试 ^ 将此行注释去掉

    // 通过闭包和函数实现增量。
    fn  function            (i: i32) -> i32 { i + 1 }

    // 闭包是匿名的，这里我们将它们绑定到引用。
    // 类型标注和函数的一样，不过类型标注和使用 `{}` 来围住代码都是可选的。
    // 这些匿名函数（nameless function）赋值给合适命名的变量。
    let closure_annotated = |i: i32| -> i32 { i + 1 };
    let closure_inferred  = |i     |          i + 1  ;

    let i = 1;
    // 调用函数和闭包。
    println!("function: {}", function(i));
    println!("closure_annotated: {}", closure_annotated(i));
    println!("closure_inferred: {}", closure_inferred(i));

    // 没有参数的闭包，返回一个 `i32` 类型。
    // 返回类型是自动推导的。
    let one = || 1;
    println!("closure returning one: {}", one());

    use std::mem;
    
    let color = "green";

    // 闭包打印 `color`，它会马上借用（`&`）`color` 并将该借用和闭包存储
    // 到 `print` 变量中。它会一保持借用状态直到 `print` 离开作用域。
    // `println!` 只需要`通过引用`，所以它没有采用更多任何限制性的内容。
    // （原文：`println!` only requires `by reference` so it doesn't
    // impose anything more restrictive.）
    let print = || println!("`color`: {}", color);

    // 使用借用来调用闭包。
    print();
    print();

    let mut count = 0;

    // 闭包使 `count` 值增加，可以使用 `&mut count` 或者 `count`，
    // 但 `&mut count` 限制更少，所以采用它。立刻借用 `count`。
    // （原文： A closure to increment `count` could take either
    // `&mut count` or `count` but `&mut count` is less restrictive so
    // it takes that. Immediately borrows `count`.）
    //
    // `inc` 前面需要加上 `mut`，因为 `&mut` 会必存储的内部。
    // 因此，调用该闭包转变成需要一个 `mut` 的闭包。
    // （原文：A `mut` is required on `inc` because a `&mut` is stored
    // inside. Thus, calling the closure mutates the closure which requires
    // a `mut`.）
    let mut inc = || {
        count += 1;
        println!("`count`: {}", count);
    };

    // 调用闭包。
    inc();
    inc();

    //let reborrow = &mut count;
    // ^ 试一试： 将此行注释去掉。
    
    // 不可复制类型（non-copy type）。
    let movable = Box::new(3);

    // `mem::drop` requires `T` so this must take by value. A copy type
    // would copy into the closure leaving the original untouched.
    // 
    // `mem::drop` 要求 `T`，所以这必须通过值来实现（原文：`mem::drop`
    // requires `T` so this must take by value.）。可复制类型将会复制
    // 值到闭包而不会用到原始值。不可复制类型必须移动（move），从而
    // `可移动`（movable） 立即移动到闭包中（原文：A non-copy must
    // move and so `movable` immediately moves into the closure）。
    let consume = || {
        println!("`movable`: {:?}", movable);
        mem::drop(movable);
    };

    // `consume` 消费（consume）了该变量，所以这只能调用一次。
    consume();
    //consume();
    // ^ 试一试：将此行注释去掉。
            
}

// 将闭包作为参数并调用它的函数。
fn apply<F>(f: F) where
    // 闭包没有输入值和返回值。
    F: FnOnce() {
    // ^ 试一试：将 `FnOnce` 换成 `Fn` 或 `FnMut`。

    f();
}

// 使用闭包并返回一个 `i32` 整型的函数。
fn apply_to_3<F>(f: F) -> i32 where
// 闭包处理一个 `i32` 整型并返回一个 `i32` 整型。
    F: Fn(i32) -> i32 {

    f(3)
}

fn teste() {
    use std::mem;
    
    let greeting = "hello";
    // 不可复制的类型。
    // `to_owned` 从借用的数据创建属于自己的数据。
    let mut farewell = "goodbye".to_owned();

    // 捕获 2 个变量：通过引用方式的 `greeting` 和
    // 通过值方式的 `farewell`。
    let diary = || {
        // `greeting` 使用引用方式：需要 `Fn`。
        println!("I said {}.", greeting);

        // 改变迫使 `farewell` 变成了通过可变引用来捕获。
        // （原文：Mutation forces `farewell` to be
        // captured by mutable reference.）
        // 现在需要 `FnMut`。
        farewell.push_str("!!!");
        println!("Then I screamed {}.", farewell);
        println!("Now I can sleep. zzzzz");

        // 手动调用 drop 将 `farewell` 强制转成通过值来捕获。
        // （原文：Manually calling drop forces `farewell` to
        // be captured by value. Now requires `FnOnce`.）
        // 现在需要 `FnOnce`。
        mem::drop(farewell);
    };

    // 调用处理闭包的函数（原文：Call the function
    // which applies the closure）。
    apply(diary);

    // `double` 满足 `apply_to_3` 的 trait 限定。
    let double = |x| 2 * x;

    println!("3 doubled: {}", apply_to_3(double));


}

fn create_fn() -> Box<Fn()> {
    let text = "Fn".to_owned();

    Box::new(move || println!("This is a: {}", text))
}

fn create_fnmut() -> Box<FnMut()> {
    let text = "FnMut".to_owned();

    Box::new(move || println!("This is a: {}", text))
}

fn testf() {
    let fn_plain = create_fn();
    let mut fn_mut = create_fnmut();

    fn_plain();
    fn_mut();
}

fn test00() {
    let vec1 = vec![1, 2, 3];
    let vec2 = vec![4, 5, 6];

    // 对 vec 的 `iter()` 产出 `&i32`（原文：`iter()` for vecs yields
    // `&i32`）。解构成 `i32` 类型。
    println!("2 in vec1: {}", vec1.iter()     .any(|&x| x == 2));
    // 对 vec 的 `into_iter()` 产出 `i32` 类型。无需解构。
    println!("2 in vec2: {}", vec2.into_iter().any(| x| x == 2));

    let array1 = [1, 2, 3];
    let array2 = [4, 5, 6];

    // 对数组（array）的 `iter()` 产出 `&i32`。
    println!("2 in array1: {}", array1.iter()     .any(|&x| x == 2));
    // 对数组的 `into_iter()` 通常产出 `&i32`。
    println!("2 in array2: {}", array2.into_iter().any(|&x| x == 2));
}

fn test01() {
    let vec1 = vec![1, 2, 3];
    let vec2 = vec![4, 5, 6];

    // 对 vec 产出 `&i32` 类型。
    let mut iter = vec1.iter();
    // 对 vec 产出 `i32` 类型。
    let mut into_iter = vec2.into_iter();

    // 产出内容的引用是 `&&i32` 类型。解构成 `i32` 类型。
    println!("Find 2 in vec1: {:?}", iter     .find(|&&x| x == 2));
    // 产出内容的引用是 `&i32` 类型。解构成 `i32` 类型。
    println!("Find 2 in vec2: {:?}", into_iter.find(| &x| x == 2));

    let array1 = [1, 2, 3];
    let array2 = [4, 5, 6];

    // 对数组 `iter()`  产出 `&i32`。
    println!("Find 2 in array1: {:?}", array1.iter()     .find(|&&x| x == 2));
    // 对数组的 `into_iter()` 通常产出 `&i32``。
    println!("Find 2 in array2: {:?}", array2.into_iter().find(|&&x| x == 2));
}

fn is_odd(n: u32) -> bool {
    n % 2 == 1
}

fn test02() {
    println!("Find the sum of all the squared odd numbers under 1000");
    let upper = 1000;

    // 命令式方式（imperative approach）
    // 声明累加器变量
    let mut acc = 0;
    // 重复：0，1, 2, ... 到无穷大
    for n in 0.. {
        // 数字的平方
        let n_squared = n * n;

        if n_squared >= upper {
            // 若大于上限（upper limit）则退出循环
            break;
        } else if is_odd(n_squared) {
            // 如果是奇数就累加值
            acc += n_squared;
        }
    }
    println!("imperative style: {}", acc);

    // 函数式方式（functional approach）
    let sum_of_squared_odd_numbers: u32 =
        (0..).map(|n| n * n)             // 所有自然数的平方
             .take_while(|&n| n < upper) // 小于上限
             .filter(|&n| is_odd(n))     // 为奇数
             .fold(0, |sum, i| sum + i); // 最后其后
    println!("functional style: {}", sum_of_squared_odd_numbers);
}

mod my {
    // 在模块中的项默认带有私有可见性。
    fn private_function() {
        println!("called `my::private_function()`");
    }

    // 使用 `pub` 修饰语来改变默认可见性。
    pub fn function() {
        println!("called `my::function()`");
    }
    
    // 在同一模块中，项可以访问其它项，即使是私有属性。
    pub fn indirect_access() {
        print!("called `my::indirect_access()`, that\n> ");
        private_function();
    }

    // 项也可以嵌套。
    pub mod nested {
        pub fn function() {
            println!("called `my::nested::function()`");
        }

        #[allow(dead_code)]
        fn private_function() {
            println!("called `my::nested::private_function()`");
        }
    }
    
    // 嵌套项的可见性遵循相同的规则。
    mod private_nested {
        #[allow(dead_code)]
        pub fn function() {
            println!("called `my::private_nested::function()`");
        }
    }
}

fn function() {
    println!("called `function()`");
}

fn test03() {
    // 模块允许在拥有相同名字的项之间消除歧义。
    function();
    my::function();
    
    // 公有项，包括内部嵌套的公有项，可以在父级的模块中访问到。
    my::indirect_access();
    my::nested::function();

    // 一个模块中的私有项不能被直接访问，即使私有项嵌套在公有的模块中：

    // 报错！`private_function` 是私有的。
    //my::private_function();
    // 试一试 ^ 将此行注释去掉

    // 报错！ `private_function` 是私有的。
    //my::nested::private_function();
    // 试一试 ^ 将此行注释去掉    

    // 报错！ `private_nested` 是私有的模块。
    //my::private_nested::function();
    // 试一试 ^ 将此行注释去掉    

}

mod my_0 {
    // 一个公有的结构体，带有一个公有的泛型类型 `T` 的字段
    pub struct WhiteBox<T> {
        pub contents: T,
    }

    // 一个公开的结构体，带有一个私有的泛型类型 `T` 的字段    
    #[allow(dead_code)]
    pub struct BlackBox<T> {
        contents: T,
    }

    impl<T> BlackBox<T> {
        // 一个公有的构造器方法
        pub fn new(contents: T) -> BlackBox<T> {
            BlackBox {
                contents: contents,
            }
        }
    }
}

fn test04() {
    // 带有公有字段的公有的结构体，可以像平常一样构造
    let white_box = my_0::WhiteBox { contents: "public information" };

    // 并且它们的字段可以正常访问到。
    println!("The white box contains: {}", white_box.contents);

    // 带有私有字段的公有结构体不能使用字段名来构造。
    // 报错！`BlackBox` 含有私有字段。
    //let black_box = my_0::BlackBox { contents: "classified information" };
    // 试一试 ^ 将此行注释去掉


    // 不过带有私有字段的结构体可以使用公有的构造器来创建。
    let _black_box = my_0::BlackBox::new("classified information");

    // 并且一个结构体中的私有字段不能访问到。
    // 报错！`content` 字段是私有的。
    //println!("The black box contains: {}", _black_box.contents);
    // 试一试 ^ 将此行注释去掉    

}

fn test00_0f() {
    test00();
    test01();
    test02();
    test03();
    test04();
    test05();	
    test06();
    test07();
    test08();
    test09();
    test0a();
    test0b();	
    test0c();	
    test0d();	
    test0e();
    test0f();
    test10_1f();	
}

// 将 `deeply::nested::function` 路径绑定到 `other_function`。
use deeply::nested::function as other_function;

fn deep_function() {
    println!("called `function()`");
}

mod deeply {
    pub mod nested {
        pub fn function() {
            println!("called `deeply::nested::function()`")
        }
    }
}

fn test05() {
    // 更容易访问 `deeply::nested::funcion`
    other_function();

    println!("Entering block");
    {
        // 这和 `use deeply::nested::function as function` 等价。
        // 此 `function()` 将覆盖掉的外部同名函数。
        use deeply::nested::function;
        function();

        // `use` 绑定拥有局部作用域。在这个例子中，`function()`
        // 的覆盖只限定在这个代码块中。
        println!("Leaving block");
    }

    deep_function();
}
mod cool {
    pub fn function() {
        println!("called `cool::function()`");
    }
}

mod my_1 {
    fn function() {
        println!("called `my::function()`");
    }
    
    mod cool {
        pub fn function() {
            println!("called `my::cool::function()`");
        }
    }
    
    pub fn indirect_call() {
        // 让我们从这个作用域中访问所有名为 `function` 的函数！
        print!("called `my::indirect_call()`, that\n> ");
        
        // `self` 关键字表示当前的模块作用域——在这个例子是 `my`。
        // 调用 `self::function()` 和直接访问 `function()` 两者都得到相同的结果，
        // 因为他们表示相同的函数。
        self::function();
        function();
        
        // 我们也可以使用 `self` 来访问 `my` 内部的另一个模块：
        self::cool::function();
        
        // `super` 关键字表示父级作用域（在 `my` 模块外面）。
        print!("super ");
        super::function();
        
        // 这将在 *crate* 作用域内绑定 `cool::function` 。
        // 在这个例子中，crate 作用域是最外面的作用域。
        {
            use cool::function as root_function;
            root_function();
        }
    }
}

fn test06() {
    my_1::indirect_call();
}

#[cfg(target_os = "linux")]
fn are_you_on_linux() {
    println!("You are running linux!")
}

// 而这个函数仅当操作系统**不是** Linux 时才会编译
#[cfg(not(target_os = "linux"))]
fn are_you_on_linux() {
    println!("You are *not* running linux!")
}

fn test07() {
    are_you_on_linux();
    
    println!("Are you sure?");
    if cfg!(target_os = "linux") {
        println!("Yes. It's definitely linux!");
    } else {
        println!("Yes. It's definitely *not* linux!");
    }
}

// 具体的类型 `A`。
#[derive(Debug)]
struct A;

// 在定义类型 `Single` 时，在 `A` 的首次使用之前没有出现 `<A>`。
// 因此，`Single` 是一个具体的类型，`A` 在上面已经定义。
// （原文：In defining the type `Single`, the first use of `A` is not preceded
// by `<A>`. Therefore, `Single` is a concrete type, and `A` is defined as above.）
#[derive(Debug)]
struct Single(A);
//            ^ 这里是 `Single` 对类型 `A` 的第一次使用。

// 此处 `<T>` 在第一次使用 `T` 前出现，所以 `SingleGen` 是一个泛型类型。
// 因为类型参量 `T` 是泛型，所以它可以是任何类型，包括在上面定义的具体类型 `A`。
#[derive(Debug)]
struct SingleGen<T>(T);

fn test08() {
    // `Single` 是具体类型并显式地接受 `A`。
    let _s = Single(A);
    
    // 创建一个 `SingleGen<char>` 类型的变量 `_char`，并给一个 `SingleGen('a') 值。
    // 这里的 `SingleGen` 拥有显式指定的类型参量。
    let _char: SingleGen<char> = SingleGen('a');

    // `SingleGen` 也可以拥有隐式指定的类型参量：
    let _t    = SingleGen(A); // 使用在上面定义的 `A`。
    let _i32  = SingleGen(6); // 使用 `i32` 类型。
    let _char = SingleGen('a'); // 使用 `char`。

    println!("{:?},{:?},{:?},{:?},{:?}",_s,_char,_t,_i32,_char);
}
struct S(A);       // 具体类型 `S`。
struct SGen<T>(T); // 泛型类型 `SGen`。

// 下面全部函数都得到了变量的所有权，传递给函数的变量在离开作用域时立即释放。
// （原文：The following functions all take ownership of the variable passed
// into them and immediately go out of scope, freeing the variable.）

// 定义一个函数 `reg_fn`，接受一个 `S` 类型的参数 `_s`。
// 因为没有 `<T>`，所以这不是泛型函数。
fn reg_fn(_s: S) {}

// 定义一个函数 `gen_spec_t`，接受一个 `SGen<T>` 类型的参数 `_s`。
// 这里显式地给出了类型参量 `A`，但因为 `A` 没有被指明为针对 `gen_spec_t` 的
// 泛型类型参量，所以这不是一个泛型。
fn gen_spec_t(_s: SGen<A>) {}

// 定义一个函数 `gen_spec_i32`，接受一个 `SGen<i32>` 类型的参数 `_s`。
// 这里显式地给出了类型参量 `i32`，而 `i32` 是一个具体类型。
// 由于 `i32` 不是一个泛型类型，所以这个函数也不是泛型。
fn gen_spec_i32(_s: SGen<i32>) {}

// 定义一个函数 `generic`，接受一个 `SGen<T>` 类型的参数 `_s`。
// 因为 `SGen<T>` 之前给定了 `<T>`，所以这个函数是关于 `T` 的泛型。
fn generic<T>(_s: SGen<T>) {}

fn test09() {
    // 使用非泛型函数
    reg_fn(S(A));          // 具体类型。
    gen_spec_t(SGen(A));   // 隐式地指定类型参量 `A`。
    gen_spec_i32(SGen(6)); // 隐式地指定类型参量 `i32`。

    // 显式地指定类型参量 `char` 传给 `generic()`。
    generic::<char>(SGen('a'));

    // 隐式地指定类型参量 `char` 传给 `generic()`。
    generic(SGen('c'));
}

struct Val {
    val: f64
}

struct GenVal<T>{
    gen_val: T
}

// Val 的实现（impl）
impl Val {
    fn value(&self) -> &f64 { &self.val }
}

// GenVal 针对泛型类型 `T` 的实现
impl <T> GenVal<T> {
    fn value(&self) -> &T { &self.gen_val }
}

fn test0a() {
    let x = Val { val: 3.0 };
    let y = GenVal { gen_val: 3i32 };
    
    println!("{}, {}", x.value(), y.value());
}

// 不可复制的类型。
struct Empty;
struct Null;

// 用到 `T` 的trait 泛型。
trait DoubleDrop<T> {
    // 定义一个关于调用者的方法，接受一个额外的单一参量 `T`，
    // 且没有任何操作。
    fn double_drop(self, _: T);
}

// 针对泛型参量 `T` 和调用者 `U` 实现了 `DoubleDrop<T>` 。
impl<T, U> DoubleDrop<T> for U {
    // 此方法获得了两个传入参数的所有权，并释放这两个参数。
    fn double_drop(self, _: T) {}
}

fn test0b() {
    let empty = Empty;
    let null  = Null;

    // 释放 `empty` 和 `null`。
    empty.double_drop(null);

    //empty;
    //null;
    // ^ 试一试：去掉这两行的注释。
}
// 这个 trait 实现了打印标记：`{:?}`。
use std::fmt::Debug;

trait HasArea {
    fn area(&self) -> f64;
}

impl HasArea for RectangleA {
    fn area(&self) -> f64 { self.length * self.height }
}

#[derive(Debug)]
struct RectangleA { length: f64, height: f64 }
#[allow(dead_code)]
struct Triangle  { length: f64, height: f64 }

// 泛型 `T` 必须实现 `Debug`。不管什么类型，都可以正常工作。
fn print_debug<T: Debug>(t: &T) {
    println!("{:?}", t);
}

// `T` 必须实现 `HasArea`。任意符合限定的函数都可以访问
// `HasArea` 的 `area` 函数。
fn area<T: HasArea>(t: &T) -> f64 { t.area() }

fn test0c() {
    let rectangle = RectangleA { length: 3.0, height: 4.0 };
    let _triangle = Triangle  { length: 3.0, height: 4.0 };

    print_debug(&rectangle);
    println!("Area: {}", area(&rectangle));

    //print_debug(&_triangle);
    //println!("Area: {}", area(&_triangle));
    // ^ 试一试：将上述语句的注释去掉。
    // | 报错：未实现 `Debug` 或 `HasArea`。
}
struct Cardinal;
struct BlueJay;
struct Turkey;

trait Red {}
trait Blue {}

impl Red for Cardinal {}
impl Blue for BlueJay {}

// 这些函数只对实现了相应的 trait 的类型有效。实际情况中 trait 内部
// 是否为空都无所谓。
fn red<T: Red>(_: &T)   -> &'static str { "red" }
fn blue<T: Blue>(_: &T) -> &'static str { "blue" }

fn test0d() {
    let cardinal = Cardinal;
    let blue_jay = BlueJay;
    let _turkey   = Turkey;

    // 由于限定，`red()` 不能调用 blue_jay （蓝松鸟），
    // 反过来也一样。
    println!("A cardinal is {}", red(&cardinal));
    println!("A blue jay is {}", blue(&blue_jay));
    //println!("A turkey is {}", red(&_turkey));
    // ^ 试一试：将此行注释去掉。
}

use std::fmt::Display;

fn compare_prints<T: Debug + Display>(t: &T) {
    println!("Debug: `{:?}`", t);
    println!("Display: `{}`", t);
}

fn compare_types<T: Debug, U: Debug>(t: &T, u: &U) {
    println!("t: `{:?}", t);
    println!("u: `{:?}", u);
}

fn test0e() {
    let string = "words";
    let array = [1, 2, 3];
    let vec = vec![1, 2, 3];

    compare_prints(&string);
    //compare_prints(&array);
    // 试一试 ^ 将此行注释去掉。

    compare_types(&array, &vec);
}

trait PrintInOption {
    fn print_in_option(self);
}

// 这里需要一个 `where` 从句，否则就要表达成 `T: Debug`
// 或使用另一种间接的方法。
impl<T> PrintInOption for T where
    Option<T>: Debug {
    // 我们要将 `Option<T>: Debug` 作为限定，因为那是要打印的内容。
    // 不这样做的话，很可能就用到错误的限定。
    fn print_in_option(self) {
        println!("{:?}", Some(self));
    }
}

fn test0f() {
    let vec = vec![1, 2, 3];

    vec.print_in_option();
}
struct Container(i32, i32);

// 这个 trait 检查 2 个项是否存到 Container（容器）中。
// 还会获得第一个值或最后一个值。
trait Contains<A, B> {
    fn contains(&self, &A, &B) -> bool; // 显式指出需要 `A` 和 `B`
    fn first(&self) -> i32; // 未显式指出需要 `A` 或 `B`
    fn last(&self) -> i32;  // 未显式指出需要 `A` 或 `B`
}

impl Contains<i32, i32> for Container {
    // 如果存储的数字相等则为真。
    fn contains(&self, number_1: &i32, number_2: &i32) -> bool {
        (&self.0 == number_1) && (&self.1 == number_2)
    }

    // 得到第一个数字。
    fn first(&self) -> i32 { self.0 }

    // 得到最后一个数字。
    fn last(&self) -> i32 { self.1 }
}

// `C` 包含 `A` 和 `B` 。鉴于此，必须重复表达 `A` 和 `B` 真麻烦。
fn difference<A, B, C>(container: &C) -> i32 where
    C: Contains<A, B> {
    container.last() - container.first()
}
fn test10_1f() {
    test10();
    test11();
    test12();
    test13();
    test14();
    test15();	
    test16();
    test17();
    test18();
    test19();
    test1a();
    test1b();	
    test1c();	
    test1d();	
    test1e();
    test1f();
}
fn test10() {
    let number_1 = 3;
    let number_2 = 10;

    let container = Container(number_1, number_2);

    println!("Does container contain {} and {}: {}",
        &number_1, &number_2,
        container.contains(&number_1, &number_2));
    println!("First number: {}", container.first());
    println!("Last number: {}", container.last());

    println!("The difference is: {}", difference(&container));
}

// fn test10() {}
// fn test11() {}
// fn test12() {}
// fn test13() {}
// fn test14() {}
// fn test15() {}
// fn test16() {}
// fn test17() {}
// fn test18() {}
// fn test19() {}
// fn test1a() {}
// fn test1b() {}
// fn test1c() {}
// fn test1d() {}
// fn test1e() {}
// fn test1f() {}

use std::marker::PhantomData;

// 虚位元组结构体，这是一个带有 `A` 和隐藏参量（hidden parameter） `B` 的泛型。
#[derive(PartialEq)] // 允许这种类型进行相等测试（equality test）。
struct PhantomTuple<A, B>(A,PhantomData<B>);

// 模型元组结构体，这是一个带有 `A` 和隐藏参量 `B` 的泛型。
#[derive(PartialEq)] // 允许这种类型进行相等测试。
struct PhantomStruct<A, B> { first: A, phantom: PhantomData<B> }

// 注意：对于泛型 `A` 会分配存储空间，但 `B` 不会。
//       因此，`B` 不能参与运算。

fn test11() {
    // 这里的 `f32` 和 `f64` 是隐藏参量。
    // 被指定为 `<char, f32>` 的虚位元组（PhantomTuple）类型。
    let _tuple1: PhantomTuple<char, f32> = PhantomTuple('Q', PhantomData);
    // 被指定为 `<char, f64>` 的虚位元组。
    let _tuple2: PhantomTuple<char, f64> = PhantomTuple('Q', PhantomData);

    // 被指定为 `<char, f32>` 的类型。
    let _struct1: PhantomStruct<char, f32> = PhantomStruct {
        first: 'Q',
        phantom: PhantomData,
    };
    // 被指定为 `<char, f64>` 的类型。
    let _struct2: PhantomStruct<char, f64> = PhantomStruct {
        first: 'Q',
        phantom: PhantomData,
    };
    
    // 编译期（compile-time）报错！类型不匹配，所以这些值不能够比较：
    //println!("_tuple1 == _tuple2 yields: {}",
    //          _tuple1 == _tuple2);
    
    // 编译期报错！类型不匹配，所以这些值不能够比较：
    //println!("_struct1 == _struct2 yields: {}",
    //          _struct1 == _struct2);
}
use std::ops::Add;

/// 创建空枚举来定义单位类型。
#[derive(Debug, Clone, Copy)]
enum Inch {}
#[derive(Debug, Clone, Copy)]
enum Mm {}

/// `Length` 是一个带有虚位类型参量的 `Unit`（单位），
/// 而且不是关于长类型（即 `f64`）的泛型。
///
/// `f64` 已经实现了 `Clone` 和 `Copy` trait.
#[derive(Debug, Clone, Copy)]
struct Length<Unit>(f64, PhantomData<Unit>);

/// `Add` trait 定义了 `+` 运算符的行为。
impl<Unit> Add for Length<Unit> {
     type Output = Length<Unit>;

    // add() 返回一个全新的包含总和的 `Length` 结构体。
    fn add(self, rhs: Length<Unit>) -> Length<Unit> {
        // `+` 调用了针对 `f64` 类型的 `Add` 实现。
        Length(self.0 + rhs.0, PhantomData)
    }
}

fn test12() {
    // 指出 `one_foot` 拥有虚位类型参量 `Inch`。
    let one_foot:  Length<Inch> = Length(12.0, PhantomData);
    // `one_meter` 拥有虚位类型参量 `Mm`。
    let one_meter: Length<Mm>   = Length(1000.0, PhantomData);

    // `+` 调用了 `add()` 方法，该方法对 `Length<Unit>` 进行了实现。
    //
    // 由于 `Length` 了实现了 `Copy`，于是 `add()` 不会消费 `one_foot`
    // 和 `one_meter`，但会复制它们到 `self` 和 `rhs`。
    let two_feet = one_foot + one_foot;
    let two_meters = one_meter + one_meter;

    // 加法正常执行。
    println!("one foot + one_foot = {:?} in", two_feet.0);
    println!("one meter + one_meter = {:?} mm", two_meters.0);

    // 无意义的操作将会失败，因为它们会导致：
    // 编译期报错：类型不匹配（Compile-time Error: type mismatch.）。
    //let one_feter = one_foot + one_meter;
}
// raii.rs
fn create_box() {
    // 在堆上分配一个整型数据
    let _box1 = Box::new(3i32);

    // `_box1` 在这里销毁，而且内存得到释放
}

fn test13() {
    // 在堆上分配一个整型数据    
    let _box2 = Box::new(5i32);

    // 嵌套作用域：
    {
        // 在堆上分配一个整型数据
        let _box3 = Box::new(4i32);

        // `_box3` 在这里销毁，而且内存得到释放        
    }

    // 创建很多 box，纯属娱乐。
    // 完全不需要手动释放内存！
    for _ in 0u32..1_000 {
        create_box();
    }

    // `_box2` 在这里销毁，而且内存得到释放    
}
// 此函数取倒堆分配的内存的所有权
fn destroy_box(c: Box<i32>) {
    println!("Destroying a box that contains {}", c);

    // `c` 被销毁且内存得到释放
}

fn test14() {
    // 栈分配的整型
    let x = 5u32;

    // 将 `x` **复制**（*Copy*）到 `y`——不存在资源移动
    let y = x;

    // 两个值都可以独立地使用
    println!("x is {}, and y is {}", x, y);

    // `a` 是一个指向堆分配的整型的指针
    let a = Box::new(5i32);

    println!("a contains: {}", a);

    // **移动**（*Move*) `a` 到 `b`
    let b = a;
    // 把 `a` 的指针地址（非数据）复制到 `b`。现在两者都是指向
    // 同一个堆分配的数据，但是现在是 `b` 占有它。
    
    // 报错！`a` 再也不能访问数据，因为它不再拥有堆上的内存。
    //println!("a contains: {}", a);
    // 试一试 ^ 将此行注释去掉

    // 此函数从 `b` 中取得栈分配的内存的所有权
    destroy_box(b);

    // 此时堆上的内存已经释放掉，而这个操作会导致解引用已释放的内存，
    // 但这种情况会被编译器会禁止。
    // 报错！和前面出错的原因一样。
    //println!("b contains: {}", b);
    // 试一试 ^ 将此行注释去掉
}
fn test15() {
    let immutable_box = Box::new(5u32);

    println!("immutable_box contains {}", immutable_box);

    // 可变性错误
    //*immutable_box = 4;

    // **移动** box，改变所有权（和可变性）
    let mut mutable_box = immutable_box;

    println!("mutable_box contains {}", mutable_box);

    // 修改 box 的内容
    *mutable_box = 4;

    println!("mutable_box now contains {}", mutable_box);
}

fn test16() {}
fn test17() {}
fn test18() {}
fn test19() {}
fn test1a() {}
fn test1b() {}
fn test1c() {}
fn test1d() {}
fn test1e() {}
fn test1f() {}
