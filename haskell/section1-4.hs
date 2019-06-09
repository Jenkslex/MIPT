-- Task 1
-- Part a
type_1_a :: [([(Int, Int -> Int)], Char)]
type_1_a = [([((x :: Int), (succ :: Int -> Int)) | x <- [1..3]], s) | s <- "foo"]

-- Part b
type_1_b :: [(Char, Int)] -> ([Int], [Char])
type_1_b b = ([ snd x | x <- b], [fst x | x <- b])

-- Part c
type_1_c :: a -> (a,a);
type_1_c c = (c,c)

-- Part d
type_1_d :: (a -> b) -> a -> b
type_1_d f c = f c


-- Task 2
-- Part a
e1 :: (a, b) -> (b, a)
e1 (x, y) = (y, x)

-- Part b
e2 :: (a, (a -> b)) -> b
e2 (x, y) = y x

-- Part c
e3 :: ((a -> b), a) -> b
e3 (x, y) = x y

-- Part d
e4 :: a -> (a -> b) -> b
e4 x y = y x

-- Part e
-- e5 tries to apply first argument to second argument and itself
-- fails due to cunstructing an infinite type
-- e5 x y = x y x


-- Task 3
-- Part a
my_and True x = x
my_and False _ = False

-- Part b
my_imp False _ = True
my_imp True x = x

-- Part c
my_xor False x = x
my_xor True True = False
my_xor True False = True

-- Part d
my_maj3 False False _ = False
my_maj3 False True x = x
my_maj3 True True _ = True
my_maj3 True False x = x


-- Task 4
replicate' :: Int -> a -> [a]
replicate' n x = if n <= 0
                 then []
                 else [x] ++ (replicate' (n - 1) x)
                 
repeat' :: a -> [a]
repeat' x = [x] ++ (repeat' x)


-- Task 5
isSqr n = (sqrt (fromIntegral n)) == fromIntegral (floor (sqrt (fromIntegral n)))

isPyth (x, y, z) = isSqr zz && sqrt (fromIntegral zz) == fromIntegral z
                where zz = x^2 + y^2

triplets n = [(x,y,z) | x <- [1..(div n 2)], y <- [x..(n-x)], let z = n - x - y, z >= y, y >= x, x > 0]

findPyth xs = [x | x <- xs, isPyth x]

pythagoras_gen n = findPyth (triplets n) ++ pythagoras_gen (n + 1)
pythagoras = pythagoras_gen 12
    

-- Task 7
triangular = [ sum [1..n] | n <- [1..]]
