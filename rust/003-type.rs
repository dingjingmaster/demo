fn main() {
    // 这里 无法将字符串 "42" 进行解析，所以会报错
    // let guess = "42".parse().expect("Not a number!");

    let guess: i32 = "42".parse().expect("Not a number!");
    println!("guess = {}", guess);

    let guess = "42".parse::<i32>().expect("Not a number!");
    println!("guess = {}", guess);
}
