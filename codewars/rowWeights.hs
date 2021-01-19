module Row where

rowWeights :: [Int] -> [Int]
rowWeights [] = [0, 0]
rowWeights [x] = [x, 0]
rowWeights (x:y:xs) = zipWith (+) [x,y] (rowWeights xs)