#ifndef COLLIDER_TOOLS_MIKUS_H
#define COLLIDER_TOOLS_MIKUS_H
#include <Collider/Collider.h>
class Collider::Tools::Vector
{
    public:
        // minimal operators
        std::vector<float> value;
        unsigned int size=0;
        float operator[](unsigned int s)
        {
            return value[s];
        };
        Vector(std::initializer_list<float> V)
        {
            for (auto v: V)
            {
                value.push_back(v);
            };
            size = V.size();
        };
        Vector(std::vector<float> V){
            size = V.size();
            value = V;
        };
        float sum()
        const{
            float val=0;
            for (auto v: value)
            {
                val = val + v;
            }
            return val;
        };
        bool operator == (const Vector& A)const{return sum() == A.sum();};
        bool operator != (const Vector& A)const{return sum() != A.sum();};
        inline Vector operator + (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];};
                if (i < s){n = n + S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator - (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < s){n = value[i];};
                if (i < d){n = n - S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator -(){
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = -value[i];
            };
            return Vector(v);
        };

        inline Vector operator += (const Vector& A)
        {
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i]*2;};
                if (i < s){n = n + S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator -= (const Vector& A)
        {
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i] - value[i];};
                if (i < s){n = n - S[i];};
                v[i] = n;
            };
            return Vector(v);
        };

        inline Vector operator ++ (int s)
        {
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i] + s;
            };
            return Vector(v);
        };
        inline Vector operator -- (int s)
        {
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i] - s;
            };
            return Vector(v);
        };
        inline Vector operator ^ (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = pow(n,S[i]);};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator ^ (int A){
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = pow(value[i],A);
            };
            return Vector(v);
        };

        inline Vector operator % (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);
            std::vector<float> v;
            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = fmod(n,S[i]);};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator % (int A){
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = fmod(value[i],A);
            };
            return Vector(v);
        };

        inline Vector operator / (const Vector& A){
            auto S = A.value;
            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);

            std::vector<float> v;

            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = n/S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator / (int A){
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i]/A;
            };
            return Vector(v);
        };

        inline Vector operator * (const Vector& A){
            auto S = A.value;

            unsigned int s = S.size();
            unsigned int d = value.size();
            unsigned int max = std::max(s,d);

            std::vector<float> v;

            for (unsigned int i=0;i < max;i++)
            {
                float n = 0;
                if (i < d){n = value[i];}else{break;};
                if (i < s){n = n*S[i];};
                v[i] = n;
            };
            return Vector(v);
        };
        inline Vector operator * (float A){
            std::vector<float> v;
            for (unsigned int i=0;i < value.size();i++)
            {
                v[i] = value[i]*A;
            };
            return Vector(v);
        };

        inline bool operator < (const Vector& A)
        const{
            return sum() < A.sum();
        };
        inline bool operator > (const Vector& A)
        const{
            return sum() > A.sum();
        };
        inline bool operator <= (const Vector& A)const{return sum() <= A.sum();};
        inline bool operator >= (const Vector& A)const{return sum() >= A.sum();};

        virtual ~Vector();
};
inline std::ostream& operator<< (std::ostream& os, Collider::Tools::Vector &A){
    os << std::endl << "--Vector[ " << A.size << "]--" << std::endl;
    int no =0;
    for (auto v: A.value)
    {
        if (no%5 == 0)
        {
            os << std::endl;
        };
        os << "[" << no << "] : " << v;
        no++;
    };
    os << "------------" << std::endl;
return os; };
// note here that T = float[T^2] therefore matrix such as mat4x3 is impossible!
template <unsigned int T>
class Collider::Tools::Matrix
{
    public:
        unsigned int size = T;
        float data[T][T] = {{1, 0, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 1}};
        const float* unpack() const{return *data;};
        float* unpack(bool b){return *data;};
        float get(unsigned int column,unsigned int row){return data[column-1][row-1];};
        Collider::Vertex multiplyOut(Collider::Vertex& v) // returns multiplied vertex
        {
            return Collider::Vertex(
                (v.x * data[0][0]) + (v.y * data[1][0]) + (v.z * data[2][0]) + data[3][0],
                (v.x * data[0][1]) + (v.y * data[1][1]) + (v.z * data[2][1]) + data[3][1],
                (v.x * data[0][2]) + (v.y * data[1][2]) + (v.z * data[2][2]) + data[3][2]
            );
        }
        void scale(Tools::Vector A){
            auto V = A.value;
            unsigned int size = V.size();
            for (auto i = 0;i < T; i++)
            {
                for (auto j=0;j < T;j++)
                {
                    if (j < size)
                    {data[i][j] *= V[j];}else{break;};
                };
            };
        };
        void scale(Collider::Vertex A){
            for (auto i = 0;i < T; i++)
            {
                data[i][0] *= A.x;
                data[i][1] *= A.y;
                data[i][2] *= A.z;
            };
        };
        Collider::Vertex getScale()
        {
            return Collider::Vertex(data[0][0],data[1][1],data[2][2]);
        }
        void translate(Tools::Vector A)
        {
            auto V = A.value;
            unsigned int size = V.size();
            for (auto i = 0;i < T; i++)
            {
                for (auto j=0;j < T;j++)
                {
                    if (j < size)
                    {data[i][j] += V[j];}else{break;};
                };
            };
        };
        void translate(Collider::Vertex A)
        {
            data[3][0] += A.x;
            data[3][1] += A.y;
            data[3][2] += A.z;
        };
        // angle should be in radians!
        void rotate(Collider::Vertex A)
        {
            //  maths/code from: http://www.songho.ca/opengl/gl_matrix.html
            rotx(A.x);
            roty(A.y);
            rotz(A.z);
        }
        float DEG2RAD = 3.141593f / 180;
        void rotx(float angle)
        {
            auto m = unpack(true);
            float c = cosf(angle * DEG2RAD);
            float s = sinf(angle * DEG2RAD);
            float m1 = m[1],  m2 = m[2],
                  m5 = m[5],  m6 = m[6],
                  m9 = m[9],  m10= m[10],
                  m13= m[13], m14= m[14];

            m[1] = m1 * c + m2 *-s;
            m[2] = m1 * s + m2 * c;
            m[5] = m5 * c + m6 *-s;
            m[6] = m5 * s + m6 * c;
            m[9] = m9 * c + m10*-s;
            m[10]= m9 * s + m10* c;
            m[13]= m13* c + m14*-s;
            m[14]= m13* s + m14* c;
        };
        void roty(float angle){
            auto m = unpack(true);
            float c = cosf(angle * DEG2RAD);
            float s = sinf(angle * DEG2RAD);
            float m0 = m[0],  m2 = m[2],
                  m4 = m[4],  m6 = m[6],
                  m8 = m[8],  m10= m[10],
                  m12= m[12], m14= m[14];

            m[0] = m0 * c + m2 * s;
            m[2] = m0 *-s + m2 * c;
            m[4] = m4 * c + m6 * s;
            m[6] = m4 *-s + m6 * c;
            m[8] = m8 * c + m10* s;
            m[10]= m8 *-s + m10* c;
            m[12]= m12* c + m14* s;
            m[14]= m12*-s + m14* c;
        };
        void rotz(float angle){
            auto m = unpack(true);
            float c = cosf(angle * DEG2RAD);
            float s = sinf(angle * DEG2RAD);
            float m0 = m[0],  m1 = m[1],
                  m4 = m[4],  m5 = m[5],
                  m8 = m[8],  m9 = m[9],
                  m12= m[12], m13= m[13];

            m[0] = m0 * c + m1 *-s;
            m[1] = m0 * s + m1 * c;
            m[4] = m4 * c + m5 *-s;
            m[5] = m4 * s + m5 * c;
            m[8] = m8 * c + m9 *-s;
            m[9] = m8 * s + m9 * c;
            m[12]= m12* c + m13*-s;
            m[13]= m12* s + m13* c;
        };
        bool operator == (Tools::Matrix<T> &b)
        {
            for (unsigned int i=0;i < T; i++)
            {
                for (unsigned int j=0;i < T; j++)
                {
                    if (data[i][j] == b.data[i][j])
                    {
                        return false;
                    }
                };
            };
            return true;
        };
        bool operator != (Tools::Matrix<T> &b){
            return !(this == b);
        };
};
template <unsigned int T>
inline std::ostream& operator<< (std::ostream& os, const Collider::Tools::Matrix<T> &A){
    os << std::endl << "--Matrix[ " << A.size << "]--" << std::endl;
    os << "| ";
    for (unsigned int column =0;column < A.size;column++){
        os << column << " | ";
    };
    os << std::endl << "|";
    for (unsigned int column =0;column < A.size;column++)
    {
        os << "---|";
    };
    os << std::endl << "| ";
    for (unsigned int column =0;column < A.size;column++){
        for (unsigned int row =0;row < A.size;row++){
            const float* s = A.unpack();
            const float ns = s[(row * T) + column];
            os << ns << " | ";
        };
        os << std::endl << "| ";
    };
    os << "------------" << std::endl;
return os; };
class Collider::Tools::Quaternion
{
public:
    float x=0;
    float y=0;
    float z=0;
    float w=1;
    Quaternion(){};
    Quaternion(float vx,float vy,float vz,float vw){
        x = vx;y = vy;z = vz;w = vw;
    }
    virtual ~Quaternion(){};

