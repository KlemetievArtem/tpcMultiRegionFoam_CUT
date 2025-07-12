//Determination of minimum Cell length in simulation domain is calculated according
//to interThermalPhaseChangeFoam developed by Rattner and MahdiNabil

//PtrList<volScalarField> minCellLengthFluid(fluidRegions.size());
PtrList<volScalarField> minCellFaceFluid(fluidRegions.size());
List<scalar> dXCVFluid(fluidRegions.size());

forAll(fluidRegions, regionI)
{
/*
	minCellLengthFluid.set
	(
		regionI,
		new volScalarField
        (
            IOobject
            (
                "minCellLengthFluid",
                    runTime.timeName(),
                    fluidRegions[regionI],
                    IOobject::NO_READ, //IOobject::NO_READ,IOobject::READ_IF_PRESENT
                    IOobject::AUTO_WRITE //IOobject::NO_WRITE
                ),
				fluidRegions[regionI],
				dimensionedScalar("dummy", dimensionSet(0,1,0,0,0,0,0), 1e-5)
            )
        );
*/
	minCellFaceFluid.set
	(
		regionI,
		new volScalarField
        (
            IOobject
            (
                "minCellFaceFluid",
                    runTime.timeName(),
                    fluidRegions[regionI],
                    IOobject::NO_READ, //IOobject::NO_READ,IOobject::READ_IF_PRESENT,IOobject::MUST_READ_IF_MODIFIED, 
                    IOobject::AUTO_WRITE //IOobject::NO_WRITE
                ),
				fluidRegions[regionI],
				dimensionedScalar("dummy", dimensionSet(0,2,0,0,0,0,0), 1e-10)
            )
        );

	Info<< "Minimum cell length: " <<  gMin(minCellLengthFluid[regionI]) << endl;
	dXCVFluid[regionI] = gMin(minCellLengthFluid[regionI]);
	
	//volScalarField& minCellFace = minCellFaceFluid[regionI];
	//fvMesh& mesh = fluidRegions[regionI];
	//minCellFace = gMin(mesh.cellVolumes())* dimensionedScalar("s", dimensionSet(0,2,0,0,0,0,0), 1e5);
	
	
}

