#include "Lucia/Maigui/Types/Container.h"
namespace Lucia {
namespace Maigui {

    void Container::generate(Vertex position,Vertex dimensions,shared_ptr<Skin> sk)
    {
        Name = "Container";
        Position = position;
        Dimensions = dimensions;
        skin = sk;
        //ctor
    };
    // these are relative!!!
    void Container::minimize(){scaleTo(minimumSize.x,minimumSize.y,minimumSize.z);};
    void Container::maximize(){scaleTo(maximumSize.x,maximumSize.y,maximumSize.z);};
    void Container::close(){visible = false;inactive = true;};
} // namespace Maigui
}
