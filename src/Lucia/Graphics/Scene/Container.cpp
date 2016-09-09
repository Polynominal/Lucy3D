#include "Lucia\Graphics\Scene\Container.h"
#include <Lucia\Graphics\Scene\Object.h>
using namespace Lucia;
using namespace Graphics;
using namespace Scene;

Object* Container::getCore(){return Core.get();};
void Container::removeFromScene(){scene->remove(this);};
void Container::setScene(Graphics::Scene::Instance*s){scene =s;};
void Container::onMorph(){if (Core.get()){Core->onMorph(this);};};
void Container::onMove(){if (Core.get()){Core->onMove(this);};};
void Container::onScale(){if (Core.get()){Core->onScale(this);};};
void Container::onRotate(){if (Core.get()){Core->onRotate(this);};};
void Container::updateInstance(){if (Core.get()){Core->updateInstance(this);};};