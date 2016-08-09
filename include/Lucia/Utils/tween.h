#ifndef MIKUS_LUCIA_UTILITIES_TWEEN_H
#define MIKUS_LUCIA_UTILITIES_TWEEN_H
#include<string>
#include<functional>
#include <map>
#include <vector>
#include <cmath>
#include <math.h>
#include <Lucia\Utils\Utils.h>
namespace Lucia {
namespace Utils
{
    extern std::map<std::string,std::function<double(double)>> TweenModes;
    enum TweenMode
    {
        Loop,
        Bounce,
        Once
    };
    template <typename Base>
    class Tween
    {
        private:
            struct single
            {
                single(Base* Value,Base Target,std::function<double(double)>func,float timez=1)
                {
                    val = Value;
                    initial = *Value;
                    target = Target;
                    fn = func;
                    time = timez;
                    calculate();
                };
                void calculate()
                {
                    distance = target - initial;
                    speed = std::abs(distance/time);
                }
                void stop()
                {
                    *val = target;
                    progress = 0;
                    finished = true;
                };
                void reset()
                {
                    *val = initial;
                    progress = 0;
                    finished = false;
                };
                void pause()
                {
                    paused = true;
                };
                void unpause()
                {
                    paused = false;
                };
                //set
                void setMode(TweenMode m)
                {
                    mode = m;
                };
                //
                void setRate(float rate)
                {
                    speed = rate;
                };
                void update(double dt)
                {
                    if (!paused or !finished)
                    {
                        speed = fn(speed);
                        progress += speed*dt;
                        if (progress <= 100)
                        {
                            auto dom =progress/100.0f;
                            if ((std::isinf(dom))){dom = 0;};
                            *val =  initial + (distance*dom);
                        }else
                        {
                            switch (mode)
                            {
                                case Loop:
                                    reset();
                                    break;
                                case Once:
                                    stop();
                                    break;
                                case Bounce:
                                    Base n = initial;
                                    initial = target;
                                    target = n;
                                    calculate();
                                    reset();

                                    break;
                            }
                        }
                    }
                };
                ~single(){stop();};

                std::function<double(double)> fn=[](double x){return x;};
                double progress =0;
                double speed = 0;
                float time=1.0f;

                bool paused = false;
                bool finished = false;
                bool inverted = false;
                TweenMode mode= TweenMode::Once;

                Base distance;

                Base initial;
                Base* val;
                Base target;
            };
            TweenMode mode = TweenMode::Once;
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
            std::shared_ptr<single> addVar(Base *Val,Base target,float time)
            {
                auto sngl = std::make_shared<single>(Val,target,fn,time);
                sngl->setMode(mode);
                singles.push_back(sngl);
                return sngl;
            };
            //set
            void setMode(TweenMode m)
            {
                for (auto v: singles){v->setMode(m);};
                mode = m;
            }
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
            void setRate(float rate)
            {
                for (auto v: singles){v->setRate(rate);};
            };
            void unpause()
            {
                for (auto v: singles){v->unpause();};
            };
            void pause()
            {
                for (auto v: singles){v->pause();};
            };
            void stop()
            {
                for (auto v: singles)
                {
                    v->stop();
                };
                singles.clear();
            }
            virtual ~Tween(){stop();};
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
}

#endif // MIKUS_UTILITIES_TWEEN_H
