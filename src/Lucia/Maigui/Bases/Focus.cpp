#include "Lucia\Maigui\Bases\Focus.h"
using namespace Lucia;
using namespace Maigui;
using namespace Base;
using namespace std;

void Focus::textinput(char c){};
void Focus::keypressed(string key){};
void Focus::mousepressed(string key){
    OnClick(key);
};
void Focus::mousereleased(string key)
{
    OnReleaseClick(key);
};
void Focus::setFocus(bool condition)
{
    currentFocus = condition;
    if (lastFocus != condition){
        OnFocus(condition);
        lastFocus = condition;
    };
};