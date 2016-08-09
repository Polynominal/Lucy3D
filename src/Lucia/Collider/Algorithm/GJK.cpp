#include "Lucia\Collider\Algorithm\GJK.h"
using namespace Lucia;
using namespace Collider;
Vertex GJK::minowskiSum(Shape *A,Shape *B,Vertex dir)
{
    A->refresh(); // update each to make sure that we are in the appropriate position.
    B->refresh();
    Vertex a = A->support(dir);
    Vertex b = B->support(-dir);
    return a-b;
}
Vertex GJK::minowskiSum(std::vector<Vertex> points,Shape *A,Vertex dir)
{
    A->refresh();
    Vertex a = A->support(dir);
    Vertex b = farthestPoint(points,-dir);
    return a-b;
}
bool GJK::triangle(Vertex &dir)
{
    Vertex a = simplex[simplex.size()-1];
    Vertex ao = -a;

    Vertex b = simplex[simplex.size()-2];
    Vertex c = simplex[simplex.size()-3];

    Vertex ab = b-a;
    Vertex ac = c-a;

    Vertex abc = ab.cross(ac);
    simplex.clear();
    if ((abc.cross(ac)).dot(ao) > 0)
    {
        if (ac.dot(ao) > 0)
        {
            dir = ac.cross(ao).cross(ac);
            simplex.push_back(c);
            simplex.push_back(a);
        }else{
            if (ab.dot(ao) > 0)
            {
                dir = ab.cross(ao).cross(ab);
                simplex.push_back(b);
                simplex.push_back(a);
            }else
            {
                dir = ao;
                simplex.push_back(a);
            }
        }
    }else
    {
        if (ab.cross(abc).dot(ao) > 0)
        {
            line(dir,a,b);
            return false;
        }
        else
        {
            if (abc.dot(ao) > 0)
            {
                dir = abc;
                simplex.push_back(c);
                simplex.push_back(b);
                simplex.push_back(a);
                return false;
            }else
            {
                dir = -abc;
                simplex.push_back(b);
                simplex.push_back(c);
                simplex.push_back(a);
                return false;
            }
        }
    }
    return false;
}
bool GJK::line(Vertex &dir,Vertex a,Vertex b)
{
    Vertex ao = -a;
    Vertex ab = b -a;
    simplex.clear();
    if (ab.dot(ao) > 0)
    {
        dir = ab.cross(ao).cross(ab);
        simplex.push_back(b);
        simplex.push_back(a);
    }else
    {
        dir = ao;
        simplex.push_back(a);
    }
    return false;
}
bool GJK::tetrahedron(Vertex &dir)
{
    Vertex a = simplex[simplex.size()-1];
    Vertex b = simplex[simplex.size()-2];
    Vertex c = simplex[simplex.size()-3];
    Vertex d = simplex[simplex.size()-4];
    /* triangles available@
    *
    *                                  a
    *                                / | \
    *                         <-    /  d  \   ->
    *                              / / + \ \
    *                             c---------b
    *                                  |
    *                                  '
    * a-b-c
    * a-c-d
    * a-d-b
    * c-d-b
    */

    Vertex ao = -a;
    int cols = 0;
    Vertex ab = b - a;
    Vertex ac = c - a;
    Vertex ad = d - a;

    Vertex abc = ab.cross(ac);
    Vertex acd = ac.cross(ad);
    Vertex adb = ad.cross(ab);

    if (abc.dot(ao) > 0){cols =+ 1;};
    if (acd.dot(ao) > 0){cols =+ 2;};
    if (adb.dot(ao) > 0){cols =+ 4;};
    switch(cols)
    {
        case 0:
            {
                // all of the sides overlap the centre so no need for further work.
                return true;
            }break;
        case 1:
            {
                // in front of ABC
                simplex.clear();
                simplex.push_back(c);
                simplex.push_back(b);
                simplex.push_back(a);
                triangle(dir);
                return false;

            }break;

        case 2:
            {
                simplex.clear();
                simplex.push_back(d);
                simplex.push_back(c);
                simplex.push_back(a);
                triangle(dir);
                return false;

            }break;

        case 3:
            {
                if ((abc.cross(ac)).dot(ao) > 0)
                {
                    if((ac.cross(acd)).dot(ao) > 0)
                    {
                        if (ac.dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,c);
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                        }
                        return false;
                    }else
                    {
                        if((acd.cross(ad)).dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,d);
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(d);
                            simplex.push_back(c);
                            simplex.push_back(a);
                        }
                    }
                }else
                {
                    if ((ab.cross(abc)).dot(ao) > 0)
                    {
                        if (ab.dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,b);

                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                        }
                    }else
                    {
                        simplex.clear();
                        simplex.push_back(a);
                        simplex.push_back(b);
                        simplex.push_back(c);
                    }
                }
                return false;
            }break;
        case 4:
            {
                simplex.clear();
                simplex.push_back(b);
                simplex.push_back(d);
                simplex.push_back(a);
                return false;
            }break;
        case 5:
            {
              if (adb.cross(ab).dot(ao) > 0)
              {
                    if (ab.cross(abc).dot(ao) > 0)
                    {
                        if (ab.dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,b);
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                        }
                        return false;
                    }else
                    {
                        if (abc.cross(ac).dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,c);
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(c);
                            simplex.push_back(b);
                            simplex.push_back(a);
                        }
                    }
              }else
              {
                  if(ad.cross(adb).dot(ao) > 0)
                  {
                      if (ad.dot(ao) > 0)
                      {
                            simplex.clear();
                            line(dir,a,d);
                      }else
                      {
                          simplex.clear();
                          simplex.push_back(a);
                      }
                  }else
                  {
                    simplex.clear();
                    simplex.push_back(a);
                    simplex.push_back(d);
                    simplex.push_back(b);
                  }
              }
              return false;
            }break;
        case 6:
            {
                if (acd.cross(ad).dot(ao) > 0)
                {
                    if (ad.cross(adb).dot(ao) > 0)
                    {

                        if (ad.dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,d);
                            return false;
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                            return false;
                        }
                    }else
                    {
                        if(adb.cross(ab).dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,b);
                            return false;
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(b);
                            simplex.push_back(d);
                            simplex.push_back(a);
                            return false;
                        }
                    }
                }else
                {
                    if (ac.cross(acd).dot(ao) > 0)
                    {
                        if (ac.dot(ao) > 0)
                        {
                            simplex.clear();
                            line(dir,a,c);
                            return false;
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                            return false;
                        }
                    }else
                    {
                        simplex.clear();
                        simplex.push_back(d);
                        simplex.push_back(c);
                        simplex.push_back(a);
                        return false;

                    }
                }
            }break;
        case 7:
            {
                if (ab.dot(ao) > 0)
                {
                    simplex.clear();
                    line(dir,a,b);
                    return false;
                }else
                {
                    if(ac.dot(ao) > 0)
                    {
                        simplex.clear();
                        line(dir,a,c);
                        return false;
                    }else
                    {
                        if (ad.dot(ao) > 0)
                        {
                            simplex.clear();
                            simplex.push_back(c);
                            simplex.push_back(a);
                            return false;
                        }else
                        {
                            simplex.clear();
                            simplex.push_back(a);
                            return false;
                        }
                    }
                }
            }break;
    }
    return false;
}
bool GJK::containsOrigin(Vertex &dir)
{
    switch(simplex.size())
    {
        case 2:
            {
                bool a = line(dir,simplex[simplex.size()-1],simplex[simplex.size()-2]);
                return a;
                break;
            }
        case 3:
            {
                bool a = triangle(dir);
                return a;
                break;
            }
        case 4:
            {
                return tetrahedron(dir);
                break;
            }

    }
    return false;
}
bool GJK::collidesWith(Shape *A,Vertex B)
{
    simplex.clear();
    Vertex dir = Vertex(1,1,1);
    Vertex a = (A->support(dir) - B);
    simplex.push_back(a);
    dir = -a;

    bool condition = true;
    int steps = 0;
    while(condition)
    {
        steps++;
        Vertex a = (A->support(dir) - B);
        if (a.dot(dir) < 0)
        {
            return false;
        }else
        {
            if (simplex.size() >= 4) // cannot form a shape so must dump
            {
                condition = false;
                return false;
            }
            simplex.push_back(a);
            if (containsOrigin(dir))
            {
                condition = false;
                return true;
            }
        };
        if (steps > 50)
        {
            return false;
        }
    }
    return false;
};
bool GJK::collidesWith(Shape *A,Shape *B)
{
    simplex.clear();
    Vertex dir = Vertex(1,1,1);
    Vertex a = minowskiSum(A,B,dir);
    simplex.push_back(a);
    dir = -a;

    bool condition = true;
    int steps = 0;
    while(condition)
    {
        steps++;
        Vertex a = minowskiSum(A,B,dir);
        if (a.dot(dir) < 0)
        {
            return false;
        }else
        {
            if (simplex.size() >= 4) // cannot form a shape so must dump
            {
                condition = false;
                return false;
            }
            simplex.push_back(a);
            if (containsOrigin(dir))
            {
                condition = false;
                return true;
            }
        };
        if (steps > 50)
        {
            return false;
        }
    }
    return false;
};
bool GJK::collidesWith(std::vector<Vertex> A,Shape *B)
{
    simplex.clear();
    Vertex dir = Vertex(1,1,1);
    Vertex a = minowskiSum(A,B,dir);
    simplex.push_back(a);
    dir = -a;

    bool condition = true;
    int steps = 0;
    while(condition)
    {
        steps++;
        Vertex a = minowskiSum(A,B,dir);
        if (a.dot(dir) < 0)
        {
            return false;
        }else
        {
            if (simplex.size() >= 4) // cannot form a shape so must dump
            {
                condition = false;
                return false;
            }
            simplex.push_back(a);
            if (containsOrigin(dir))
            {
                condition = false;
                return true;
            }
        };
        if (steps > 50)
        {
            return false;
        }
    }
    return false;
};
bool GJK::checkCollision(std::vector<Vertex>Points1,std::vector<Vertex>Points2)
{
    simplex.clear();
    Vertex dir = Vertex(1,1,1);
    Vertex a = support(Points1,Points2,dir);
    simplex.push_back(a);
    dir = -a;

    bool condition = true;
    int steps = 0;
    while(condition)
    {
        steps++;
        Vertex a = support(Points1,Points2,dir);
        if (a.dot(dir) < 0)
        {
            return false;
        }else
        {
            if (simplex.size() >= 4) // cannot form a shape so must dump
            {
                condition = false;
                return false;
            }
            simplex.push_back(a);
            if (containsOrigin(dir))
            {
                condition = false;
                return true;
            }
        };
        if (steps > 50)
        {
            return false;
        }
    }
    return false;
}
// this is for use with points-only system.
Vertex GJK::farthestPoint(std::vector<Vertex> Points,Vertex Dir)
{
    // calculate the world position by applying translations or apply prior to using this!.
    Vertex Center = Points[0];
    float maxDot = Center.dot(Dir);
    Vertex maxPoint = Center;
    // translated points go here ->
    for (auto v: Points)
    {
        float dot = v.dot(Dir);
        if (dot > maxDot)
        {
            maxDot = dot;
            maxPoint = v;
        }
    }
    return maxPoint;
}
Vertex GJK::support(std::vector<Vertex> Points1,std::vector<Vertex> Points2,Vertex dir)
{
    Vertex shape1 = farthestPoint(Points1,dir);
    Vertex shape2 = farthestPoint(Points2,-dir);
    return shape1 - shape2;
}
