#ifndef FASTDEF
#define FASTDEF


# include "vtkSmartPointer.h"
# include "initials.h"

#define Instantiate(obj,class) vtkSmartPointer<class> obj = vtkSmartPointer<class>::New();
#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
template <typename T>
inline void vsp(vtkSmartPointer<T> &var)
{
    vtkSmartPointer<T> temp = vtkSmartPointer<T>::New();
    var = temp;
}

struct Point3f {
    double x, y, z;
};
static double tranElements[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
};
static double coronalElements[16] = {
    1, 0, 0, 0,
    0, 0, 1, 0,
    0, -1, 0, 0,
    0, 0, 0,  1
};
static double sagittalElements[16] = {
    0, 0, -1, 0,
    1, 0, 0, 0,
    0, -1, 0, 0,
    0, 0 , 0, 1
};
#endif // FASTDEF

