// Copyright (c) 2016-2019, Myriota Pty Ltd, All Rights Reserved
// SPDX-License-Identifier: BSD-3-Clause-Attribution
//
// This file is licensed under the BSD with attribution  (the "License"); you
// may not use these files except in compliance with the License.
//
// You may obtain a copy of the License here:
// LICENSE-BSD-3-Clause-Attribution.txt and at
// https://spdx.org/licenses/BSD-3-Clause-Attribution.html
//
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MYRIOTA_MATH_H
#define MYRIOTA_MATH_H

#include <complex.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// double, float, etc. Default is double.
// define MYRIOTA_DECIMAL at compile time if you want to change this.
#ifndef MYRIOTA_DECIMAL
#define MYRIOTA_DECIMAL double
#endif

// Force a compilation error if condition is true
#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2 * !!(condition)]))
#endif

#define field_size(type, member) sizeof(((type *)0)->member)

// Get number of element in an array
#define NUM_ELEMS(array) (sizeof(array) / sizeof((array)[0]))

// Myriota's floating point and complex floating point types
typedef MYRIOTA_DECIMAL myriota_decimal;
typedef MYRIOTA_DECIMAL _Complex myriota_complex;

// Approximation of the irrational number
#define pi 3.14159265358979323846

// square root of 2.
#define sqrt2 1.41421356237

// square root of 2 divided by 2.
// equivalently the reciprocal of sqrt(2.0)
#define sqrt2on2 (sqrt2 / 2.0)

#define myriota_free(x) (free((void *)x))

