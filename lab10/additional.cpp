#include <iostream>
#include <cmath> // for std::sqrt

class SensorDataFixedVector { // sample class for example that would make sense for the operators
    private:
        static const int SIZE = 3;
        double data[SIZE];

    public:
        // simple constructors
        SensorDataFixedVector() {
            for (int i = 0; i < SIZE; i++) {
                this->data[i] = 0.;
            }
        }

        SensorDataFixedVector(double x, double y, double z) {
            this->data[0] = x; 
            this->data[1] = y;
            this->data[2] = z;
        }

        // indexing operators
        double& operator[](int i) { // write access
            if (i < 0 || i >= 3) { // guard against out-of-bounds access
                throw "Invalid index!";
            }
            return this->data[i];
        }

        // explicit cast operators, for converting the class to its magnitude
        explicit operator double() const {
            return std::sqrt(this->data[0] * this->data[0] +
                this->data[1] * this->data[1] +
                this->data[2] * this->data[2]);
        }

        // function call operator
        // scale the data by some value
        void operator()(double factor) {
            for (int i = 0; i < SIZE; ++i) {
                this->data[i] *= factor;
            }
        }

        // ++ preincrement -> increment then return
        SensorDataFixedVector& operator++() {
            for (int i = 0; i < SIZE; ++i) {
                ++this->data[i];
            }
            return *this;
        }

        // postincrement ++ -> return then increment
        SensorDataFixedVector operator++(int) {
            SensorDataFixedVector temp = *this;
            ++(*this);
            return temp;
        }

        // -- predecrement -> decrement then return
        SensorDataFixedVector& operator--() {
            for (int i = 0; i < SIZE; ++i) {
                --this->data[i];
            }
            return *this;
        }

        // postdecrement -- -> return then decrement
        SensorDataFixedVector operator--(int) {
            SensorDataFixedVector temp = *this;
            --(*this);
            return temp;
        }

        // output stream behaviour (how to send it to an output stream, for example std::cout)
        friend std::ostream& operator<<(std::ostream& os, const SensorDataFixedVector& v) {
            os << "[ " << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << " ]";
            return os;
        }

        // input stream behaviour (how to read an object of this type from an input stream, for example std::cin)
        friend std::istream& operator>>(std::istream& is, SensorDataFixedVector& v) {
            return is >> v.data[0] >> v.data[1] >> v.data[2];
        }
};

int main2() {
    SensorDataFixedVector v(1.0, 2.0, 3.0);

    std::cout << "Initial: " << v << std::endl; // operator<<

    v[0] = 10.0; // write access indexing operator
    std::cout << "After v[0] = 10: " << v << std::endl;

    std::cout << "++v: " << ++v << std::endl; // ++ pre-increment
    std::cout << "After ++v: " << v << std::endl;

    std::cout << "v++: " << v++ << std::endl; // post-increment ++
    std::cout << "After v++: " << v << std::endl;

    v(0.5); // scale vector by 0.5 via function call operator
    std::cout << "After v(0.5): " << v << std::endl;

    double length = (double)(v); // explicit cast via custom define cast operator
    std::cout << "Magnitude: " << length << std::endl;

    std::cout << "Enter 3 numbers for the vector: ";
    std::cin >> v; // operator>>
    std::cout << "You entered: " << v << std::endl;

    return 0;
}
