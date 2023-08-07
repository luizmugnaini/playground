get35 :: [Int]
get35 = [x | x <- [1..], x `mod` 3 == 0 || x `mod` 5 == 0]

--sumList :: [Int] -> Int
--sumList = (sum . get35)

mult35 :: Int -> Int
mult35 bound = foldr (\x acc -> x + acc) 0 $ takeWhile (< bound) get35