#ifdef __cplusplus
extern "C" {
#endif

// Print error function, line, message and then exit
#ifndef error_message_and_exit
#define error_message_and_exit(message)                            \
  do {                                                             \
    fprintf(stderr, "!!%s %d: %s\n", __func__, __LINE__, message); \
    fflush(stderr);                                                \
    while (1) {                                                    \
      exit(EXIT_FAILURE);                                          \
    }                                                              \
  } while (0)
#endif

// The factional part of a number
static inline double myriota_frac(double x) { return x - floor(x); }

// The sign of x, zero if x is zero.
static inline double myriota_signum(double x) { return (x > 0) - (x < 0); }

// Square of x, i.e., x*x
static inline double myriota_sqr(double x) { return x * x; }

// Cube of x, i.e., x*x*x
static inline double myriota_cub(double x) { return x * x * x; }

// Floating point x mod y
double myriota_modulus(const double x, const double y);

//  x mod 2pi into the interval [0, 2pi]
static inline double myriota_mod_2pi(const double x) {
  return myriota_modulus(x, 2 * pi);
}

// Returns smallest power of 2 greater than or equal to this integer
unsigned int myriota_greater_power_of_two(unsigned int x);
uint64_t myriota_greater_power_of_two_long(uint64_t x);

// Returns 1 if x is a power of 2, zero otherwise
static inline int myriota_is_power_of_two(unsigned int x) {
  return myriota_greater_power_of_two(x) == x;
}
static inline int myriota_is_power_of_two_long(uint64_t x) {
  return myriota_greater_power_of_two_long(x) == x;
}

// Get the nth bit from buffer x
unsigned int myriota_get_bit(const unsigned int n, const uint8_t *x);

//  Set the value of the nth bit of buffer x
void myriota_set_bit(const unsigned int n, const unsigned int value,
                     uint8_t *x);

// write from[0:end-start+1] into to[start,end]
void myriota_write_bits(const uint8_t *from, uint8_t *to,
                        const unsigned int start, const unsigned int stop);

// read from[start:end] into to[0,end-start+1]
void myriota_read_bits(const uint8_t *from, uint8_t *to,
                       const unsigned int start, const unsigned int stop);

// Returns a complex number from rectangular coordinates, i.e.
// from real and imaginary parts.
myriota_complex myriota_rectangular(myriota_decimal re, myriota_decimal im);

// Complex number from magnitude and phase in radians
myriota_complex myriota_polar(myriota_decimal magnitude, myriota_decimal phase);

// Norm (magnitude squared) of a complex number
myriota_decimal myriota_complex_norm(myriota_complex x);

// Sinc function
double myriota_sinc(double t);

// Factorial of an integer n*(n-1)*(n-2)*...*1
unsigned long myriota_factorial(unsigned int n);

// Returns the value of the nth modified Bessel function at x.
double myriota_besselI(int n, double x);

// Returns true if a string contains only hexidecimal characters
bool myriota_is_hex(const char *s);

// Converts two hexidecimal characters to byte, i.e number in the
// interval [0, 255].
// Returns 2 (the number of characters scanned) on success and 0 if
// either character is not hexidecimal
int myriota_hex_to_byte(const char *h, uint8_t *b);

// Parses a hexidecimal string into a buffer of bytes.
// Returns the number of hexidecimal digits parsed and zero
// if scan failed, i.e. if the string is not valid hex.
// The string must contain an even number of character or
// 0 is returned and no parsing takes place
int myriota_hex_to_buf(const char *s, void *buf);

// Like myriota_hex_to_buf but scans at most n characters
// The smaller of n and strlen(s) must be even or 0 is returned
int myriota_n_hex_to_buf(const char *s, const size_t n, void *buf);

// Writes buffer in hexidecimal format to string. s should been
// allocated with size at least 2*buf_size+1. Returns number of hexidecimal
// characters written
int myriota_buf_to_hex(const void *buf, const size_t buf_size, char *s);

// Print buffer to standard out in hex format
void myriota_print_hex(const void *buf, int size_bytes);

// Parses a base64 string into a buffer of bytes.
// The length of s (i.e. strlen(s)) must be a multiple of 4. Returns the number
// of base64 characters parsed and zero if scan failed, i.e. if the string is
// not valid base64.
int myriota_base64_to_buf(const char *s, void *buf);

// Like myriota_base64_to_buf but scans precisely n characters.
// Returns the number of bytes written to buf
int myriota_n_base64_to_buf(const char *s, const size_t n, void *buf);

// Writes buffer in base64 format to string. s should be allocated with size at
// least buf_size*4/3+1. buf_size must be a multiple of 3. Returns number of
// hexidecimal characters written.
int myriota_buf_to_base64(const void *buf, const size_t buf_size, char *s);

// Parses a zbase32 string into a buffer of bytes.
// buf should be of size at least strlen(s)*5/8+1. Returns the number
// of zbase32 characters parsed and zero if scan failed, i.e. if the string is
// not valid zbase32.
int myriota_zbase32_to_buf(const char *s, void *buf);

// Like myriota_zbase32_to_buf but scans precisely n characters.
// Returns the number of bytes written to buf
int myriota_n_zbase32_to_buf(const char *s, const size_t n, void *buf);

// Writes buffer in zbase32 format to string. buf_size must be a multiple of 5
// or -1 is returned. s should be allocated with size at least buf_size*8/5+1.
// Returns number of zbase32 characters written.
int myriota_buf_to_zbase32(const void *buf, const size_t buf_size, char *s);

// Standard 32-bit cyclic redudunacy check.
uint32_t myriota_crc32(const void *data, size_t length, uint32_t offset);

// Returns the (centered) fractional part of x
//
// This is x minus the nearest integer to x. Equivalently
// the cosest representative of x from [-0.5,0.5) of the
// quotient group R/Z.
static inline double myriota_fracpart(double x) { return x - round(x); }

// Returns x rounded to the nearest multiple of s
static inline double myriota_round_scaled(double x, double s) {
  return s * round(x / s);
}

// Returns x rounded to the nearest number of the form ks + t where k is an
// integer
static inline double myriota_round_scaled_affine(double x, double s, double t) {
  return myriota_round_scaled(x - t, s) + t;
}

// Returns x modulo s into the interval [-s/2,s/2)
//
// Equivalently the cosest representative of x from [-s/2,s/2) of the
// quotient group R/sZ.
static inline double myriota_fracpart_scaled(double x, double s) {
  return x - myriota_round_scaled(x, s);
}

static inline double degrees_to_radians(const double x) { return x * pi / 180; }

static inline double radians_to_degrees(const double x) { return x * 180 / pi; }

// minimum of two integers
static inline int myriota_int_min(int a, int b) { return a < b ? a : b; }

// maximum of two integers
static inline int myriota_int_max(int a, int b) { return a > b ? a : b; }

// True if array a is in strictly ascending order
bool myriota_is_strictly_ascending(const int *a, const int size);

// returns minimum element of a vector of integers.
// returns INT_MAX if the vector is empty
int myriota_int_array_min(const int *S, unsigned int numS);

// returns maximum element of a vector of integers.
// returns INT_MIN if the vector is empty
int myriota_int_array_max(const int *S, unsigned int numS);

// returns the index of the minimum element of a vector of integers.
// returns INT_MAX if the vector is empty
int myriota_int_array_arg_min(const int *S, unsigned int numS);

// returns the index of the maximum element of a vector of integers.
// returns INT_MIN if the vector is empty
int myriota_int_array_arg_max(const int *S, unsigned int numS);

// Returns x modulo y, i.e., the coset representative from
// {0,1,...,y-1} of x from the group Z/y.
//
// Different from x % y when x is negative. This is not the same
// as the remainder of x after division by y when x is negative
int myriota_int_mod(int x, int y);
long myriota_long_mod(long x, long y);
int64_t myriota_int64_mod(int64_t x, int64_t y);

// Ceiling after division of nonegative integer a by nonnegative integer b
static inline unsigned int myriota_int_div_ceil(unsigned int a,
                                                unsigned int b) {
  return a / b + (a % b != 0);
}
static inline unsigned long myriota_long_div_ceil(unsigned long a,
                                                  unsigned long b) {
  return a / b + (a % b != 0);
}

// Number uniformly distributed on the interval [0,1]
static inline double myriota_random_uniform() {
  return ((double)rand()) / RAND_MAX;
}

// Generate a Bernoulli observation with success probability p
// That is, returns 1 with probability p and 0 with probabiliy 1-p
int myriota_random_bernoulli(double p);

// Generate a normally distributed random variable.
// Uses the Box Muller method
double myriota_random_normal();

// Generate an exponentially distributed random variable
double myriota_random_exponential(const double mean);

// Computes continued fraction expansion of decimal number of size N.
// Returns upper bound on approximation error.
double myriota_continued_fraction(double x, unsigned int size, int *r);

// Greatest common divisor between two integers
long long gcd(long long a, long long b);

typedef struct {
  long long p;  // numerator
  long long q;  // denominator
} myriota_rational;

// construct rational number equivalent to a/b. Numerator and denominator
// of resulting rational will always be relatively prime.
myriota_rational make_myriota_rational(long long a, long long b);

// The sum of two rational numbers
myriota_rational myriota_rational_sum(myriota_rational a, myriota_rational b);

// Return 1 if a > b, -1 if a < b, and 0 if a == b
int myriota_rational_compare(myriota_rational a, myriota_rational b);

// Return first size best rational approximations to decimal x. Output written
// into b
void myriota_best_approximations(double x, unsigned int size,
                                 myriota_rational *b);

// Finds a rational approximation p/q to x satisfying either |x - p/q| < tol or
// |q| < qmax or is the kth best approximation (convergent) of x
myriota_rational myriota_rational_approximation(double x, double tol, int qmax,
                                                unsigned int k);

// Search for a zero of the function f(x) in the interval [a, b]. Finds a
// solution x such that |x0 - x| < tol where f(x0) = 0. Uses the bisection
// method, only guaranteed to converge if there is a unique zero between a and
// b and f is continuous and sign(f(a)) = -sign(f(b))
//
// Example usage:
//
// double cubic_func(double x, void* d) { return x * x * (x - 1); }
// double x = myriota_bisection(cubic_func, NULL, 0.5, 1.7, 1e-7, 100);
//  x will be 1.
double myriota_bisection(double (*f)(double, void *), void *fdata, double ax,
                         double bx, double tol, unsigned int ITRMAX);

// Finds x such that f(x) = y.
// Uses the bisection method internally.
// The solution must lie within the interval (ax, bx).
double myriota_solve(double (*f)(double, void *), void *fdata, double y,
                     double ax, double bx, double tol, unsigned int ITRMAX);

// Performs a 1-dimensional minimization of the function f. Implements Brent's
// method combining a golden-section search and parabolic interpolation.
//
// The search interval is [ax,cx] and bx must satisfy both ax < bx < cx and
// f(ax) > f(bx) < f(cx). The myriota_brent function terminates when either the
// number of iterations performed reaches max_iterations or the value of *x is
// within tol of the minimiser of f.
//
// Returns the number of iterations performed. The minimum value of f is return
// in *fx and the minimiser in *x.
unsigned int myriota_brent(double (*f)(double, void *), void *fdata,
                           const double ax, const double bx, const double cx,
                           double *fx, double *x, const double tol,
                           const unsigned int max_iterations);

// A function for unwrapping a phase wrapped sequence
// Phase is assumed to reside in the interval [-pi, pi].
//
// This will only work reliably on relatively clean (not much noise) phase
// signals sampled considerably above the Nyquist rate.
//
// The initial previous_value should be zero. Given x the
// unwrapped sequence y is obtained as follows.
//
// y[0] = myriota_unwrap(x[0], 0);
// y[1] = myriota_unwrap(x[1], y[0]);
// y[2] = myriota_unwrap(x[2], y[1]);
// y[3] = myriota_unwrap(x[3], y[2]);
//
// etc. Or in a forloop.
//
// y[0] = myriota_unwrap(x[0], 0);
// for(int n = 1; n < N; n++ )
//    y[n] =  myriota_unwrap(x[n], y[n-1]);
double myriota_unwrap(const double value, const double previous_value);

// Circularly rotate array of integers to the right n times.
// Modifies input array inplace. The array has the size specified by the size
// argument.
//
// Example usage:
// int N = 4;
// int array[4] = {1,2,3,4};
// myriota_rotate(array, N, 1);
// // array is now {2,3,4,1}
void myriota_rotate(int *array, int size, int n);

// Generates an m-sequence of length 2^N-1.
// Returns sequence into argument r
//
// Example Usage:
// int N = 3;
// int M = (1<<N)-1; //M = 2^N-1
// int r[M];
// myriota_msequence(N, r);
// r is now {1,1,0,0,1,0,1}
void myriota_msequence(const int N, int *r);

// Return the kth largest element from the array of size nitems
double myriota_select_double(const int k, double *a, const size_t nitems);
int32_t myriota_select_int32(const int k, int32_t *a, const size_t nitems);

// Returns the median of an array of size nitems
double myriota_median_double(double *a, const size_t nitems);
int32_t myriota_median_int32(int32_t *a, const size_t nitems);

// Returns the discrete Fourier transform of a complex array of length N
// at frequency f in cycles per sample
//
// Usage:
//
// myriota_complex in[4] = {1, 1, 1, 1};
// myriota_complex four, zero;
// four = myriota_discrete_fourier_transform(4,in,0);
// four = myriota_discrete_fourier_transform(4,in,1);
// zero = myriota_discrete_fourier_transform(4,in,0.25);
// zero = myriota_discrete_fourier_transform(4,in,0.5);
myriota_complex myriota_discrete_fourier_transform(const unsigned int N,
                                                   const myriota_complex *in,
                                                   const myriota_decimal f);

// Computes the fast Fourier transform of the complex array of length N and
// returns result into complex array out. N must be a power of 2.
//
// Cooley-Tukey radix-2 decimation in time implementation. Input arrays must be
// zero padded to length a power of 2.
//
// This implementation works inplace by setting out == in.
void myriota_fft(const unsigned int N, const myriota_complex *in,
                 myriota_complex *out);

// Computes the inverse fast Fourier transform of the complex array of length N
// and returns result into complex array out. N must be a power of 2.
//
// Cooley-Tukey radix-2 decimation in time implementation. Input arrays must be
// zero padded to length a power of 2.
//
// This implementation works inplace by setting out == in.
void myriota_inverse_fft(const unsigned int N, const myriota_complex *in,
                         myriota_complex *out);

// Detect sinusoid in given signal x of length N.
//
// Returns estimates of sinusoids frequency on the interval [-0.5, 0.5),
// complex amplitude,  and a confidence that the signals does indeed contain
// this sinusoid.  The confidence is returned as a probability. Values
// near 1 suggesting the precesence of a sinsoid.
//
// This implementation operates inplace on the input x. x will be mutated.
// N need not be a power of 2 but x must be zero padded to the lengh a power of
// two greater than N, i.e., to length myriota_greater_power_of_two(N)
void myriota_detect_sinusoid_inplace(myriota_complex *x, const unsigned int N,
                                     myriota_decimal *frequency,
                                     myriota_complex *amplitude,
                                     myriota_decimal *residual_variance,
                                     myriota_decimal *confidence);

// Multiply MxN matrix A by N by K matrix B producing M by K matrix X.
// Matrices are assumed flattened rowise, that is, one row after the next.
void myriota_matrix_multiply(const int M, const int N, const int K,
                             const double *A, const double *B, double *X);

// Find N by K matrix X such that AX = Y where A is a non-singular N by N matrix
// and Y is an N by K matrix.
int myriota_matrix_solve(const int N, const int K, const double *A,
                         const double *Y, double *X);

// Return transpose of matrix A into B.
void myriota_matrix_transpose(const int M, const int N, const double *A,
                              double *B);

// LUP decomposition of M by N matrix A. Returns M by N matrix L, permutation
// vector p of length M, and N by N matrix U such that permuting the rows of A
// by p results in the matrix LU, that is, results in the multiplication of
// matrix L by matrix U. Requires M >= N. Returns -1 if this is not the case.
// Returns 0 on success.
int myriota_matrix_lu(const int M, const int N, const double *A, double *L,
                      double *U, int *p);

void myriota_matrix_print(const int M, const int N, const double *A, FILE *f);

// Least sequare fit polynomial a[0] + a[1] t + a[2] t^2 + ... a[r-1] t^r of
// order r to data x. Both tand x assumed to be arrays of length N.
void myriota_polyfit(const double *t, const double *x, const int N, const int r,
                     double *a);

// Generic function for type, length, value data structures. These functions
// require two functions, int size(void*) that returns the size (or length) in
// bytes of an element, and int end(void*) that write a terminating value and
// return the length in bytes of the terminating value. It is expected that
// end(NULL) returns the length without writing any bytes and that size(NULL)
// returns 0. The size of the terminating value must be zero as out by the
// size(void*) function. Returns next element in type, length, value sequence.
// Returns NULL if next element does not exist.
void *myriota_tlv_next(const void *tlv, unsigned int (*size)(const void *));
// Append an element to a type length value sequence, Returns -1 on fail, 0 on
// success.
int myriota_tlv_append(void *tlv, const void *a,
                       unsigned int (*size)(const void *), int (*end)(void *));
// Delete an element. Return -1 if the element does not exist, 0 on success.
int myriota_tlv_delete(void *tlv, void *d, unsigned int (*size)(const void *),
                       int (*end)(void *));
// Return total size of sequence
unsigned int myriota_tlv_size(const void *tlv,
                              unsigned int (*size)(const void *));
// Return total number of element
unsigned int myriota_tlv_count(const void *tlv,
                               unsigned int (*size)(const void *));
// Get the ith element in a sequnece. Returns NULL if out of bounds.
void *myriota_tlv_get(int i, const void *tlv,
                      unsigned int (*size)(const void *));
// Find an element in a sequnece. Returns NULL if the element cannot be found.
void *myriota_tlv_find(const void *tlv, unsigned int (*size)(const void *),
                       bool (*find)(const void *, void *), void *find_state);
// Count the number of elements that satisfy a boolean valued function
unsigned int myriota_tlv_count_find(const void *tlv,
                                    unsigned int (*size)(const void *),
                                    bool (*find)(const void *, void *),
                                    void *find_state);
// Find ith element in a sequnece satisfying boolean function. Returns NULL if
// the element cannot be found.
void *myriota_tlv_get_find(int i, const void *tlv,
                           unsigned int (*size)(const void *),
                           bool (*find)(const void *, void *),
                           void *find_state);
// Filter those elements satisfying boolean function f into array of pointers x.
// Assumes x is allocated with enough memory to hold the result, e.g. at least
// myriota_tlv_count_find(tlv, size, find, find_state) elements. Returns the
// number of elements written into x.
int myriota_tlv_filter(const void *tlv, unsigned int (*size)(const void *),
                       bool (*f)(const void *, void *), void *f_state,
                       const void *x[]);
// Read tlv structure from file. If the file ends before the terminating value
// is reached, then the terminating value is added.
// Returns malloc'd structure than must be freed by the caller.
void *myriota_tlv_from_file(FILE *f, int (*end)(void *));

// Like standard qsort but also removes duplicates. Returns the number of unique
// elements.
int myriota_sort_unique(void *base, size_t nitems, size_t size,
                        int (*compar)(const void *, const void *));

#ifdef __cplusplus
}

