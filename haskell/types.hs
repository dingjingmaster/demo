module Main where

main :: IO()

main = do
    -- Int
    let x :: Int
        x = 1
    print x

    -- Int range
    let biggestInt, smallestInt :: Int
        biggestInt = maxBound
        smallestInt = minBound
    print "min Int: "
    print smallestInt
    print "max Int: " 
    print biggestInt

    -- reallyBig
    let n :: Integer
        n = 2
    let reallyBig :: Integer
        reallyBig = 2^(2^(2^(2^n)))
    print reallyBig

    let reallyBigLen :: Int
        reallyBigLen = length(show reallyBig)
    print reallyBigLen

    -- Float
    let f :: Float
        f = 0.1
    print f

    -- Double
    let d1, d2 :: Double
        d1 = 3.1415926
        d2 = 6.28e-4
    print d1
    print d2

    -- Booleans
    let b1, b2 :: Bool
        b1 = True
        b2 = False
    print b1
    print b2

    -- Unicode characters
    let c1, c2 :: Char
        c1 = '1'
        c2 = '3'
    print c1
    print c2

    -- String
    let str1 :: String
        str1 = "Hello, world"
    print str1

    -- Arithmetic
    let a, b :: Int
        a = 13
        b = 2
    let c, d :: Float
        c = 13.0
        d = 2.0

    let add1 :: Int
        add1 = a + b
    let add2 :: Float
        add2 = c + d

    let mod1 :: Int
        mod1 = mod a b
    let mod2 :: Int
        mod2 = a `mod` b

    -- error
    -- let divi1 :: Int
    --     divi1 = a / b

    let divi2 :: Float
        divi2 = c / d

    let multi :: Int
        multi = a * b

    let mi :: Int
        mi = a ^ b

    print add1
    print add2
    print mod1
    print mod2
    -- print divi1
    print divi2
    print multi
    print mi

    -- Boolean logic
    -- error: print True && True
    print ('a' == 'b')

    -- Basic functions
    let fun1 :: Integer -> Integer
        fun1 0 = 0
        fun1 n = n + fun1(n - 1)
    print (fun1(100))

    let hailstone :: Integer -> Integer
        hailstone n
            | n `mod` 2 == 0 = n `div` 2
            | otherwise      = 3 * n + 1
    print (hailstone(1))
    print (hailstone(2))

    let foo :: Integer -> Integer
        foo 0 = 16
        foo 1 
            |   "Haskell" > "C++" = 3
            | otherwise           = 4
        foo n
            | n < 0               = 0
            | n `mod` 17 == 2     = -43
            | otherwise           = n + 3
    print (foo(0))
    print (foo(1))
    print (foo(2))
    print (foo(-1))

    let isEven :: Integer -> Bool
        isEven n
            | n `mod` 2 == 0    = True
            | otherwise         = False
    print (isEven(1))
    print (isEven(2))

    -- Pairs
    let p :: (Int, Int)
        p = (3, 2)
    let sumPair :: (Int, Int) -> Int
        sumPair(x, y) = x + y
    print (sumPair(p))

    -- Functions multiple arguments
    let func1 :: Int -> Int -> Int -> Int
        func1 x y z = x + y + z
    let v1 :: Int
        v1 = func1 1 2 3
    print v1

    -- List
    let nums, range, range2 :: [Int]
        nums = [1, 2, 7, 0]
        range = [1..100]
        range2 = [2,4..100]
    print nums
    print range
    print range2

    -- Char List
    let c1 :: [Char]
        c1 = ['h', 'e', 'l', 'l', 'o']
    print c1
    let s1 :: String
        s1 = "hello"
    print s1

    -- 没有隐式转换
    let eq :: Bool
        eq = s1 == c1
    print eq

    -- Functions on lists
    let func2 :: [Integer] -> Integer
        func2 [] = 0
        func2 (x:xs) = 1 + func2 xs
    print (func2 [1, 2, 3, 4, 9]) -- 5

    -- Combining functions
    let func3 :: [Integer] -> Integer
        func3 n = func2(n) - 1

    let func4 :: Integer -> Integer
        func4 n = func3([0..n])

    print (func4(100))
    print (func4(1000))
