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