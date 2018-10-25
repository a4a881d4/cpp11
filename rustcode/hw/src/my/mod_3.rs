use std::fmt;

// 此外部代码块链接到 libm 库
#[link(name = "m")]
extern {
    // 这是外部语言函数
    // 这计算了一个单精度复数的平方根
    pub fn csqrtf(z: Complex) -> Complex;
}


// 最小化实现单精度复数
#[repr(C)]
#[derive(Clone, Copy)]
pub struct Complex {
    pub re: f32,
    pub im: f32,
}

impl fmt::Debug for Complex {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        if self.im < 0. {
            write!(f, "{}-{}i", self.re, -self.im)
        } else {
            write!(f, "{}+{}i", self.re, self.im)
        }
    }
}