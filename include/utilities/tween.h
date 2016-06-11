#ifndef MIKUS_UTILITIES_TWEEN_H
#define MIKUS_UTILITIES_TWEEN_H
#include<string>
#include<functional>
#include <map>
#include <vector>
#include <cmath>
#include <Utils.h>

namespace Utils
{
    extern std::map<std::string,std::function<double(double)>> TweenModes;
    template <typename Base>
    class Tween
    {
        private:
            struct single
            {
                single(Base* Value,Base Target,std::function<double(double)>func,float Rate=1)
                {
                    val = Value;
                    initial = *Value;
                    target = Target;
                    fn = func;
                    rate = Rate;
                };
                void stop()
                {
                    *val = target;
                    finished = true;
                };
                void pause()
                {
                    paused = true;
                };
                void unpause()
                {
                    paused = false;
                }
                void setRate(float Rate)
                {
                    rate = Rate;
                };
                void update(double dt)
                {
                    if (!paused)
                    {
                        progress += rate*fn(progress)*dt;
                        if (progress <= 100)
                        {
                            *val =  initial + Utils::difference(target,initial)*progress;
                        }else
                        {
                            stop();
                        }
                    }
                };
                ~single(){stop();};

                std::function<double(double)> fn=[](double x){return x;};
                double progress =0;
                float rate=1;
                bool paused = false;
                bool finished = false;
                bool inverted = false;
                Base initial;
                Base* val;
                Base target;
            };

            std::vector<std::shared_ptr<single>> singles;
            std::function<double(double)> fn = [](double x){return x;};
            float rate =1;

            public:
            Tween(std::string mode="Linear",bool Out=false)
            {
                if (Out)
                {
                    auto f = TweenModes[mode];
                    fn = [f](double x){return (1 - f(x));};
                }else
                {
                    fn = TweenModes[mode];
                };
            };
            //add
            std::shared_ptr<single> addVar(Base *Val,Base target)
            {
                auto sngl = std::make_shared<single>(Val,target,fn,rate);
                singles.push_back(sngl);
                return sngl;
            };
            //set
            void setRate(float f)
            {
                for (auto v: singles){v->setRate(f);};
                rate = f;
            };
            void update(double dt)
            {
                Utils::erase_if(&singles,[dt](std::shared_ptr<single> s)
                {
                    if (s->finished)
                    {
                        return true;
                    }else
                    {
                        s->update(dt);
                    }
                    return false;
                });
            };
            void stop()
            {
                for (auto v: singles)
                {
                    v->stop();
                }
            }
            virtual ~Tween();
    };
    template <typename Base>
    class TweenManager
    {
        public:
            TweenManager();
            void addTween(std::shared_ptr<Tween<Base>> t)
            {
                Tweens.push_back(t);
            };
            void clear()
            {
                for (auto v: Tweens){v->stop();};
                Tweens.clear();
            };
            void update(double dt)
            {
                for (auto v: Tweens){v->update(dt);};
            };
            ~TweenManager();
        private:
            std::vector<Tween<Base>> Tweens;
    };
}

#endif // MIKUS_UTILITIES_TWEEN_H
