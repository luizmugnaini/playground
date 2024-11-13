import qualified Data.IntSet as IS

-- Better optimization since it uses sets instead of lists
primesBound :: Int -> [Int]
primesBound b = IS.toList $ cut 3 (IS.singleton 2) IS.empty
  where
    cut :: Int -> IS.IntSet -> IS.IntSet -> IS.IntSet
    cut i primes nonPrimes
      | i >= b = primes -- Reached bound
      | IS.member i nonPrimes = cut (i + 2) primes $ IS.insert i nonPrimes
      | otherwise =     -- Found prime
        cut (i + 2) (IS.insert i primes)
          $ IS.union nonPrimes $ IS.fromAscList [3 * i, 5 * i .. b]

sumBelow :: Int -> Int
sumBelow b = sum $ takeWhile (< b) (primesBound b)
