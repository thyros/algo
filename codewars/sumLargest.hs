-- Given a list of n (n > 1) positive integers,
-- return the sum of two largest

-- Example: [1,2,3]     -> 2 + 3 = 5
--          [1,8,2,6,3] -> 8 + 6 = 14

sumTwoLargest :: Eq a => [a] -> a
sumTwoLargest = foldr 
        where 