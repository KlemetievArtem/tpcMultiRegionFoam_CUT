// Initialise fluid field pointer lists
PtrList<rhoThermo> thermoFluid(fluidRegions.size());
//PtrList<twoPhaseVOFThermo> thermoFluid(fluidRegions.size());
PtrList<phaseChangeTwoPhaseMixtureCond> mixtureFluid(fluidRegions.size());
PtrList<IOdictionary> transportPropertiesFluid(fluidRegions.size());
PtrList<volScalarField> dheFluid(fluidRegions.size());
PtrList<volScalarField> q_pcFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());
PtrList<volScalarField> alpha1Fluid(fluidRegions.size());
PtrList<volScalarField> alpha2Fluid(fluidRegions.size());
PtrList<dimensionedScalar> dhvFluid(fluidRegions.size());
PtrList<dimensionedScalar> TsatFluid(fluidRegions.size());
PtrList<volScalarField> DTFluid(fluidRegions.size());
PtrList<dimensionedScalar> DT1Fluid(fluidRegions.size());
PtrList<dimensionedScalar> DT2Fluid(fluidRegions.size());
PtrList<volScalarField> CpFluid(fluidRegions.size());
PtrList<dimensionedScalar> Cp1Fluid(fluidRegions.size());
PtrList<dimensionedScalar> Cp2Fluid(fluidRegions.size());
PtrList<dimensionedScalar> rho1Fluid(fluidRegions.size());
PtrList<dimensionedScalar> rho2Fluid(fluidRegions.size());
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<surfaceScalarField> rhoPhiFluid(fluidRegions.size());
PtrList<surfaceScalarField> alpha1PhiFluid(fluidRegions.size());
PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
PtrList<incompressible::turbulenceModel> turbulenceFluid(fluidRegions.size());
//PtrList<PhaseIncompressibleTurbulenceModel<phaseChangeTwoPhaseMixtureCond>> turbulenceFluid_test(fluidRegions.size());

PtrList<volScalarField> p_rghFluid(fluidRegions.size());
PtrList<volScalarField> pFluid(fluidRegions.size());
PtrList<volScalarField> ghFluid(fluidRegions.size());
PtrList<surfaceScalarField> ghfFluid(fluidRegions.size());
PtrList<fv::options> fluidFvOptions(fluidRegions.size());
PtrList<pimpleControl> pimpleFluid(fluidRegions.size());
List<int> QIterationFlagFluid(fluidRegions.size());
List<int> QcyclesFluid(fluidRegions.size());
List<scalar> QrelaxFluid(fluidRegions.size());
List<int> dVFluid(fluidRegions.size());
List<scalar> QResidualFluid(fluidRegions.size());

List<int> SourceTypeFluid(fluidRegions.size());
List<scalar> SourceConstantFluid(fluidRegions.size());


PtrList<volScalarField> alphatFluid(fluidRegions.size());

