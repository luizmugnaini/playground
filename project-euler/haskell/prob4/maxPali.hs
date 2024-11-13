maxPali :: Int 
maxPali = 
  max' [n * m |
        n <- [100..999],
        m <- [100..999],
        (show $ n*m) == (rev $ show $ n*m)
      ]

-- Maximum of a list of Int
max' :: [Int] -> Int 
max' (x:xs) 
  | null (x:xs) = 0
  | null xs     = x
  | otherwise   = max x (max' xs)

-- Reverses order of a list
rev :: [a] -> [a]
rev xs = foldl (\acc x -> x:acc) [] xs
