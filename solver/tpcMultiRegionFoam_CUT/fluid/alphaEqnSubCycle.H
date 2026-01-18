{
    surfaceScalarField phic(mag(phi/mesh.magSf()));
    phic = min(interface.cAlpha()*phic, max(phic));

    volScalarField divU(fvc::div(phi));

    if (nAlphaSubCycles > 1)
    {
        dimensionedScalar totalDeltaT = runTime.deltaT();
        surfaceScalarField rhoPhiSum("rhoPhiSum", rhoPhi);

        for
        (
            subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
            !(++alphaSubCycle).end();
        )
        {
            #include "alphaEqn.H"
            rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
        }

        rhoPhi = rhoPhiSum;
    }
    else
    {
        #include "alphaEqn.H"
    }

	
    interface.correct();


	const volScalarField alpha1Lim( min(max(alpha1, scalar(0)), scalar(1)) );
	alpha2 = scalar(1) - alpha1Lim;
    rho == alpha1Lim*rho1 + alpha2*rho2;
    DT == alpha1Lim*DT1 + alpha2*DT2;
    Cp == alpha1Lim*Cp1  + alpha2*Cp2;
    
    thermo.recalcRho(rho);
    thermo.recalcMu(mixture.mu());
}
