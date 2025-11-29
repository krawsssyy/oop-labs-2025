#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

class StringAsNumber {
    private:
        char* number;

        bool isNegative() const { // helper for checking if a number is negative
            return this->number[0] == '-';
        }

        char* digits() const { // helper for getting the actual digits of a number
            char* tmp = nullptr;
            int off = 0;
            if (this->isNegative()) {
                off = 1;
            }
            tmp = new char[strlen(this->number) + 1 - off];
            strcpy(tmp, this->number + off);
            return tmp;
        }

        void normalize() { // helper for removing leading '0's from a string
            bool negative = this->isNegative();
            char* digits = this->digits();

            int length = strlen(digits);
            int start = 0;
            // get length of leading '0's
            while (start < length - 1 && digits[start] == '0') {
                start++;
            }

            int newLength = length - start;
            int total = newLength + (negative ? 1 : 0) + 1; // calculate total length

            char* tmp = new char[total];
            int pos = 0;

            if (negative) {
                tmp[pos] = '-';
                pos++;
            }
            // copy remaining digits
            for (int i = start; i < length; i++) {
                tmp[pos] = digits[i];
                pos++;
            }

            tmp[pos] = '\0'; // null-terminate the new string
            // make this tmp as our new number
            delete[] this->number;
            delete[] digits; // cleanup the memory allocated by "digits"
            digits = nullptr;
            this->number = tmp;

            // sanity check
            if (strcmp(this->number, "-0") == 0 || strcmp(this->number, "0") == 0) {
                delete[] this->number;
                this->number = new char[2];
                this->number[0] = '0';
                this->number[1] = '\0';
            }
        }

        int compareAbs(const char* lhs, const char* rhs) const {
            // compare in absolute values (used only on buffers which have their '-' removed via "digits" function)
            int lenLhs = strlen(lhs);
            int lenRhs = strlen(rhs);
            // check their lengths
            // this is required as strcmp compares strings lexicographically, and if we check "9" and "10", we would get that "9" is bigger than "10"
            // as '9' > '1'
            // therefore, we need to ensure the strings are the same length
            if (lenLhs < lenRhs) {
                return -1;
            }
            if (lenLhs > lenRhs) {
                return 1;
            }
            // if same length, do a pure strcmp
            return strcmp(lhs, rhs);
        }

    public:
        StringAsNumber() { // default constructor
            this->number = new char[2];
            this->number[0] = '0';
            this->number[1] = '\0';
        }

