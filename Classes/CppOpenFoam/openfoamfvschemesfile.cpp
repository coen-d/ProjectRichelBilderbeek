#include "openfoamfvschemesfile.h"

OpenFoamFvSchemesFile::OpenFoamFvSchemesFile()
{
}

/*

#include "trianglemeshbuilder.h"

#include <sstream>

const std::string ribi::trim::TriangleMeshBuilder::CreateOpenFoamFvSchemes() const noexcept
{
  std::stringstream s;
  s
    << "FoamFile\n"
    << "{\n"
    << "    version     2.0;\n"
    << "    format      ascii;\n"
    << "    class       dictionary;\n"
    << "    location    \"system\";\n"
    << "    object      fvSchemes;\n"
    << "}\n"
    << "\n"
    << "ddtSchemes\n"
    << "{\n"
    << "    default         Euler;\n"
    << "}\n"
    << "\n"
    << "gradSchemes\n"
    << "{\n"
    << "    default         Gauss linear;\n"
    << "}\n"
    << "\n"
    << "divSchemes\n"
    << "{\n"
    << "    default         none;\n"
    << "    div(phi,U)      Gauss limitedLinearV 1;\n"
    << "    div(phi,e)      Gauss limitedLinear 1;\n"
    << "    div(phid,p)     Gauss limitedLinear 1;\n"
    << "    div(phi,K)      Gauss limitedLinear 1;\n"
    << "    div(phiv,p)     Gauss limitedLinear 1;\n"
    << "    div(phi,k)      Gauss upwind;\n"
    << "    div(phi,epsilon) Gauss upwind;\n"
    << "    div((muEff*dev2(T(grad(U))))) Gauss linear;\n"
    << "}\n"
    << "\n"
    << "laplacianSchemes\n"
    << "{\n"
    << "    default         Gauss linear corrected;\n"
    << "}\n"
    << "\n"
    << "interpolationSchemes\n"
    << "{\n"
    << "    default         linear;\n"
    << "}\n"
    << "\n"
    << "snGradSchemes\n"
    << "{\n"
    << "    default         corrected;\n"
    << "}\n"
    << "\n"
    << "fluxRequired\n"
    << "{\n"
    << "    default         no;\n"
    << "    p;\n"
    << "}\n"
    << "\n"
  ;
  return s.str();
}

*/
