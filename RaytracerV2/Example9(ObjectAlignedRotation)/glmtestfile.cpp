#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class vec3
{
public:
    float x, y, z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    vec3 operator*(const glm::mat3& m) const
    {
        return vec3(
            x * m[0][0] + y * m[1][0] + z * m[2][0],
            x * m[0][1] + y * m[1][1] + z * m[2][1],
            x * m[0][2] + y * m[1][2] + z * m[2][2]
        );
    }
};

int main()
{
    // Create a 3x3 matrix
    glm::mat3 matrix(1.0f);
    matrix[0][0] = 2.0f;
    matrix[1][1] = 3.0f;

    // Create a 3D vector
    vec3 vector(1.0f, 2.0f, 3.0f);

    // Multiply the matrix by the vector
    vec3 result = vector * matrix;

    // Print the result
    std::cout << "Result: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;

    return 0;
}