        StringAsNumber(const char* number) { // parametrized constructor
            bool ok = true;

            if (!number || number[0] == '\0') { // if not a valid src array
                ok = false;
            }
            else {
                int len = strlen(number);
                int start = 0;
                bool hasSign = false;

                // check for leading '-'
                if (number[0] == '-') {
                    hasSign = true;
                    start = 1;

                    // string must not be only "-"
                    if (len == 1) {
                        ok = false;
                    }
                }
                for (int i = start; i < strlen(number); i++) {
                    if (number[i] < '0' || number[i] > '9') { // validate that we only get numbers as strings
                        // in the ascii table, we get the digits one after the other, starting at number 48 (0x30), which is '0'
                        // up to 57 (0x39), which is '9'
                        // therefore, anything smaller than 0x30 or bigger than 0x39 would not be a digit
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) {
                this->number = new char[strlen(number) + 1];
                strcpy(this->number, number);
                this->normalize();
            }
            else {
                std::cout << "Invalid number!" << std::endl;
                // default init in this case
                this->number = new char[2];
                this->number[0] = '0';
                this->number[1] = '\0';
            }
        }

        StringAsNumber(const StringAsNumber& other) { // copy constructor
            // no need for size checks since in the other constructors we allocate something in all cases
            this->number = new char[strlen(other.number) + 1];
            strcpy(this->number, other.number);
        }

        ~StringAsNumber() { // destructor
            delete[] this->number;
            this->number = nullptr;
        }

        StringAsNumber& operator=(const StringAsNumber& other) { // copy-assignment operator - marks the rule of three complete
            // as we have all three (copy constructor, destructor and copy-assignment operator)
            if (this != &other) { // guard against self-assignment
                delete[] this->number; // it is safe not to check here, since we allocate this->number in all cases
                this->number = new char[strlen(other.number) + 1];
                strcpy(this->number, other.number);
            }
            return *this;
        }

        bool operator==(const StringAsNumber& rhs) const {
            return strcmp(this->number, rhs.number) == 0;
        }

        bool operator!=(const StringAsNumber& rhs) const {
            return !(*this == rhs);
        }

        bool operator<(const StringAsNumber& rhs) const {
            bool leftNeg = this->isNegative();
            bool rightNeg = rhs.isNegative();

            // simple checks for sign mismatches
            if (leftNeg && !rightNeg) {
                return true;
            }
            if (!leftNeg && rightNeg) {
                return false;
            }

            char* currDigits = this->digits();
            char* rhsDigits = rhs.digits();
            int cmp = this->compareAbs(currDigits, rhsDigits);
            // cleanup for the arrays obtained via "digits()"
            delete[] currDigits;
            currDigits = nullptr;
            delete[] rhsDigits;
            rhsDigits = nullptr;

            // if both are negative
            //      if cmp > 0 <=> leftHandSide is bigger than the rightHandSide
            //          when adding back the negatives, the signs will flip, and therefore leftHandSide will be smaller than rightHandSide
            //          which makes the operator return true, since we have "operator<"
            // if both are positive
            //      if cmp < 0 <=> leftHandSide is smaller than rightHandSide, which makes this operator return true
            return leftNeg && rightNeg ? (cmp > 0 ? true : false) : (cmp < 0 ? true : false);
        }

        bool operator<=(const StringAsNumber& rhs) const {
            return *this < rhs || *this == rhs;
        }

        bool operator>(const StringAsNumber& rhs) const {
            return !(*this <= rhs);
        }

        bool operator>=(const StringAsNumber& rhs) const {
            return !(*this < rhs);
        }

        StringAsNumber operator+(const StringAsNumber& rhs) const {
            bool leftNeg = this->isNegative();
            bool rightNeg = rhs.isNegative();

            // translate additions to their respective subtractions if the signs mismatch
            if (leftNeg && !rightNeg) {
                // -a + b = b - a
                // needed those things here so we don't allocate them and then return, which would leave them allocated and not cleared
                // with nobody to clear them
                char* lhsDigits = this->digits();
                StringAsNumber temp(lhsDigits);
                delete[] lhsDigits;
                lhsDigits = nullptr;
                return rhs - temp;
            }

            if (!leftNeg && rightNeg) {
                // a + (-b) = a - b
                char* rhsDigits = rhs.digits();
                StringAsNumber temp(rhsDigits);
                delete[] rhsDigits;
                rhsDigits = nullptr;
                return *this - temp;
            }

            char* lhsDigits = this->digits();
            char* rhsDigits = rhs.digits();
            int lhsLen = strlen(lhsDigits);
            int rhsLen = strlen(rhsDigits);
            int size = (lhsLen > rhsLen ? lhsLen : rhsLen) + 1; // allocate +1 just in case of carry

            char* result = new char[size + 1];
            result[size] = '\0'; // null-terminate the result
            // start from the end so we don't have to reverse the string at the end
            int lhsPos = lhsLen - 1;
            int rhsPos = rhsLen - 1;
            int resPos = size - 1;
            int carry = 0;

            while (resPos >= 0) {
                // do simple school addition algorithm
                // add each digit with the one from the same position from the other number
                // note down the result, and add the carry to the next addition
                
                // give them default values in case we run out of digits for a number
                int lhsDigit = 0;
                int rhsDigit = 0;

                if (lhsPos >= 0) { // bounds check
                    lhsDigit = lhsDigits[lhsPos] - '0'; // convert to int
                    // for e.g., if we have '7' - '0', that equates to 55 (0x37) - 48 (0x30) which is 7
                }
                if (rhsPos >= 0) {
                    rhsDigit = rhsDigits[rhsPos] - '0';
                }

                int sum = lhsDigit + rhsDigit + carry; // add carry from previous round
                // (in case we have a carry at the end, giving "lhsDigit" and "rhsDigit" default values and allocating the result with size + 1
                // ensures that we get the final value
                carry = sum / 10; // get carry from this round, to be used in the next

                result[resPos] = (sum % 10) + '0'; // convert back to the respective number as a character array
                // move simultaneously
                lhsPos--;
                rhsPos--;
                resPos--;
            }

            StringAsNumber out(result);
            // cleanup
            delete[] result;
            result = nullptr;
            delete[] lhsDigits;
            lhsDigits = nullptr;
            delete[] rhsDigits;
            rhsDigits = nullptr;

            if (leftNeg && rightNeg) { // if both negative, then addition is also negative
                char* tmp = new char[strlen(out.number) + 2];
                tmp[0] = '-';
                strcpy(tmp + 1, out.number);
                delete[] out.number;
                out.number = tmp;
            }
            return out;
        }

        StringAsNumber operator-(const StringAsNumber& rhs) const {
            bool leftNeg = this->isNegative();
            bool rightNeg = rhs.isNegative();

            // translate subtractions to their respective additions if signs mismatch
            if (leftNeg && !rightNeg) {
                char* lhsDigits = this->digits();
                StringAsNumber temp(lhsDigits);
                StringAsNumber res = temp + rhs;

                char* tmp = new char[strlen(res.number) + 2];
                // -a - b = - (a + b)
                tmp[0] = '-';
                strcpy(tmp + 1, res.number);
                delete[] res.number;
                res.number = tmp;
                delete[] lhsDigits;
                lhsDigits = nullptr;

                return res;
            }

            if (!leftNeg && rightNeg) {
                // a - (-b) = a + b
                char* rhsDigits = rhs.digits();
                StringAsNumber temp(rhsDigits);
                StringAsNumber res = *this + temp;
                delete[] rhsDigits;
                rhsDigits = nullptr;
                return res;
            }

            char* lhsDigits = this->digits();
            char* rhsDigits = rhs.digits();
            // simple check for fast exits
            int cmp = this->compareAbs(lhsDigits, rhsDigits);
            if (cmp == 0) {
                // if both negative, i.e. -a - (-a) = -a + a = 0
                // if both positive, i.e. a - a = 0
                // cleanup before return
                delete[] lhsDigits;
                lhsDigits = nullptr;
                delete[] rhsDigits;
                rhsDigits = nullptr;
                return StringAsNumber("0");
            }

            bool negative = false;
            const char* big = lhsDigits;
            const char* small = rhsDigits;
            if (cmp < 0) {
                // swap values in case the result is negative (if a < b <=> a - b < 0)
                negative = true;
                big = rhsDigits;
                small = lhsDigits;
            }

            int bigLen = strlen(big);
            int smallLen = strlen(small);
            char* result = new char[bigLen + 1];
            result[bigLen] = '\0';

            // start again from the end so we don't have to reverse the string at the end
            int bigPos = bigLen - 1;
            int smallPos = smallLen - 1;
            int resPos = bigLen - 1;
            int borrow = 0;
            while (resPos >= 0) {
                // do regular subtraction algorithm
                // subtract each digit from the other one
                // if the result is negative, then borrow one from the next position and then subtract it from there when reaching its calculation
                int bigDigit = big[bigPos] - '0' - borrow;
                int smallDigit = 0;

                if (smallPos >= 0) {
                    smallDigit = small[smallPos] - '0';
                }
                if (bigDigit < smallDigit) {
                    bigDigit += 10;
                    borrow = 1;
                }
                else {
                    borrow = 0;
                }

                result[resPos] = (bigDigit - smallDigit) + '0';
                bigPos--;
                smallPos--;
                resPos--;
            }

            StringAsNumber out(result);
            delete[] result;
            result = nullptr;
            delete[] lhsDigits;
            lhsDigits = nullptr;
            delete[] rhsDigits;
            rhsDigits = nullptr;
            big = small = nullptr; // big = (small = nullptr) <=> do small = nullptr, then do big = small, which also makes big be nullptr
            // also make these ones nullptr, since they are copies of "lhsDigits" and "rhsDigits"
            // not reallocated, just using the same address
            // so we don't de-allocate them since that'd be a double-free, but we just make them null to not leave them dangling

            if (leftNeg && rightNeg) {
                // -a - (-b) = -a + b = b - a = - (a - b)
                negative = !negative;
            }

            if (negative) { // if result is negative
                char* tmp = new char[strlen(out.number) + 2];
                tmp[0] = '-';
                strcpy(tmp + 1, out.number);
                delete[] out.number;
                out.number = tmp;
            }
            return out;
        }

        StringAsNumber operator*(const StringAsNumber& rhs) const {
            // check for multiplication with zero
            if (*this == StringAsNumber("0") || rhs == StringAsNumber("0")) {
                return StringAsNumber("0");
            }

            // check for sign mismatches
            bool negative = false;
            if (this->isNegative() != rhs.isNegative()) {
                negative = true;
            }

            char* lhsDigits = this->digits();
            char* rhsDigits = rhs.digits();
            int lhsLen = strlen(lhsDigits);
            int rhsLen = strlen(rhsDigits);

            // result can have at most lhsLen + rhsLen digits
            char* result = new char[lhsLen + rhsLen + 1];
            memset(result, 48, lhsLen + rhsLen);
            result[lhsLen + rhsLen] = '\0';

            // do regular school digit multiplication
            // multiply digit by digit and then add them in the correct place
            // go in reverse order so we don't have to reverse the string at the end
            // there are multiple algorithms for multiplying and dividing, but we go with simple ones
            for (int i = lhsLen - 1; i >= 0; i--) {
                int carry = 0;
                int lhsDigit = lhsDigits[i] - '0';
                for (int j = rhsLen - 1; j >= 0; j--) {
                    int rhsDigit = rhsDigits[j] - '0';

                    int pos = (lhsLen - 1 - i) + (rhsLen - 1 - j); // current position in lhs + current position in rhs
                    int index = (lhsLen + rhsLen - 1) - pos;

                    int curr = (result[index] - '0') + lhsDigit * rhsDigit + carry;
                    result[index] = (curr % 10) + '0';
                    carry = curr / 10;
                }
                if (carry > 0) {
                    int index = (lhsLen + rhsLen - 1) - ((lhsLen - 1 - i) + rhsLen);
                    result[index] = (result[index] - '0' + carry) + '0';
                }
            }

            StringAsNumber out(result);
            delete[] result;
            result = nullptr;
            delete[] lhsDigits;
            lhsDigits = nullptr;
            delete[] rhsDigits;
            rhsDigits = nullptr;

            if (negative) {
                char* tmp = new char[strlen(out.number) + 2];
                tmp[0] = '-';
                strcpy(tmp + 1, out.number);
                delete[] out.number;
                out.number = tmp;
            }
            return out;
        }


        StringAsNumber operator/(const StringAsNumber& rhs) const {
            // implement it as integer division, as we don't know yet how we can return 2 values beautifully other than using a list
            if (rhs == StringAsNumber("0")) {
                throw "Cannot divide by zero!"; // throw is a special statement, which invokes an error
                // we give an error for this, since we cannot return anything in this case, and we want the program to crash
                // in the future, we'll see how we can catch those and resume gracefully, but for now we don't do any catching
                // and hence division by 0 will crash the program, just as it regularly does
            }
            // simple check for fast exit
            if (*this == StringAsNumber("0")) { // 0 / x = 0
                return StringAsNumber("0");
            }

            // check for sign mismatch
            bool negative = false;
            if (this->isNegative() != rhs.isNegative()) {
                negative = true;
            }

            char* lhsDigits = this->digits();
            char* rhsDigits = rhs.digits();
            StringAsNumber left(lhsDigits);
            StringAsNumber divisor(rhsDigits);
            // cleanup
            delete[] lhsDigits;
            lhsDigits = nullptr;
            delete[] rhsDigits;
            rhsDigits = nullptr;

            StringAsNumber zero("0");
            StringAsNumber ten("10");
            StringAsNumber quotient("0");
            // chunked subtraction
            // for e.g., if we do 216 / 6, we multiply 6 by multiples of 10, until it is bigger, then take the previous value
            // subtract that and repeat, so we chunk and don't do that many subtractions
            while (left >= divisor) {
                StringAsNumber currentDivisor = divisor;
                StringAsNumber multiple("1");
                // multiply divisor by 10 as long as <= leftHandSide
                while (true) {
                    StringAsNumber next = currentDivisor * ten;
                    if (next > left) {
                        break;
                    }
                    currentDivisor = next;
                    multiple = multiple * ten;
                }
                left = left - currentDivisor;
                quotient = quotient + multiple;
            }

            if (negative && quotient != zero) {
                char* tmp = new char[strlen(quotient.number) + 2];
                tmp[0] = '-';
                strcpy(tmp + 1, quotient.number);
                delete[] quotient.number;
                quotient.number = tmp;
            }
            return quotient;
        }

        // implement copy-assigment operators based on the already defined binary operators
        StringAsNumber& operator+=(const StringAsNumber& rhs) {
            *this = *this + rhs;
            return *this;
        }

        StringAsNumber& operator-=(const StringAsNumber& rhs) {
            *this = *this - rhs;
            return *this;
        }

        StringAsNumber& operator*=(const StringAsNumber& rhs) {
            *this = *this * rhs;
            return *this;
        }

        StringAsNumber& operator/=(const StringAsNumber& rhs) {
            *this = *this / rhs;
            return *this;
        }

        void printNumber() const { // small public helper for printing the number
            std::cout << this->number << std::endl;
        }
};

int main() {
    StringAsNumber a("-12373213123112312312312312353536546");
    StringAsNumber b("12398123781182371287381723722");
    std::cout << "a = "; a.printNumber();
    std::cout << "b = "; b.printNumber();
    std::cout << "a + b = "; (a + b).printNumber(); // since these operators return an object, we can do this
    std::cout << "a - b = "; (a - b).printNumber();
    std::cout << "a * b = "; (a * b).printNumber();
    std::cout << "a / b = "; (a / b).printNumber();
    std::cout << "a *= (-1) = "; (a *= StringAsNumber("-1")).printNumber(); // since assignment operators return an object, we can do this
    std::cout << "a /= (-3) = "; (a /= StringAsNumber("-3")).printNumber();
    std::cout << "a += 6 = "; (a += StringAsNumber("6")).printNumber();
    std::cout << "a -= 9 = "; (a -= StringAsNumber("9")).printNumber();
    return 0;
}
