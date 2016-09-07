#ifndef MIKUS_LUCIA_COLLIDER_CBBOX_H
#define MIKUS_LUCIA_COLLIDER_CBBOX_H
#include <Lucia/Collider/Collider.h>
namespace Lucia{
    namespace Collider{
        class BoundingBox
        {
            public:
                BoundingBox();
                BoundingBox(float x,float y,float z,float w,float h,float d);
                bool collidesWith(BoundingBox *Box);
                bool engulfs(BoundingBox *Box);
                bool contains(float x,float y,float z);
                bool contains(Vertex v){return contains(v.x,v.y,v.z);};
                void refresh();
                Vertex getDimensions(); // width,height,depth
                Vertex getMaxDimensions(); // width height and depth and rotation.
                virtual ~BoundingBox();

                std::vector<Vertex> Points = std::vector<Vertex>();
                std::vector<Vertex> Faces = std::vector<Vertex>();
                Matrix<4> Transformation = Matrix<4>();
                
                Vertex Center = Vertex(0,0,0);
                Vertex Min = Vertex(0,0,0);
                Vertex Max = Vertex(0,0,0);
                Vertex Dimensions = Vertex(1,1,1);
        };
    }
}
#endif