-- Complete the solution so that the function will break up camel casing, using a space between words.
-- 
-- Example
-- solution("camelCasing")  ==  "camel Casing"

import Data.Char

solution :: String -> String
solution [] = []
solution (x:xs) = (x:) . foldr (\e acc -> if isUpper e then ' ':e:acc else e:acc) [] $ xs