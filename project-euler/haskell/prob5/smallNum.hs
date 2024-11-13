{- 
    Problem 5: What is the smallest positive number that is evenly divisible by
    all of the numbers from 1 to 20? 
-}

smallNum :: [Int] -> Int
smallNum xs = foldr (\x acc -> lcm x acc) 1 xs
