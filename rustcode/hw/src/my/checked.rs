// 我们想要捕获的数学“错误”
#[derive(Debug)]
pub enum MathError {
    DivisionByZero,
    NegativeLogarithm,
    NegativeSquareRoot,
}

pub type MathResult = Result<f64, MathError>;

pub fn div(x: f64, y: f64) -> MathResult {
    if y == 0.0 {
        // 此操作将会失败，反而让我们返回失败的理由，并装包成 `Err`
        Err(MathError::DivisionByZero)
    } else {
        // 此操作是有效的，返回装包成 `Ok` 的结果
        Ok(x / y)
    }
}

pub fn sqrt(x: f64) -> MathResult {
    if x < 0.0 {
        Err(MathError::NegativeSquareRoot)
    } else {
        Ok(x.sqrt())
    }
}

pub fn ln(x: f64) -> MathResult {
    if x < 0.0 {
        Err(MathError::NegativeLogarithm)
    } else {
        Ok(x.ln())
    }
}
    // 中间函数
fn op_(x: f64, y: f64) -> MathResult {
    // 如果 `div` “失败”了，那么 `DivisionByZero` 将被返回
    let ratio = div(x, y)?;

    // 如果 `ln` “失败”了，那么 `NegativeLogarithm` 将被返回
    let ln = ln(ratio)?;

    sqrt(ln)
}

pub fn op(x: f64, y: f64) {
    match op_(x, y) {
        Err(why) => println!("{}",match why {
                        MathError::NegativeLogarithm
                            => "logarithm of negative number",
                        MathError::DivisionByZero
                            => "division by zero",
                        MathError::NegativeSquareRoot
                            => "square root of negative number",
                    }),
        Ok(value) => println!("{}", value),
    }
}