// Populate fluid field pointer lists
forAll(fluidRegions, i)
{
  Info<< "*** Reading fluid mesh thermophysical properties for region " << fluidRegions[i].name() << nl << endl;

  thermoFluid.set
  (
    i,
    //twoPhaseVOFThermo::New(fluidRegions[i]).ptr()
    rhoThermo::New(fluidRegions[i]).ptr()
  );

  Info<< "Adding to transportProperties\n" << endl;
  transportPropertiesFluid.set
  (
    i,
    new IOdictionary 
    (
      IOobject
      (
        "transportProperties",
        runTime.constant(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      )
    )
  );

  dhvFluid.set
  (
    i,
    new dimensionedScalar
	(
      transportPropertiesFluid[i].lookup("dhv")
	)
  );
  TsatFluid.set
  (
    i,
    new dimensionedScalar
    (
      transportPropertiesFluid[i].lookup("Tsat")
    )
  );

  Info<< "Adding to p_rghFluid\n" << endl;
  p_rghFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "p_rgh",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      ),
      fluidRegions[i]
    )
  );
  Info<< "Adding to UFluid\n" << endl;
  UFluid.set
  (
    i,
    new volVectorField
    (
      IOobject
      (
        "U",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      ),
      fluidRegions[i]
    )
  );

  Info<< "Adding to phiFluid\n" << endl;
  phiFluid.set
  (
    i,
    new surfaceScalarField
    (
      IOobject
      (
        "phi",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
      ),
      linearInterpolate(UFluid[i])
      & fluidRegions[i].Sf()
    )
  );

  Info<< "Adding to mixtureFluid\n" << endl;
  mixtureFluid.set
  (
    i,
    phaseChangeTwoPhaseMixtureCond::New(UFluid[i], phiFluid[i]).ptr()
  );
  

  Info<< "Adding to alpha1Fluid\n" << endl;
  alpha1Fluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "alpha.kondensate",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      ),
      mixtureFluid[i].alpha1()
    )
  );
  
  
  alpha1PhiFluid.set
  (
    i,
    new surfaceScalarField
    (
      IOobject
      (
        "alphaPhi.kondensate",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      ),
      linearInterpolate(UFluid[i]*alpha1Fluid[i])
      & fluidRegions[i].Sf()
    )
  );
  
  
  Info<< "Adding to alpha2Fluid\n" << endl;
  alpha2Fluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "alpha.vapor",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      1 - alpha1Fluid[i]
    )
  );

  Info<< "Adding to rhoFluid\n" << endl;
  rho1Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].rho1()
    )
  );
  rho2Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].rho2()
    )
  );

  rhoFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "rho",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      alpha1Fluid[i]*rho1Fluid[i]+alpha2Fluid[i]*rho2Fluid[i]
    )
  );
  rhoFluid[i].oldTime();

        Info<< "    Adding to rhoPhiFluid\n" << endl;
        rhoPhiFluid.set
        (
            i,
            new surfaceScalarField
            (
                IOobject
                (
                    "rhoPhi",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                linearInterpolate(rhoFluid[i])*linearInterpolate(UFluid[i])
                    & fluidRegions[i].Sf()
            )
        );


  Info<< "Adding to DTFluid\n" << endl;
  DT1Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].DT1()
    )
  );
  DT2Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].DT2()
    )
  );

  DTFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "DT",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      alpha1Fluid[i]*DT1Fluid[i]+alpha2Fluid[i]*DT2Fluid[i]
    )
  );

  Info<< "Adding to CpFluid\n" << endl;
  Cp1Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].Cp1()
    )
  );
  Cp2Fluid.set
  (
    i,
    new dimensionedScalar
    (
      mixtureFluid[i].Cp2()
    )
  );

  CpFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "Cp",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      alpha1Fluid[i]*Cp1Fluid[i]+alpha2Fluid[i]*Cp2Fluid[i]
    )
  );

  Info<< "Adding to turbulence\n" << endl;

  turbulenceFluid.set
  (
    i,
    //incompressible::turbulenceModel::New(UFluid[i], phiFluid[i], mixtureFluid[i]).ptr()

    incompressible::turbulenceModel::New
    (
      UFluid[i],
      phiFluid[i],
      mixtureFluid[i]
    ).ptr()

  );
  
  
/*
  turbulenceFluid_test.set
  (
    i,
    PhaseIncompressibleTurbulenceModel<phaseChangeTwoPhaseMixtureCond>::New
    (
      alpha1Fluid[i],
      UFluid[i],      
      rhoPhiFluid[i],
      phiFluid[i],
      mixtureFluid[i]
    ).ptr()

  );
*/
  
  

  Info<< "Reading field alphat\n" << endl;
  alphatFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "alphat",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      ),
      fluidRegions[i]
    )
  );
  
  Info<< "Adding to gFluid\n" << endl;
  gFluid.set
  (
    i,
    new uniformDimensionedVectorField
    (
      IOobject
      (
        "g",
        runTime.constant(),
        fluidRegions[i],
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      )
    )
  );

  Info<< "Adding to ghFluid\n" << endl;
  ghFluid.set
  (
    i,
    new volScalarField("gh", gFluid[i] & fluidRegions[i].C())
  );

  Info<< "Adding to ghfFluid\n" << endl;
  ghfFluid.set
  (
    i,
    new surfaceScalarField("ghf", gFluid[i] & fluidRegions[i].Cf())
  );

  Info<< "Adding fvOptions\n" << endl;
  fluidFvOptions.set 
  (
    i,
    new fv::options(fluidRegions[i])
  );

  pimpleFluid.set
  (
    i,
    new pimpleControl
    (
      fluidRegions[i]
    )
  );

  Info<< "Adding to dhe\n" << endl;
  dheFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "dhe",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      thermoFluid[i].T() * CpFluid[i] *rhoFluid[i]/ runTime.deltaT()*0
    )
  );
  
  q_pcFluid.set
  (
    i,
    new volScalarField
    (
      IOobject
      (
        "q_pc",
        runTime.timeName(),
        fluidRegions[i],
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
      ),
      thermoFluid[i].T() * rhoFluid[i] * CpFluid[i] / runTime.deltaT()*0
    )
  );

  const dictionary& pimpleDict = fluidRegions[i].solutionDict().subDict("PIMPLE");
  pimpleDict.readIfPresent("QIterationFlag", QIterationFlagFluid[i]);
  pimpleDict.readIfPresent("Qcycles", QcyclesFluid[i]);
  pimpleDict.readIfPresent("Qrelax", QrelaxFluid[i]);
  pimpleDict.readIfPresent("dV", dVFluid[i],1);
  pimpleDict.readIfPresent("QResidual", QResidualFluid[i]);
  pimpleDict.readIfPresent("SourceType", SourceTypeFluid[i]);
  pimpleDict.readIfPresent("SourceConstant", SourceConstantFluid[i]);
}
