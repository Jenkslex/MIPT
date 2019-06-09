-- Task 9
-- Part a
myHead (x:xs) = x

-- Part b
myTail (x:xs) = xs

-- Part c
myTake n (x:xs) | n > 0 = x : myTake (n-1) xs
myTake n xs = []

-- Part d
myDrop n xs | n < 1 = xs
myDrop n (x:xs) = myDrop (n - 1) xs

-- Part e
myNull [] = True
myNull (x:xs) = False

-- Part f
myElem e [] = False
myElem e (x:xs) = e == x || myElem e xs

-- Part g
myGet xs n | n < 0 = error "myGet: Negative index"
myGet (x:xs) 0 = x
myGet (x:xs) n = myGet xs (n-1)

-- Part h
myAdd [] xs = xs
myAdd xs [] = xs
myAdd (x:xs) ys = x : (myAdd xs ys)

-- Part i
myConcat [] = []
myConcat (x:xs) = myAdd x (myConcat xs)


-- Task 10
seg xs i j = drop i (take (j+1) xs)

segs xs = [seg xs i j | i <- [0..n], j <- [i..n]]
            where n = (length xs) - 1


-- Task 11
-- Part a
euclMod a b | a < b = euclMod b a
euclMod a b | mod a b == 0 = b
euclMod a b = euclMod b (mod a b)

-- Part b
euclSub a b | a < b = euclSub b a
euclSub a b | a - b == 0 = b
euclSub a b = euclSub (a-b) b

-- Check algs
checker f = all (\x -> x == True) [(gcd a b) == (f a b) | a <- [1..1000], b <- [1..1000]]


-- task 12
euclExt a b = euclExt' a b 1 0 0 1

euclExt' a b x2 y2 _ _ | b == 0 = (a, x2, y2)
euclExt' a b x2 y2 x1 y1 = euclExt' b (mod a b) x1 y1 x y
                    where x = x2 - (div a b) * x1 
                          y = y2 - (div a b) * y1 

-- Task 14
dupl [] = []
dupl (x:xs) = [x] ++ [x] ++ (dupl xs)


-- Task 15
nrem n xs = [fst(x) | x <- zip xs [0..], mod (snd x) n /= (n-1)]


-- Task 16
noRepeat xs = [ fst(x) | x <- zip xs [0..], notElem (fst x) (take (snd x) xs)]


-- Task 19
sbseqs [] = [[]]
sbseqs xs = concat [sbseqsl l xs | l <- [1..(length xs)]]

sbseqsl n xs | n > (length xs) = [[]]
sbseqsl n xs | n == (length xs) = [xs]
sbseqsl 1 xs = [[x] | x <- xs]
sbseqsl n (x:xs) = (map (x:) (sbseqsl (n-1) xs)) ++ (sbseqsl n xs)
