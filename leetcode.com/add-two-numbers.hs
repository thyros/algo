-- You are given two non-empty linked lists representing two non-negative integers. 
-- The digits are stored in reverse order and each of their nodes contain a single digit.
-- Add the two numbers and return it as a linked list.
--
-- You may assume the two numbers do not contain any leading zero, except the number 0 itself.
--
-- Example:
-- 
-- Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
-- Output: 7 -> 0 -> 8
-- Explanation: 342 + 465 = 807.

firstOrZero :: [Integer] -> Integer
firstOrZero [] = 0
firstOrZero (x:_) = x


addTwoNumbersImpl :: [Integer] -> [Integer] -> Integer -> [Integer]
addTwoNumbersImpl [] [] 0 = []
addTwoNumbersImpl [] [] 1 = [1]
addTwoNumbersImpl x y overflow = [current] ++ addTwoNumbersImpl xs ys overflow'
    where
        value = firstOrZero x + firstOrZero y + overflow
        current = value `mod` 10
        overflow' = value `div` 10
        xs = drop 1 x
        ys = drop 1 y


addTwoNumbers :: [Integer] -> [Integer] -> [Integer]
addTwoNumbers x y = addTwoNumbersImpl x y 0
