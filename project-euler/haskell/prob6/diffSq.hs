diffSq :: [Int] -> Int
diffSq xs = sqSum xs - sumSq xs
  where sumSq xs = sum $ map (\x -> x^2) xs
        sqSum xs = (sum xs)^2
