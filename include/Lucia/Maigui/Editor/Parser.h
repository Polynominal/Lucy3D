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
        inline int parseWidget(Lucia::Maigui::Manager* m,Maigui::Item* parent,sol::table table,std::string path,int no,sol::state* lua)
        {
            std::string name = table["name"].get_or<std::string>("null");
            std::string t = table["widget"].get_or<std::string>("null");
            if (t == "null"){LOG << "Error" << "Widget " << no << " from: " << path << " Has no type!" << std::endl;}
            else
            {
                std::shared_ptr<Maigui::Item> nItem;
                if (t == "button")
                {
                    nItem = std::make_shared<Maigui::Button>();
                }else if (t == "container")
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
                    LOG << "Error" << "Widget " << name << " from: " << path << " Has no name!" << std::endl;
                    return no;
                };
                nItem->setReference(name);
                
                sol::table pos = table["Position"];
                int x = table["Position"][0].get_or<int>(0);
                int y = table["Position"][1].get_or<int>(0);
                int z = table["Position"][2].get_or<int>(0);
                nItem->moveTo(x,y,z);
                
                sol::table scale = table["Scale"];
                x = table["Scale"][0].get_or<int>(1);
                y = table["Scale"][1].get_or<int>(1);
                z = table["Scale"][2].get_or<int>(1);
                nItem->scaleTo(x,y,z);
                
                sol::table rot = table["Rotation"];
                x = table["Rotation"][0].get_or<int>(1);
                y = table["Rotation"][1].get_or<int>(1);
                z = table["Rotation"][2].get_or<int>(1);
                nItem->rotateTo(x,y,z);
                
                nItem->setDrag(table["dragEnabled"].get_or<bool>(true));
                //nItem->setVisible(table["visible"].get_or<bool>(true));
                nItem->setInactive(table["inactive"].get_or<bool>(true));
                           
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
                sol::table children = table["children"];
                for (unsigned int i=0; i < children.size(); i++)
                {
                    no = parseWidget(m,nItem.get(),children[i],path,no + 1,lua);
                }
                return no;
            }
            return no;
        };
        inline void parseLua(Lucia::Maigui::Manager* manager,std::string path)
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
                LOG << "Error" << "Failed to open: " << path << " | file not found!" << std::endl;
            }
            //
            
            sol::state lua;
            lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package);
            lua.set_function("findItem", &Maigui::Manager::findItem,manager);
            sol::load_result LuaState = lua.load(LuaScene);
            
            sol::table items = LuaState();
            int loaded = 0;
            for (unsigned int i=0; i < items.size(); i++)
            {
                loaded++;
                loaded = parseWidget(manager,nullptr,items[i],path,loaded,&lua);
            };
            LOG << "Info" << "Loaded " << loaded << " items" << std::endl;
        };
        }
    }
}
#endif