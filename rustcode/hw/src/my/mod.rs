pub mod my_0;
pub mod my_1;
pub mod mod_1;
pub mod mod_2;
pub mod mod_3;
pub mod checked;

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