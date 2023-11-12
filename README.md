# How to run
To run these programs you just need to run the ./runTest.sh file

## runTest parameters
The runTest program takes 3 parameters:
- The number of points
- The rmax value to use
- The breadth of the space to generate points within

## Example run
### The following is a test run with these parameters
- Points: 100
- Rmax: 20
- Breadth: 50

```
./runTest.sh 100 20 50
Running tests with 100 points, rmax of 20 and max coordinate 50:

C++ time:
real    0m0.761s
user    0m0.652s
sys     0m0.476s

Python time:
real    0m8.118s
user    0m8.262s
sys     0m0.041s

Results:
Outputs match!
```