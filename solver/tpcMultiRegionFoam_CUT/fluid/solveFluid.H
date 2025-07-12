if (finalIter)
{
    mesh.data::add("finalIteration", true);
}
#include "alphaControls.H"
mixture.correct();
p == p_rgh + rho*gh;

// --- Energy cycle
#include "EEqnCycle.H"

// --- Fluid transport
Info << nl << ">>>>>VOF transport (alpha)" << endl;
#include "alphaEqnSubCycle.H"
interface.correct();
mixture.correct();

Info << ">>>>>Momentum transport (U,p)" << endl;
#include "UEqn.H"

// --- Pressure corrector loop
while (pimple.correct())
{
  #include "pEqn.H"
}

p == p_rgh + rho*gh;


if (pimple.turbCorr())
{
  mixture.correct();
  turbulence.correct();
}
        
if (finalIter)
{
    mesh.data::remove("finalIteration");
}
//***********************************************************************
