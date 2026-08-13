fn main() {
    let mut s = String::from("hello world");

    let word = first_word(&mut s);

    println!("the first word is: {}", word);
    s.clear(); // error!
}
fn first_word(s: &mut String) -> &str {
    &(*s)[..1]
}