    Collider::Vertex multiplyOut(Collider::Vertex v){
        Quaternion Q;
        Q.x = v.x;Q.y = v.y;Q.z = v.z;Q.w = 0;
        Quaternion nQ = *this * Q * this->conjugate();
        return Vertex(nQ.x,nQ.y,nQ.z);
    };
    Quaternion conjugate()
    {
        return Quaternion(-x,-y,-z,-w);
    };
    void fromAxis(float nx,float ny,float nz)
    {
        float vx = nx/2;
        float vy = ny/2;
        float vz = nz/2;
        w = (cos(vx)*cos(vy))*cos(vz) - (sin(vx)*sin(vy))*sin(vz);
        x = (sin(vx)*sin(vy))*cos(vz) + (cos(vx)*cos(vy))*sin(vz);
        y = (sin(vx)*cos(vy))*cos(vz) + (cos(vx)*sin(vy))*sin(vz);
        z = (cos(vx)*sin(vy)*cos(vz)) - (sin(vx)*cos(vy))*sin(vz);
    };
    Quaternion operator* (const Quaternion &A)
    {
        Quaternion Q;
        Q.w = w*A.w - x*A.x - y*A.y - z*A.z;
        Q.x = w*A.x + x*A.w + y*A.z - z*A.y;
        Q.y = w*A.y + y*A.w + z*A.x - x*A.z;
        Q.z = w*A.z + z*A.w + x*A.y - y*A.x;
        return Q;
    };
};

#endif // COLLIDER_TOOLS_MIKUS_H
