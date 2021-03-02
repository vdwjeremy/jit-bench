#include <iostream>
#include <sstream>
using namespace std;

bool isPrime(uint32_t N) 
{
    for (uint32_t j = 2; j <= N / 2; ++j) {
        if (N % j == 0) {
            return false;
        }
    }
    return true;
}

// Function to print first N prime numbers
uint32_t findPrimes(uint32_t N)
{
    //asm("mov r8, #0x99");
    uint32_t primes[100000];

    // Declare the variables
    uint32_t i, j, flag;
    uint32_t* result = primes;
 
    // Traverse each number from 1 to N
    // with the help of for loop
    for (i = 1; i <= N; i++) {
 
        // Skip 0 and 1 as they are
        // niether prime nor composite
        if (i == 1 || i == 0)
            continue;
 
        if (isPrime(i))
            *(result++) = i;
    }

    return result - primes;
}
  
/* Driver code */
int main(int argc, char *argv[])  
{  
    int n = 0;
    istringstream(argv[1]) >> n;
    cout << "Found " << findPrimes(n) << " primes" << endl;
    return 0;  
}  