#include <complex>
#include <stdexcept>
#include <vector>

namespace myriota {

typedef MYRIOTA_DECIMAL decimal;
typedef std::complex<decimal> complex;

// Type generic circular buffer class that supports sequential write and
// random access.
//
// Internally, the size of the buffer is always increased to the power
// of 2 greater than or equal to the requested size. This allows faster
// mod size operations (bitwise AND).
template <typename T>
class CircularBuffer {
 public:
  const unsigned int size;
  const unsigned int mask;

  // Creates a circular buffer and initialises all
  // entries to init parameter.
  CircularBuffer(unsigned int size, T init)
      : size(myriota_greater_power_of_two(size + 1)),
        mask(this->size - 1),
        buf(this->size, init),
        N(0){};

  // Write/push an element to the end of the buffer
  inline void push(const T &elem) {
    buf[N & mask] = elem;
    N++;
  }

  // The total number of elements that have been pushed into this buffer.
  inline uint64_t pushed() const { return N; }

  // The maximum and minimum values of n for which this(n) is valid
  inline int64_t maxn() const { return N - 1; }
  inline int64_t minn() const { return N - size; }

  // Read the nth element of the buffer
  inline const T &operator()(const int64_t n) const { return buf[n & mask]; }

  // Read the nth element of the buffer after index validation. Throws
  // std::out_of_range if the requested element hasn't been pushed yet or
  // is no longer in the buffer. Analogous to std::vector::at.
  const T &at(const int64_t n) const {
    if (n >= minn() && n <= maxn()) return (*this)(n);
    throw std::out_of_range("circluar buffer at " + std::to_string(n) +
                            " outside [" + std::to_string(minn()) + ", " +
                            std::to_string(maxn()) + "]");
  }

