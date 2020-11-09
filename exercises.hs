-- check if element exist in the list
el :: (Eq a) => a -> [a] -> Bool
el _ [] = False
el a (x:xs) = a == x || el a xs

-- remove duplications from the list
nu :: (Eq a) => [a] -> [a]
nu [] = []
nu (x:xs)
    | x `elem` xs = nu xs
    | otherwise = x : nu xs

-- checks if an array is of ascending order
isAsc :: (Ord a) => [a] -> Bool
isAsc [] = True
isAsc [_] = True
isAsc (x:y:xs) = x < y && isAsc (y:xs)


-- checks if there is a path between nodes in a directed graph
-- p = [(1,2), (2,3), (3,2), (4,3), (4,5)]
-- hasPath p 1 3 == True
-- hasPath p 4 2 == True
-- hasPath p 4 1 == False
hasPath :: [(Int, Int)] -> Int -> Int -> Bool
hasPath [] x y = x == y
hasPath xs x y
    | x == y = True
    | otherwise = 
        let xs' = [ (n,m) | (n,m) <- xs, n /= x] in 
            or [ hasPath xs' m y | (n,m) <- xs, n == x]
