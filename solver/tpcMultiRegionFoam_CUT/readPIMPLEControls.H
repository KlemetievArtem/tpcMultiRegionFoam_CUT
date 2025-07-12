    // We do not have a top-level mesh. Construct the fvSolution for
    // the runTime instead.
    fvSolution solutionDict(runTime);

    const dictionary& pimple1 = solutionDict.subDict("PIMPLE");

    const int nOuterCorr = 
        pimple1.lookupOrDefault<int>("nOuterCorrectors", 25);

    const int nNonOrthCorr =
        pimple1.lookupOrDefault<int>("nNonOrthogonalCorrectors", 1);

    const bool momentumPredictor =
        pimple1.lookupOrDefault("momentumPredictor", true);
