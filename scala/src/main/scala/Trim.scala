object Trim {
  def main(args: Array[String]): Unit = {
    val a = List[String](" asad  ", "  adas", "asddsa ")

    // 返回为 void
    val b = a.foreach(x => x.trim())

    // 可以返回结果
    val c = a.map(x => x.trim())
    println(b)
    println(c)
  }
}
