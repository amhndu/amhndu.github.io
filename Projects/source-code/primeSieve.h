#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>

using std::size_t;

unsigned int ctz(size_t v)//count trailing zeros
{
    //Taken from *Bit Twiddling Hacks* at http://graphics.stanford.edu/~seander/bithacks.html
    unsigned int c;
    // NOTE: if 0 == v, then c = 31.
    if (v & 0x1) { c = 0; }
    else
    {
        c = 1;
        if ((v & 0xffff) == 0)
        {
            v >>= 16;
            c += 16;
        }
        if ((v & 0xff) == 0)
        {
            v >>= 8;
            c += 8;
        }
        if ((v & 0xf) == 0)
        {
            v >>= 4;
            c += 4;
        }
        if ((v & 0x3) == 0)
        {
            v >>= 2;
            c += 2;
        }
        c -= v & 0x1;
    }
    return c;
}

template<size_t to>
class PrimeSieve
{
    constexpr static size_t bits_per_word = 32;//can be sizeof(size_t)*8 but the ctz() function assumes 32 bit
    static_assert(sizeof(size_t) * 8 >= bits_per_word , "Word (size_t) size is smaller than 4 , unsupported");

    std::vector<size_t> sieve;

    size_t inline NtoI(size_t n)  const { return (n + 1) >> 1;} //Number to index in sieve
    size_t inline ItoN(size_t in) const { return (in << 1) - 1;} //Sieve index to number
public:

    //Constructor fills the sieve up to the `to` template argument
    //Derivation of sieve size :
    //The sieve only stores odd numbers,
    //if `to` is odd we add 1 to it, then divide it by 2
    //Dividing the above result by `bits_per_word`, we get the required number of words,
    //Add 1 to the size if there is any remainder from the above division.
    //So, `size = (to+to%2)/(2*bits_per_word)+((to+to%2)%(2*bits_per_word)!=0)`
    //But due to the way we map a number to its index, we also have a "zero" number at index 0,
    //So, add 1 to the `to` in the above formula
    //thus size = ((to+1)+(to+1)%2)/(2*bits_per_word)+(((to+1)+(to+1)%2)%(2*bits_per_word)!=0)
    //which can be simplified as ,
    //size = ((to-to%2)/2+1)/bits_per_word + ( ((to-to%2)/2+1)%bits_per_word!=0 )
    PrimeSieve()	:
        sieve( ((to - to % 2) / 2 + 1) / bits_per_word + ( ((to - to % 2) / 2 + 1) % bits_per_word != 0 ) , 0xffffffff)
    {
        sieve[0] = ~3;  //mark 0 and 1 as composites
        size_t to_sqrt = std::sqrt(to);
        size_t to_in = NtoI(to);    //The last number's index in the sieve
        for(size_t i = 3; i <= to_sqrt ; i = nextPrime(i))
        {
            //if we add i to the reduced j, we are adding 2*i to ItoN(j), but j is odd, thus skipping any even multiple
            for(size_t j = NtoI(i * i) ; j <= to_in ; j += i)
            {
                sieve[j / bits_per_word] &= ~(1 << (j % bits_per_word));    //set the bit at (j%bits_per_word) as 0
            }
        }
        sieve.back() &= (1 << (to_in % bits_per_word + 1)) - 1;
    }

    ///return true if n is prime, false otherwise
    bool isprime(size_t n) const
    {
        if( (n % 2) == 0 )
        {
            if( n != 2)
            {
                return false;
            }
            return true;
        }
        size_t i = NtoI(n);
        if( i / bits_per_word >= sieve.size() )
        {
            throw std::range_error("error : n = " + std::to_string(i) + " is >= max prime stored in sieve = " + std::to_string(to));
        }
        return sieve[i / bits_per_word] & (1 << (i % bits_per_word));
    }

    ///Same as isprime() but doesn't check if n is within bounds
    bool operator[](size_t n) const
    {
        if( (n % 2) == 0 )
        {
            if( n != 2)
            {
                return false;
            }
            return true;
        }
        size_t i = NtoI(n);
        return sieve[i / bits_per_word] & (1 << (i % bits_per_word));
    }

    ///returns the next prime number after a,
    ///if no other primes exist in the sieve currently then
    ///returns PrimeSieve::npos
    size_t inline nextPrime(size_t a) const
    {
        //if a is not even then add one to it,
        //then divide 'a' by 2
        if(a < 2)
        {
            return 2;
        }
        a = ((a + (a % 2)) >> 1);
        size_t i = a / bits_per_word;
        size_t r = a % bits_per_word;
        size_t word = sieve[i];
        word >>= ++r;
        if(word && r != bits_per_word) //i.e. if the word at sieve[i] has any more bits set after offset r
        {
            r += ctz(word);
            return ItoN(i * bits_per_word + r);
        }
        else
        {
            ++i;
            for(; i < sieve.size(); ++i)
            {
                if(sieve[i])
                {
                    r = ctz(sieve[i]);
                    return ItoN(i * bits_per_word + r);
                }
            }
        }
        return npos;
    }

    ///The size of the sieve
    size_t constexpr inline size() const
    {
        return to;
    }
    static constexpr size_t npos = -1;
};
