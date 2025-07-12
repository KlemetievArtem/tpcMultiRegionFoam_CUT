   fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(rhoPhi, U)
      //+ turbulence.divDevRhoReff(rho,U)
      - fvm::laplacian((thermo.nu()*rho+turbulence.nut()*rho*(1 - alpha1Lim)), U)
      - fvc::div((thermo.nu()*rho+turbulence.nut()*rho*(1 - alpha1Lim))*dev(fvc::grad(U)().T()))
    );

    UEqn.relax();

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    interface.surfaceTensionForce()
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
                ) * mesh.magSf()
            )
//            -fvc::grad(p_rgh)
        );
    }
    
//    thermoRho = thermo.rho();
//    thermoMu = thermo.mu();
//    thermoNu = thermo.nu();
    


