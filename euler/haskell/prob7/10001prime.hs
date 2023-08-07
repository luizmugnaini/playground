prime :: Int -> Int
prime i = last $ take i primes

prime2 :: Int -> Int
prime2 i = primes !! i

primes :: [Int]
primes = cut [2..] 
  where cut (p:xs) = p:(cut [x | x <- xs, x `mod` p /= 0])
