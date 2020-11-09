-- Check to see if a string has the same amount of 'x's and 'o's. The method must return a boolean and be case insensitive. The string can contain any char.
-- 
-- Examples input/output:
-- 
-- XO("ooxx") => true
-- XO("xooxx") => false
-- XO("ooxXm") => true
-- XO("zpzpzpp") => true // when no 'x' and 'o' is present should return true
-- XO("zzoo") => false


increment :: Char -> (Int, Int) -> (Int, Int)
increment 'x' (a,b) = (a+1, b)
increment 'o' (a,b) = (a, b+1)
increment 'X' (a,b) = (a+1, b)
increment 'O' (a,b) = (a, b+1)
increment _ (a,b) = (a,b)

count :: String -> (Int, Int)
count str = foldr increment (0,0) str

xo :: String -> Bool
xo str = x == y
    where (x,y) = count str
