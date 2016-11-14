#ifndef LUCIA_MAIGUI_EDITOR_PARSER
#define LUCIA_MAIGUI_EDITOR_PARSER
#include <sol.hpp>
#include <Lucia/Maigui/Maigui.h>
#include <Lucia/Maigui/Item.h>
#include <Lucia/Maigui/Manager.h>
#include <Lucia/Maigui/Types/Button.h>
#include <Lucia/Maigui/Types/Container.h>
#include <Lucia/Maigui/Types/Frame.h>
#include <Lucia/Maigui/Types/Widget.h>
namespace Lucia
{
    namespace Maigui
    {
        namespace Editor
        {
            /*
             * format:
             * {{
             *  Widget: 
             *  Widget Name: 
             *  {x y z}
             *  {sx sy sz}
             *  {rx ry rz}
             *  dragAble
             *  visible
             *  inactive
             *  Children = {<widgets>}
             *  -OPTIONAL-
             *  OnCreate
             *  OnClick
             *  OnFocus
             *  OnMove
             * },
             * {...}
             * }
             */
             //We insert a FindItem function to find the item that we need so we can use it functionaly.
             // eg my_item = findItem(my name)
             // my_item.do_something();
            extern std::function<void(Lucia::Maigui::Skin* skin)> applyWrappers;
            inline void setWrappers(std::function<void(Lucia::Maigui::Skin* skin)> w){applyWrappers= w;}
            inline std::shared_ptr<Lucia::Maigui::Skin> parseSkin(sol::table table,std::string& local)
            {
                auto skin = std::make_shared<Lucia::Maigui::Skin>();
                if (not table["image"])
                {
                    LOG << "Warning" << "Skin has no image" << std::endl;
                    return nullptr;
                }else
                {
                    skin->DrawState = [skin](string name,int state)
                    {
                        switch(state)
                        {
                            case Item::FOCUS:
                            {
                                skin->drawColoredRectangle(1.0f,1.0f,1.0f,0.5f);
                                break;
                            }
                            case Item::INACTIVE:
                            {
                                skin->drawColoredRectangle(0.0f,0.0f,0.0f,0.3f);
                                break;
                            }
                        }
                    };
                    std::string image = table["image"].get_or<std::string>("missing");
                    skin->setPath(local + "/" + image);
                }
                for (unsigned int i = 1; i < table.size()+1; i++){
                    auto item = table[i];
                    if (item)
                    {
                        auto name = item["widget"];
                        if (not name)
                        {
                            auto clas = item["class"];
                            if (clas)
                            {
                                name = clas;
                            }
                            else
                            {
                                name = "MISSING!";
                                LOG << "Warning" << " Skin item has no widget name or class" << std::endl;
                            }
                        }
                        auto quad = item["quad"]; 
                        if (quad)
                        {
                            LOG << "Info" << "forming: " << item["widget"].get_or<std::string>("NULL") <<
                            " : " << (int)quad[1] << 
                            " : " << (int)quad[2] << 
                            " : " << (int)quad[3] << 
                            " : " << (int)quad[4] << std::endl;
                            
                            skin->form(name,(float)quad[1],(float)quad[2],(float)quad[3],(float)quad[4]);
                        }else
                        {
                            auto color = item["color"];
                            if (color)
                            {
                                Skin::Shapes shape;
                                auto shapeType = item["shape"].get_or<std::string>("rectangle");
                                if (shapeType == "rectangle")
                                {
                                    shape = Skin::Shapes::Rectangle;
                                }
                                skin->form(name,shape,color[1].get_or<int>(255),color[2].get_or<int>(255),color[3].get_or<int>(255),color[4].get_or<int>(255));
                            }else
                            {
                                LOG << "Warning" << "The widget: " << item["widget"].get_or<std::string>("NULL") << " has no quad or color!" << std::endl;
                            }
                        }
                    }
                    else
                    {
                        LOG << "Warning" << "Item no " << i << " is missing!" << std::endl;
                    }
                }
                applyWrappers(skin.get());
                return skin;
            }
            inline std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>* parseSkins(sol::table table, std::string& file)
            {
                auto local = file.substr(0,file.find_last_of("/\\"));
                LOG << "Info" << "Looking into directory " << local << " from: " << file << std::endl;
                auto skins = new std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>();
                unsigned int size = table.size()+1;
                for (unsigned int i = 1; i <size; i++)
                {
                    std::string name;
                    auto item = parseSkin(table[i],local);
                    if (item)
                    {
                        name = table[i]["name"].get_or<std::string>("default");
                        skins->insert({name,item});
                        LOG << "Building" << "New Skin : " << name << std::endl;
                        
                    }else
                    {
                        LOG << "Warning" << "Loading skin failed!" << std::endl;
                        return nullptr;
                    }
                }
                return skins;
            };
            inline int parseWidget(std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>* skins,Lucia::Maigui::Manager* m,Maigui::Item* parent,sol::table table,std::string path,int no,sol::state* lua)
            {
                std::string positionMode = (*lua)["positionMode"].get_or<std::string>("center");
                std::string name = table["name"].get_or<std::string>("null");
                std::string t = table["widget"].get_or<std::string>("null");
                if (t == "null"){LOG << "Warning" << "Widget " << no << " from: " << path << " Has no type!" << std::endl;}
                else
                {
                    std::shared_ptr<Maigui::Item> nItem;
                    if (t == "button")
                    {
                        nItem = std::make_shared<Maigui::Button>();
                    }
                    else if (t == "panel")
                    {
                        nItem = std::make_shared<Maigui::Containers::Frame>();
                    }
                    else if (t == "container")
                    {
                        nItem = std::make_shared<Maigui::Container>();
                    }
                    else if (t == "frame")
                    {
                        nItem = std::make_shared<Maigui::Containers::Frame>();
                    }
                    else if (t == "costum")
                    {
                        nItem = std::make_shared<Maigui::Item>();
                    }else
                    {
                        LOG << "Warning" << "Widget: "<< no << " name: " << name << " from: " << path << " Has no type name!" << std::endl;
                        return no;
                    };
                    LOG << "Building" << " GUI " << t << ": "<< no << " : " << name << std::endl;
                    nItem->setReference(name);
                    
                    sol::table scale = table["Scale"].get_or<sol::table>(sol::table());
                    int x = table["scale"][1].get_or<int>(1);
                    int y = table["scale"][2].get_or<int>(1);
                    int z = table["scale"][3].get_or<int>(1);
                    LOG << "Info" << "Scale: x:" << x << " y: " << y << " z: " << z << std::endl; 
                    nItem->scaleTo(x,y,z);
                    
                    sol::table pos = table["position"].get_or<sol::table>(sol::table());
                    int px = table["position"][1].get_or<int>(0);
                    int py = table["position"][2].get_or<int>(0);
                    int pz = table["position"][3].get_or<int>(0);
                    
                    if (positionMode == "top right"){px += x/2.0f; py += y/2.0f;}
                    else if (positionMode == "top left"){px -= x/2.0f; py -= y/2.0f;};
                    nItem->moveTo(px,py,pz);
                    LOG << "Info" << "Position: x:" << px << " y: " << py << " z: " << pz << std::endl; 

                    
                    sol::table rot = table["rotation"].get_or<sol::table>(sol::table());
                    x = table["rotation"][1].get_or<int>(0);
                    y = table["rotation"][2].get_or<int>(0);
                    z = table["rotation"][3].get_or<int>(0);
                    LOG << "Info" << "Rotation: x:" << x << " y: " << y << " z: " << z << std::endl; 
                    nItem->rotateTo(x,y,z);
                    
                    nItem->setDrag(table["dragEnabled"].get_or<bool>(false));
                    nItem->setVisible(table["visible"].get_or<bool>(false));
                    nItem->setInactive(table["inactive"].get_or<bool>(false));
                               
                    nItem->OnDrawPre = table["OnDrawPre"].get_or<std::function<void()>>([](){});
                    nItem->OnDrawAfter = table["OnDrawAfter"].get_or<std::function<void()>>([](){});
                    nItem->OnDrag = table["OnDrag"].get_or<std::function<void(int,int)>>([](int dx,int dy){});
                    nItem->OnUpdate = table["OnUpdate"].get_or<std::function<void(double)>>([](double dt){});
                    
                    nItem->OnFocus = table["OnFocus"].get_or<std::function<void(bool)>>([](bool hasFocus){});
                    nItem->OnClick = table["OnClick"].get_or<std::function<void(std::string)>>([](std::string b){});
                    nItem->OnReleaseClick = table["OnReleaseClick"].get_or<std::function<void(std::string)>>([](std::string b){});
                    
                    nItem->OnFocus = table["OnFocus"].get_or<std::function<void(bool)>>([](bool b){});
                    nItem->OnScale = table["OnScale"].get_or<std::function<void(float,float,float)>>([](float x,float y,float z){});
                    nItem->OnRotate = table["OnRotate"].get_or<std::function<void(float,float,float)>>([](float x,float y,float z){});
                    nItem->OnGlobalScale = table["OnGlobalScale"].get_or<std::function<bool(float,float)>>([](float x,float y){return false;});
                    
                    if (parent != nullptr)
                    {
                        parent->addItem(nItem.get());
                    }else
                    {
                        m->addItem(nItem.get());
                    }
                    
                    if (table["class"])
                    {
                        auto reference = table["class"].get_or<std::string>("invalid");
                        nItem->setSkinReference(reference);
                    }
                    if (table["skin"])
                    {
                        auto skinName = table["skin"];
                        auto it = skins->find(skinName);
                        if (it != skins->end())
                        {
                            auto skin = it->second;
                            nItem->setSkin(skin);
                        }
                        else 
                        {
                            LOG << "Warning" << "Skin by the name " << table["skin"].get_or<std::string>("NULL") << " does not exist." << std::endl;
                        }
                    }else 
                    {
                        auto it = skins->find("default");
                        if (it != skins->end())
                        {
                            auto skin = it->second;
                            nItem->setSkin(skin);
                        }else
                        {
                            LOG << "Warning" << "Default skin not found!" << std::endl;
                        }
                    }
                    if (table["quad"])
                    {
                        auto quad = table["quad"];
                        nItem->setSprite(quad[1].get_or<int>(0),quad[2].get_or<int>(0),quad[3].get_or<int>(1),quad[4].get_or<int>(1));
                    }
                    
                    if (table["children"])
                    {
                        sol::table children = table["children"];
                        for (unsigned int i=1; i < children.size(); i++)
                        {
                            no = parseWidget(skins,m,nItem.get(),children[i],path,no + 1,lua);
                        }
                            
                    }
                    return no;
                }
                return no;
            };
            inline std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>* makeDefaultSkin()
            {
                auto skins = new std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>();
                skins->insert({"default",Maigui::Default::Skin});
                return skins;
            }
            inline void parseLua(Lucia::Maigui::Manager* manager,std::string path,std::function<void(sol::state* s)> attachements=[](sol::state*){})
            {
                std::ifstream LuaScriptStream(path, std::ios::in);
                std::string LuaScene = "";
                if(LuaScriptStream.is_open()){
                    std::string Line = "";
                    while(std::getline(LuaScriptStream, Line))
                        LuaScene += "\n" + Line;
                        
                    LuaScriptStream.close();
                    
                }else
                {
                    LOG << "Warning" << "Failed to open: " << path << " | file not found!" << std::endl;
                }
                try
                {
                    sol::state lua = sol::state();
                    lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package);
                    attachements(&lua);
                    lua.set_function("findItem", &Maigui::Manager::findItem,manager);
                    sol::table items = sol::table();
                
                    sol::load_result luaState = lua.load_file(path);
                    if (not luaState.valid())
                    {
                        LOG << "Warning" << "Lua state is not valid" << std::endl;
                        return;
                    }
                    luaState();
                    items = lua["UserInterface"];
                    
                    std::cout << "ITEM SIZE : " << items.size() << std::endl;
                    
                    auto skinTable = lua["Skins"];
                    std::map<std::string,std::shared_ptr<Lucia::Maigui::Skin>>* skins;
                    if (skinTable)
                    {
                        LOG << "Building" << "Found skin table!" << std::endl;
                        skins = parseSkins(skinTable,path);
                        if (not skins)
                        {
                            skins = makeDefaultSkin();
                            LOG << "Warning" << "Failed to load skin." << std::endl;
                        }
                        
                    }else
                    {   
                        skins = makeDefaultSkin();
                        LOG << "Warning" << "Failed to load skins table." << std::endl;
                    };
                    
                    int loaded = 0;
                    for (unsigned int i=0; i < items.size(); i++)
                    {
                        loaded++;
                        loaded = parseWidget(skins,manager,nullptr,items[i+1],path,loaded,&lua);
                    };
                    LOG << "Info" << "Loaded " << loaded << " items" << std::endl;
                    delete skins;
                }catch(...)
                {
                    std::cout << "RUNTIME ERROR" << std::endl;
                }
            };
            inline void applyMaiguiWrapper(sol::state* lua)
            {
                
            };
        }
    }
}
#endif