#include "Point.hpp"

class Quaternion
{
public:
    static Quaternion Euler(float x, float y, float z);

    float x, y, z, w;

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(const Quaternion& other);
    ~Quaternion();

    Quaternion normalize() const;
    Quaternion inverse() const;
    Quaternion conjugate() const;
    
    Quaternion operator*(const Quaternion& q) const;
    Quaternion operator*(float s) const;
    Quaternion operator+(const Quaternion& q) const;
    Quaternion operator-(const Quaternion& q) const;
    Quaternion operator-() const;

    Vec3f operator*(const Vec3f& v) const;
    Point operator*(const Point& p) const;
};
