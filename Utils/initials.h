#ifndef INITIALS
#define INITIALS

/// add this to resolve
/// "Warning: Link to vtkInteractionStyle for default style selection."
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
// VTK_MODULE_INIT(vtkRenderingFreeTypeOpenGL);

/// additional libs
#pragma comment(lib, "snmpapi.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")

# include "vtklibs.h"
# include "itklibs.h"
# include "itkutils.h"

class PreInitials {
public:
    static void Initilize()
    {
        ItkUtils::RegisterAllImageIOFactories();
    }
};


#endif // INITIALS

