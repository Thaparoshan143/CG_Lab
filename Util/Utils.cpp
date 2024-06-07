#include<iostream>

#include"../Core/Types.h"

namespace Util
{
    float* get_2dpoint_with_color(float *rawpointxy, uint count, Color3 color)
    {
        // get new block for point as well as color (3) 
        float *temp = new float[count + ((count*3)/2)];

        for(int i=0;i<count/2;++i)
        {
            *(temp + 5*i) = *(rawpointxy + 2*i);
            *(temp + 5*i + 1) = *(rawpointxy + 2*i + 1);
            *(temp + 5*i + 2) = color.r;
            *(temp + 5*i + 3) = color.g;
            *(temp + 5*i + 4) = color.b;
        }

        return temp;
    }

    // modifies the passed raw array against -1 to 1 range
    void normalizeM11_point2d(float *rawxy, uint count, iVec2 screensize)
    {
        for(int i=0;i<count/2;++i)
        {
            *(rawxy + 2*i) = (*(rawxy + 2*i) - (screensize.x/2.0))/(screensize.x/2.0);
            *(rawxy + 2*i + 1) = (*(rawxy + 2*i + 1) - (screensize.y/2.0))/(screensize.y/2.0);
        }
    }

    // modifies the passed raw array against 0 to 1 range
    void normalize01_point2d(float *rawxy, uint count, iVec2 screensize)
    {
        for(int i=0;i<count/2;++i)
        {
            *(rawxy + 2*i) = (*(rawxy + 2*i)/float(screensize.x));
            *(rawxy + 2*i + 1) = (*(rawxy + 2*i + 1)/float(screensize.y));
        }
    }

    template<typename T>
    T get_title_input(const char *title)
    {
        T temp;
        std::cout << title << std::endl;
        std::cin >> temp;
        return temp;
    }

    fVec2 get_2dpoint(const char *title = nullptr)
    {
        fVec2 temp;
        if(title != nullptr)
            std::cout << title << std::endl;
        std::cout << "X, Y : ";
        std::cin >> temp.x >> temp.y;
        return temp;
    }

    template<typename T = float>
    void vector_roundup(std::vector<T> &target)
    {
        T *raw = target.data();
        for(int i=0;i<target.size();i++)
        {
            *(raw + i) = round(*(raw+i));
        }
    }
}