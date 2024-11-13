{- Problem 3: What is the largest prime factor of the number 600851475143? -}

maxFacDiv :: Int -> Int
maxFacDiv n
  | m == smallFac m = m -- Stops if m is the greatest prime factor
  | otherwise       = maxFacDiv (n `div` smallFac n)
  where 
    smallFac n = head [x | x <- [2..n], n `mod` x == 0] -- Always prime
    m = (n `div` smallFac n)
