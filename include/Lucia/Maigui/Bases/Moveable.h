#ifndef MIKUS_LUCIA_MAIGUI_BASES_MOVEABLE_H
#define MIKUS_LUCIA_MAIGUI_BASES_MOVEABLE_H
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maths/Moveable.h>

namespace Lucia {
namespace Maigui
{
    namespace Base
    {
        class Moveable
        {
            public:
                virtual void onMorph(){};
                virtual void onMove(){};
                virtual void onScale(){};
                virtual void onRotate(){};
                virtual void onResize(){};

                std::function<void(float,float,float)>OnScale = [](float x,float y,float z){};
                std::function<void(float,float,float)>OnMove =   [](float x,float y,float z){};
                std::function<void(float,float,float)>OnRotate = [](float x,float y,float z){};
                std::function<bool(float,float)>OnGlobalScale = [](float x,float y){return false;};
                // GET
                virtual Vertex getPosition(){return Position;};
                virtual Vertex getDimensions(){return Dimensions;};
                virtual Vertex getRotation(){return Rotation;};
                virtual Vertex getGlobalPosition(){return globalPosition;};
                virtual Vertex getGlobalDimensions(){return globalDimensions;};
                // SET
                virtual void setParentDimensions(Vertex v)
                {
                    parentWidth = v.x;
                    parentHeight = v.y;
                    needsRefresh = true;
                }
                virtual void setGlobalScale(Vertex s)
                {
                    globalScale = s;
                    needsRefresh = true;
                    positionChanged = true;
                    rotationChanged = true;
                };
                virtual Vertex getGlobalScale()
                {
                    return globalScale;
                };
                //TO's
                virtual float toPrecentage(float val,float outof)
                {
                    return (val/outof);
                };
                virtual Vertex toPrecentage(Vertex val)
                {
                    return Vertex(toPrecentage(val.x,parentWidth),toPrecentage(val.y,parentHeight),val.z);
                };
                virtual void move(float x,float y,float z)
                {
                    if (!fixedPosition)
                    {
                        Position += Vertex(x,y,z);needsRefresh=true;positionChanged=true;
                    }
                };
                virtual void moveTo(float x,float y,float z)
                {
                    if (!fixedPosition)
                    {
                        Position = Vertex(x,y,z);needsRefresh=true;positionChanged=true;
                    }
                };
                virtual void move(Vertex v){move(v.x,v.y,v.z);};
                virtual void moveTo(Vertex v){moveTo(v.x,v.y,v.z);};
                virtual void scale(float x,float y,float z)
                {
                    if (!fixedSize)
                    {
                        Dimensions += Vertex(x,y,z);needsRefresh=true;scaleChanged=true;
                    }
                };
                virtual void scaleTo(float x,float y,float z)
                {
                    if (!fixedSize)
                    {
                        Dimensions = Vertex(x,y,z);needsRefresh=true;scaleChanged=true;
                    }
                };
                virtual void scale(Vertex v){scale(v.x,v.y,v.z);};
                virtual void scaleTo(Vertex v){scaleTo(v.x,v.y,v.z);};
                
                virtual void rotate(float x,float y,float z)
                {
                    if (!fixedRotation)
                    {
                        Rotation += Vertex(x,y,z);needsRefresh=true;rotationChanged=true;
                    }
                };
                virtual void rotateTo(float x,float y,float z)
                {
                    if (!fixedRotation)
                    {
                        Rotation = Vertex(x,y,z);needsRefresh=true;rotationChanged=true;
                    }
                };
                virtual void rotate(Vertex v){rotate(v.x,v.y,v.z);};
                virtual void rotateTo(Vertex v){rotateTo(v.x,v.y,v.z);};
                virtual void refresh()
                {
                    if (needsRefresh)
                    {
                        Translation = Matrix<4>();
                        if (scaleToResolution)
                        {
                            if (!OnGlobalScale(globalScale.x,globalScale.y))
                            {
                                int w =(parentWidth*(1.0f/globalScale.x));
                                int h = (parentHeight*(1.0f/globalScale.y));
                                Vertex pDimensions = toPrecentage(Dimensions);
                                globalDimensions.x = parentWidth*(pDimensions.x*globalScale.x);
                                globalDimensions.y = parentHeight*(pDimensions.y*globalScale.y);
                                globalDimensions.z = Dimensions.z;

                                Vertex pPosition = toPrecentage(Position);
                                globalPosition.x = w*(pPosition.x*globalScale.x);
                                globalPosition.y = h*(pPosition.y*globalScale.y);
                                globalPosition.z = Position.z;
                            };
                        }else
                        {
                            globalDimensions = Dimensions;
                            globalPosition = Position;
                        }
                        Translation.scale(globalDimensions);
                        Translation.rotate(Rotation);
                        Translation.translate(globalPosition);

                        if(positionChanged)
                        {
                            OnMove(Position.x,Position.y,Position.z);
                            onMove();
                        };
                        if (scaleChanged)
                        {
                            OnScale(Dimensions.x,Dimensions.y,Dimensions.z);
                            onScale();
                        };
                        if (rotationChanged)
                        {
                            OnRotate(Rotation.x,Rotation.y,Rotation.z);
                            onRotate();
                        };
                        onMorph();
                        needsRefresh = false;
                    }
                };
            protected:
                float parentWidth=800;
                float parentHeight=600;

                bool scaleToResolution = true;

                bool needsRefresh = true;

                bool fixedPosition = false;
                bool fixedRotation = false;
                bool fixedSize = false;

                bool positionChanged = false;
                bool scaleChanged = false;
                bool rotationChanged = false;

                Matrix<4> Translation;
                //global scaling is the scale from the manager.
                Vertex globalScale=Vertex(1.0f,1.0f,1.0f);

                Vertex Position=Vertex(0.0f,0.0f,0.0f);
                Vertex Rotation=Vertex(0.0f,0.0f,0.0f);
                Vertex Dimensions=Vertex(1,1,0.01);

                Vertex globalPosition=Vertex(0.0f,0.0f,0.0f);
                Vertex globalDimensions=Vertex(1,1,0.01);
        };
    }
}
}
#endif // MIKUS_MAIGUI_BASES_MOVEABLE_H
