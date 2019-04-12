/**
  *  样例类
  */
object Demo1 {
  def main(args: Array[String]): Unit = {
    val alice = Person("Alice", 25)
    val bob = Person("Bob", 32)
    val chaelie = Person("Charlie", 32)
    for (person <- List(alice, bob, chaelie)) {
      person match {
        case Person("Alice", 25) => println("Hi Alice!")
        case Person("Bob", 32) => println("Hi Bob!")
        case _ => println("who are you")
      }
    }
  }
  // 样例类
  case class Person(name: String, age: Int)
}
