#ifndef PLANE_H
#define PLANE_H

#include "hittable.hpp"

class plane : public hittable {
public:

    plane(double A, double B, double C, double D)
    {
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
    }
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        
        auto t = (A * r.direction().x() + B * r.direction().y() + C * r.direction().z());
        if (t == 0)
            return false;

        auto root = (-(A * r.origin().x() + B * r.origin().y() + C * r.origin().z() + D)) / t;
        if (root < 0) return false;
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 out_normal(A + rec.p.x(), B + rec.p.y(), C + rec.p.z());
        rec.set_face_normal(r, out_normal);


        return true;
    }

private:
    
    point3 center;
    double radius;
};

#endif