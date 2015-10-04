#include "BoundingBox.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


Ray::Ray(Vector origin, Vector end) : m_origin(origin), m_end(end)
{
    Vector v1 (end[0] - origin[0], end[1] - origin[1], end[2] - origin[2]);
    m_dir = v1;
}

Ray::Ray(float x1, float y1, float z1, float x2, float y2, float z2)
{
    Vector v1(x1, y1, z1);
    m_origin = v1;
    Vector v2(x2, y2, z2);
    m_end = v2;
    Vector v3( x2 - x1, y2 - y1, z2 - z1);
    v3.normalize();
    m_dir = v3;
}

BoundingBox::BoundingBox(Vector center, Vector size)
{
    xmin = center[0] -  size[0]/2.0;
    xmax = center[0] +  size[0]/2.0;
    ymin = center[1] -  size[1]/2.0;
    ymax = center[1] +  size[1]/2.0;
    zmin = center[2] -  size[2]/2.0;
    zmax = center[2] +  size[2]/2.0;
}

BoundingBox::BoundingBox(float x, float y, float z, float sizex, float sizey, float sizez)
{
    xmin = x - sizex/2.0;
    xmax = x + sizex/2.0;
    ymin = y - sizey/2.0;
    ymax = y + sizey/2.0;
    zmin = z - sizez/2.0;
    zmax = z + sizez/2.0;
}

void BoundingBox::setCenter(float x, float y, float z)
{
    float xs = xmax - xmin;
    xmin = x - xs/2.0;
    xmax = x + xs/2.0;
    float ys = ymax - ymin;
    ymin = y - ys/2.0;
    ymax = y + ys/2.0;
    float zs = zmax - zmin;
    zmin = z - zs/2.0;
    zmax = z + zs/2.0;
}

void BoundingBox::setCenter(Vector c)
{
    setCenter(c[0], c[1], c[2]);
}

void BoundingBox::setSize(float x, float y, float z)
{
    setSizeX(x);
    setSizeY(y);
    setSizeZ(z);
}

void BoundingBox::setSize(Vector c)
{
    setSize(c[0], c[1], c[2]);
}

hitResult BoundingBox::hitTest(Ray *ray)
{
    hitResult hr = {false, 0, 0, 0};
    float tmin, tmax, tymin, tymax, tzmin, tzmax, txmin, txmax;
    tmin = tymin = tzmin = txmin = 0.0;
    tmax = tymax = tzmax = txmax = 1.0;
    float dirx = ray->m_dir.x;
    if ( dirx > 0 )
    {
        txmin = ( xmin - ray->m_origin.x ) / dirx;
        txmax = ( xmax - ray->m_origin.x ) / dirx;
    }
    else if ( dirx < 0 )
    {
        txmin = ( xmax - ray->m_origin.x) / dirx;
        txmax = ( xmin - ray->m_origin.x) / dirx;
    }
    else
    {
        txmin = ( xmax - ray->m_origin.x) / -0.0001;
        txmax = ( xmin - ray->m_origin.x) / -0.0001;
    }
    tmin = txmin;
    tmax = txmax;

    float diry = ray->m_dir.y;
    if ( diry > 0 )
    {
        tymin = ( ymin - ray->m_origin.y)/diry;
        tymax = ( ymax - ray->m_origin.y)/diry;
    }
    else if ( diry < 0 )
    {
        tymin = ( ymax - ray->m_origin.y)/diry;
        tymax = ( ymin - ray->m_origin.y)/diry;
    }
    else
    {
        tymin = ( ymax - ray->m_origin.y) / -0.0001;
        tymax = ( ymin - ray->m_origin.y) / -0.0001;
    }
    if ( tmin > tymax || tymin > tmax ){
        return hr;
    }
    if ( tymin > tmin ){
        tmin = tymin;
    }
    if ( tymax < tmax ){
        tmax = tymax;
    }

    float dirz = ray->m_dir.z;
    if ( dirz > 0 )
    {
        tzmin = ( zmin - ray->m_origin.z ) / dirz;
        tzmax = ( zmax - ray->m_origin.z ) / dirz;
    }
    else if ( dirz < 0 )
    {
        tzmin = ( zmax - ray->m_origin.z ) / dirz;
        tzmax = ( zmin - ray->m_origin.z ) / dirz;
    }
    else
    {
        tzmin = ( zmax - ray->m_origin.z) / -0.0001;
        tzmax = ( zmin - ray->m_origin.z) / -0.0001;
    }
    if ( tmin > tzmax || tzmin > tmax ){
        return hr;
    }
    if ( tzmin > tmin ){
        tmin = tzmin;
    }
    if ( tzmax < tmax ){
        tmax = tzmax;
    }

    if ( tmin > tmax ){
        return hr;
    }
    hr.hit = true;
    hr.tmin = tmin;
    return hr;
}

Vector BoundingBox::hitCoordinate(Ray *ray, int axis){

    Vector i1 = ray->m_end;
    Vector i0 = ray->m_origin;
    Vector p0,p1,p2,n;
    if (axis == 1){ //AXIAL
        p0 = Vector(xmin,ymin,zmin);
        p1 = Vector(xmax,ymin,zmin);
        p2 = Vector(xmin,ymin,zmax);
        n = Vector(0,ymin-1,0);    
    } else if (axis == 2){ //CORONAL
        p0 = Vector(xmin,ymin,zmin);
        p1 = Vector(xmin,ymax,zmin);
        p2 = Vector(xmax,ymin,zmin);
        n = Vector(0,0, zmin-1);            
    } else { //SAGITAL        
        p0 = Vector(xmin,ymin,zmin);
        p1 = Vector(xmin,ymax,zmin);
        p2 = Vector(xmin,ymin,zmax);
        n =  Vector(xmin-1,0,0);
    }
        
    double d = -p0.x * n.x + -p0.y * n.y + -p0.z * n.z;
    double i0n = i0.x * n.x + i0.y * n.y + i0.z * n.z;
    double i10n = (i1.x - i0.x) * n.x + (i1.y - i0.y) * n.y + (i1.z - i0.z) * n.z;
    double t = (-d - i0n)/i10n;

    Vector res = Vector(0.0, 0.0, 0.0);
    res.x = i0.x + t * (i1.x-i0.x);
    res.y = i0.y + t * (i1.y-i0.y);
    res.z = i0.z + t * (i1.z-i0.z);

    return res;
}

