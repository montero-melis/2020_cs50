#include <cs50.h>
#include <stdio.h>


long long int e10(int k);
int count_digits(long long int n);
int get_last_digit(long long int n);
int get_kth_digit(long long int n, int k);
int sum_digits(int n);
int sum_odds_or_evens(long long int ccn, bool odd);
bool is_valid_checksum(long long int ccn);
bool is_valid_length(long long int ccn);
int get_k_first_digits(long long int ccn, int k);
string get_card_type(long long int ccn);


int main(void)
{
    //initialize return value
    string out = "INVALID\n";
    
    //prompt user for a credit card number
    long long int ccn = get_long("A credit card number:");

    //check length
    bool valid_length = is_valid_length(ccn);

    //do all the necessary checks
    if (valid_length)
    {
        int le = count_digits(ccn);
        bool valid_checksum = is_valid_checksum(ccn);

        if (valid_checksum)
        {
            out = get_card_type(ccn);
        }
    }

    //finish program by printing out the card type
    printf("%s", out);
}


//DEFINE ALL THE FUNCTIONS

//kth power of 10
long long int e10(int k)
{
    long long int base = 1;
    for (int i = 0; i < k; i++)
    {
        base *= 10;
    }
    return base;
}

//function counts digits (NB: for n <= 0 return value is 0)
int count_digits(long long int n)
{
    //initialize to 0
    int nb_digits = 0;

    //count digits
    //treat 0 as special case
    if (n == 0)
    {
        nb_digits = 1;
    } 
    else
    {
        long long int base;
        long long int quot = 1;
        while (quot > 0)
        {
            base = e10(nb_digits + 1);
            quot = n / base;
            nb_digits += 1;
            //printf("n = %lli, base = %lli, quot = %lli, nb_digits = %d\n", n, base, quot, nb_digits);
        }
    }
    return nb_digits;
}

//gets the last digit of a positive integer n
int get_last_digit(long long int n)
{
    int m = n % 10;
    return m;
}


//gets the kth digit of a positive integer n, counting from last
int get_kth_digit(long long int n, int k)
{
    long long int base = e10(k - 1);
    long long int quot = n / base;
    int nth_digit = get_last_digit(quot);
    return nth_digit;
}

//sums over all digits of a k-digit number
int sum_digits(int n)
{
    int nb_digits = count_digits(n);
    int mysum;
    if (nb_digits == 0)
    {
        mysum = 0;
    } 
    else if (nb_digits == 1)
    {
        mysum = n;
    }
    else
    {
        mysum = 0;
        for (int i = 0; i < nb_digits; i++)
        {
            mysum = mysum + get_kth_digit(n, i + 1);
        }
    }
    return mysum;
}

//sum of every other digit; if odd, starting from last, else from next-to-last
int sum_odds_or_evens(long long int ccn, bool odd)
{
    int le = count_digits(ccn);
    int curr_digit;
    int mysum = 0;
    int k = odd ? 1 : 2;
    //printf("odd is %d.\n", odd);
    for (; k <= le; k += 2)
    {
        curr_digit = get_kth_digit(ccn, k);
        int curr_sum = odd ? curr_digit : sum_digits(2 * curr_digit);
        mysum = mysum + curr_sum;
        //printf("Current digit: %d; curr_sum: %d; mysum: %d.\n", curr_digit, curr_sum, mysum);
    }
    return mysum;
}

// perform checksum: returns 1 if valid, 0 otherwise
bool is_valid_checksum(long long int ccn)
{
    int sum_of_odds  = sum_odds_or_evens(ccn, true);
    int sum_of_evens = sum_odds_or_evens(ccn, false);
    int checksum = (sum_of_evens + sum_of_odds);
    bool valid_checksum = false;
    if (checksum % 10 == 0) 
    {
        valid_checksum = true;
    }
    //string validity = valid_checksum ? "valid" : "invalid";
    //printf("checksum is %d; it is %s\n", checksum, validity);
    return valid_checksum;
}

//check if length is valid
bool is_valid_length(long long int ccn)
{
    int le = count_digits(ccn);
    //printf("length %d\n", le);
    bool valid;
    if (le == 13 | le == 15 | le == 16)
    {
        valid = true;
    }
    else
    {
        valid = false;
    }
    return valid;
}

//get the k first digits of a number
int get_k_first_digits(long long int ccn, int k)
{
    int length = count_digits(ccn);
    long long int base = e10(length - k);
    long long int first_k = ccn / base;
    //printf("ccn is %lld\nFirst %d digits are %lld\n", ccn, k, first_k);
    return first_k;
}

// //identify card type
string get_card_type(long long int ccn)
{
    string card_type = "INVALID\n";
    int length = count_digits(ccn);
    int first    = get_k_first_digits(ccn, 1);
    int first2 = get_k_first_digits(ccn, 2);

    //Check for AMEX
    if (first2 == 34 | first2 == 37)
    {
        if (length == 15)
        {
            card_type = "AMEX\n";
        }
    }
    //Check for MC
    else if (first2 == 51 | first2 == 52 | first2 == 53 | first2 == 54 | first2 == 55)
    {
        if (length == 16)
        {
            card_type = "MASTERCARD\n";
        }
    }
    //Check for VISA
    else if (first == 4)
    {
        printf("first is %d\n", first);
        if (length == 13 | length == 16)
        {
            card_type = "VISA\n";
        }
    }
    return card_type;
}
