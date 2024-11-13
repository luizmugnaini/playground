-- Sum of the elements
evenFib :: Int -> Int
evenFib bound = foldr (\x acc -> x + acc) 0 boundEvenFib
  where boundEvenFib = filter even $ takeWhile (<= bound) fibl
        fibl = 0:1:(zipWith (+) fibl (tail fibl))
