struct Struct {
    m: i32
}

fn main() {

    // 默认情况下 Rust 的变量是不可变的。
    // 除非你通过 `mut` 关键字让变量变为可变的
    let x = 5;  // 变量绑定
    // x = 6; // 此时报错了
    println!("x = {}", x);

    let mut y = 6;
    println!("y = {}", y);
    y = 7;
    println!("y = {}", y);

    // 使用 _ 来忽略不使用的变量 在编译时候产生的警告
    let _z = 5;
    

    // 变量解构
    let (a, mut b) : (bool, bool) = (true, false);
    println!("a = {:?}, b = {:?}", a, b);
    b = true;
    println!("a = {:?}, b = {:?}", a, b);


    // 解构式赋值
    let (h, j, k, l, m);
    (h, j) = (1, 2);
    [k, .., l, _] = [1, 2, 3, 4, 5];
    Struct { m, .. } = Struct{ m: 5};
    // 1, 2, 1, 4, 5
    println!("h = {}, j = {}, k = {}, l = {}, m = {}", h, j, k, l, m);
}
