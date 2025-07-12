 scalar maxFo
(
    readScalar(runTime.controlDict().lookup("maxFo"))
);

 scalar FoNum = -GREAT;
 scalar meanFoNum = -GREAT;
 
    forAll(fluidRegions, regionI)
    {
        FoNum = max
        (
            gMax(  ((DTFluid[regionI] / (CpFluid[regionI] * rhoFluid[regionI])))
          *runTime.deltaTValue() / pow(minCellLengthFluid[regionI].field(), 2) ),
            FoNum
        );
		
        meanFoNum = max
        (
            gAverage(  ((DTFluid[regionI] / (CpFluid[regionI] * rhoFluid[regionI])))
          *runTime.deltaTValue() / pow(minCellLengthFluid[regionI].field(), 2) ),
            FoNum
        );
    }

Info<< "Fourier number mean: " << meanFoNum
    << " max: " << FoNum << endl;

// ************************************************************************* //
