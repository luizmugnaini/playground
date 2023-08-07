import Data.Char
import Data.List

-- Takes list and returns rearrangement
sublists :: Int -> [a] -> [[a]]
sublists s xs = map (map (\xi -> fst xi))
                $ filter (\ys -> length ys == s)
                $ foldr (\(_, i) acc -> (subl i) : acc) [] zxs
  where
    subl i = takeWhile (\(_, j) -> j < i + s)
               $ snd $ splitAt i zxs
    zxs = zip xs [0..]

-- Largest product of s consecutive digits in n
largeProd :: Int -> String -> Int 
largeProd s num
  | (s > length num) = error "Product size denied"
  | otherwise 
    = maximum $ map (\xs -> product xs) $ sublists s num'
  where num' = map digitToInt num

main :: IO ()
main = do
  str <- readFile "number.txt" 
  print $ largeProd 13 $ concat . lines $ str
