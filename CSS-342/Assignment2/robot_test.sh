echo "Test X: (shortest distance, number of paths)"

echo "Test 1: (2, 2)"
./greedy_robot 1 3 4 4 3

echo "Test 2: (Already there)"
./greedy_robot 2 1 1 1 1

echo "Test 3: (NP, 0)"
./greedy_robot 1 1 1 5 1

echo "Test 4: (4, 2)"
./greedy_robot 2 1 3 -2 4

echo "Test 5: (5, 7)"
./greedy_robot 2 1 2 3 5

echo "Test 6: (7, 1)"
./greedy_robot 1 3 5 7 2

echo "Test 7: (5, 10)"
./greedy_robot 3 1 2 3 5

echo "Test 8: (5, 1)"
./greedy_robot 1 2 3 5 1

echo "Test 9: (4, 6)"
./greedy_robot 2 1 1 3 3

echo "Test 10: (NP, 0)"
./greedy_robot 1 1 1 4 2

echo "Test 11: (4, 2)"
./greedy_robot 1 -3 -3 -1 -1

echo "Test 12: (3, 3)"
./greedy_robot 2 -1 1 -3 0

echo "Test 13: (NP, 0)"
./greedy_robot 1 -1 -1 -4 -2

echo "Test 14: (6, 20)"
./greedy_robot 3 -2 -2 1 1

echo "Test 15: (2, 2)"
./greedy_robot 2 -1 1 -2 0

echo "Test 16: (4, 2)"
./greedy_robot 1 2 -2 0 0

echo "Test 17: (5, 7)"
./greedy_robot 2 0 0 3 2

echo "Test 18: (4, 4)"
./greedy_robot 3 1 -1 2 2

echo "Test 19: (2, 2)"
./greedy_robot 2 1 -1 2 0

echo "Test 20: (Invalid input)"
./greedy_robot 1 1 1 1

echo "Test 21: (Invalid input)"
./greedy_robot 1 1 1 1 1 1

echo "Test 22: (6,14)"
./greedy_robot 2 1 3 4 6

echo "Test 23: (16,2)"
./greedy_robot 1 3 4 11 12