#include <iostream>
#include <sstream>
#include <stdio.h>
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


constexpr uint32_t MAXCOUNT = 30 ;
constexpr uint32_t MAXX = 1024 ;
constexpr uint32_t MAXY = 1024 ;
  
// Function to draw mandelbrot set (N times)
uint32_t fractal(uint32_t N) 
{ 
    uint32_t image[MAXX][MAXY];
    uint32_t count;
    
    for (uint32_t n=0; n<N; ++n) {
        float left = -1.75; 
        float top = -0.25; 
        float xside = 0.25; 
        float yside = 0.45; 

        int x, y, i, j; 
        float xscale, yscale, zx, zy, cx, tempx, cy; 
    
        // setting up the xscale and yscale 
        xscale = xside / MAXX; 
        yscale = yside / MAXY; 
    
        // scanning every point in that rectangular area. 
        // Each point represents a Complex number (x + yi). 
        // Iterate that complex number 
        for (y = 1; y <= MAXY - 1; y++) { 
            for (x = 1; x <= MAXX - 1; x++) 
            { 
                // c_real 
                cx = x * xscale + left; 
    
                // c_imaginary 
                cy = y * yscale + top; 
    
                // z_real 
                zx = 0; 
    
                // z_imaginary 
                zy = 0; 
                count = 0; 
    
                // Calculate whether c(c_real + c_imaginary) belongs 
                // to the Mandelbrot set or not and draw a pixel 
                // at coordinates (x, y) accordingly 
                // If you reach the Maximum number of iterations 
                // and If the distance from the origin is 
                // greater than 2 exit the loop 
                while ((zx * zx + zy * zy < 4) && (count < MAXCOUNT)) 
                { 
                    // Calculate Mandelbrot function 
                    // z = z*z + c where z is a complex number 
    
                    // tempx = z_real*_real - z_imaginary*z_imaginary + c_real 
                    tempx = zx * zx - zy * zy + cx; 
    
                    // 2*z_real*z_imaginary + c_imaginary 
                    zy = 2 * zx * zy + cy; 
    
                    // Updating z_real = tempx 
                    zx = tempx; 
    
                    // Increment count 
                    count = count + 1; 
                } 
    
                // To display the created fractal 
                image[x][y] = count; 
            } 
        }

        // Compute checksum
        count = 0;
        for (y = 1; y <= MAXY - 1; y++) { 
            for (x = 1; x <= MAXX - 1; x++) 
            { 
                count += image[x][y]; 
            }
        }
    }

    return count;
} 

  
/* Driver code */
int main(int argc, char *argv[])  
{  
    std::string benchmark;
    uint32_t n = 0;
    istringstream(argv[1]) >> benchmark;
    istringstream(argv[2]) >> n;
    if (benchmark == "primes")
        cout << "Found " << findPrimes(n) << " primes" << endl;
    else if (benchmark == "fractal")
        cout << "Fractal checksum " << fractal(n) << endl;
    return 0;  
}  