  void set(const int64_t n, const T &v) {
    if (n >= minn() && n <= maxn())
      buf[n & mask] = v;
    else
      throw std::out_of_range("circluar buffer set " + std::to_string(n) +
                              " outside [" + std::to_string(minn()) + ", " +
                              std::to_string(maxn()) + "]");
  }

 protected:
  std::vector<T> buf;
  uint64_t N;
};

// Upsample input sequence where in_rate <= out_rate
class Upsampler {
 public:
  const double W;  // window width
  const myriota_rational r;
  const double gamma;
  const int gmin;
  const int gmax;
  // Widow width W can be adjusted, larger is slower, but more accurate
  Upsampler(double in_rate, double out_rate, double W = 30);
  void push(complex x) { a.push(x); };
  int64_t pushed() const { return a.pushed(); }
  complex operator()(int64_t n) const;
  int64_t minn() const { return ceil(gamma * (a.maxn() - a.size + W)); }
  int64_t maxn() const { return floor(gamma * (a.maxn() - 1 - W)); }

 protected:
  CircularBuffer<complex> a;
  std::vector<double> g_buf;
  inline double g(int64_t n) const { return g_buf[n - gmin]; };
};

// Downsample input sequence where in_rate > out_rate
class Downsampler {
 public:
  const double W;  // window width
  const myriota_rational r;
  const double gamma;
  const int gmin;
  const int gmax;
  // Widow width W can be adjusted, larger is slower, but more accurate
  Downsampler(double in_rate, double out_rate, double W = 30);
  void push(complex x) { a.push(x); };
  int64_t pushed() const { return a.pushed(); }
  complex operator()(int64_t n) const;
  int64_t minn() const { return ceil(gamma * (a.maxn() - a.size) + W); }
  int64_t maxn() const { return floor(gamma * (a.maxn() - 1) - W); }

 protected:
  CircularBuffer<complex> a;
  std::vector<double> g_buf;
  inline double g(int64_t n) const { return g_buf[n - gmin]; };
};

// Returns int x modulo int y, i.e., the coset representative from
// {0,1,...,y-1} of x from the group Z/y.
//
// Different from x % y when x is negative. This is not the same
// as the remainder of x after division by y when x is negative
//
// The templated type T must implement the % and + operators, e.g. int64_t.
template <class T>
T mod(const T &x, const T &y) {
  const T t = x % y;
  if (t < 0) return t + y;
  return t;
}
}  // namespace myriota
#endif

#endif
