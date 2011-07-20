#ifndef LIB_DET_INTERFACE_H
#define LIB_DET_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "string.h"

#define MAX_TARGETS 25 // 25 Seems enough, want more? recompile

struct imagesize_t
{
    int width;
    int height;
};
// Provides a byte for each pixel
struct motion_t
{
	imagesize_t size;
	unsigned char* motion;
};

struct motionhelper_t
{
    imagesize_t size;
    int MinX;
    int MinY;
    int MaxX;
    int MaxY;
    unsigned char* motion;
};

struct pixel_t
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


template <class myType>
myType GetMax (myType a, myType b) {
 return (a>b?a:b);
}
class CDetectorImmutable
{
public:
    void Refrence(void)
    {
        m_iRefrenceCount++;
    };
    void DeRefrence(void)
    {
        m_iRefrenceCount--;
        if(m_iRefrenceCount == 0)
            delete this;
    };
protected:
    int m_iRefrenceCount;
};


class CDetectorImage : public CDetectorImmutable
{
public:
    CDetectorImage(int Width, int Height)
    {
        this->Refrence();
        m_sSize.width = Width;
        m_sSize.height = Height;
        m_psPixels = new pixel_t[Width + Width * Height];
    };
    CDetectorImage(imagesize_t size)
    {
        this->Refrence();
        m_sSize.width = size.width;
        m_sSize.height = size.height;
        m_psPixels = new pixel_t[size.width + size.width * size.height];
    };
    ~CDetectorImage()
    {
        delete[] m_psPixels;
    };
    CDetectorImage* Exclusive()
    {
        if(m_iRefrenceCount == 1)
            return this;
        CDetectorImage* ptr = new CDetectorImage(this->GetSize());
        memcpy(ptr->m_psPixels, this->m_psPixels, sizeof(this->m_psPixels));
        this->DeRefrence();
        return ptr;
    };
    pixel_t* Pixel(int x, int y)
    {
        return &m_psPixels[x + y * m_sSize.width];
    };
    imagesize_t GetSize()
    {
        return m_sSize;
    };
private:
    imagesize_t m_sSize;
    pixel_t* m_psPixels;
};

struct target_t
{
    float x;
    float y;
    float width;
    float height;
};

class IDetector
{
public:
    // Push the next image here, calculates the new target
    virtual void PushImage(CDetectorImage *Image) = 0;
    // Returns the number of targets, outputs to argument
    virtual int GetTargets(target_t* Targets[MAX_TARGETS]) = 0;
};


#endif // LIB_DET_INTERFACE_H
