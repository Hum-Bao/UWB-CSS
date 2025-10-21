echo "Test X: (shortest distance, number of paths)"
echo "TEST 1: (2,2)"
./greedy_robot 1 3 4 4 3
echo "TEST 2: (Already there)"
./greedy_robot 2 1 1 1 1
echo "TEST 3: (NP, 0)"
./greedy_robot 1 1 1 5 1
echo "TEST 4: (18, 2)"
./greedy_robot 1 10 10 1 1
echo "TEST 5: (18, 3254)"
./greedy_robot 2 10 10 1 1
echo "Test 6: (5, 10)"
./greedy_robot 1000 1000 1000 998 997
echo "Test 7: (16, 10432)"
./greedy_robot 5 -1 -1 -10 6
