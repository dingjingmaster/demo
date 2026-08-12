fn main () {
    println!("for i in i..5");
    for i in 1..5 {
        print!("{} ", i);
    }

    println!("\n\nfor i in 1..=5:");
    for i in 1..=5 {
        print!("{} ", i);
    }

    println!("\n\nfor i in 'a'..='z':");
    for i in 'a'..='z' {
        print!("{} ", i);
    }

    println!("\n");
}
