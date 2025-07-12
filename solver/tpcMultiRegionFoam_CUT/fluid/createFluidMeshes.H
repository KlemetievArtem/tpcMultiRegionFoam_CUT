    const wordList fluidNames(rp["fluid"]);

    PtrList<fvMesh> fluidRegions(fluidNames.size());

    PtrList<volScalarField> minCellLengthFluid(fluidRegions.size());
    forAll(fluidNames, i)
    {
        Info<< "Create fluid mesh for region " << fluidNames[i]
            << " for time = " << runTime.timeName() << nl << endl;

        fluidRegions.set
        (
            i,
            new fvMesh
            (
                IOobject
                (
                    fluidNames[i],
                    runTime.timeName(),
                    runTime,
                    IOobject::MUST_READ
                )
            )
        );
	minCellLengthFluid.set
	(
		i,
		new volScalarField
        (
            IOobject
            (
                "minCellLengthFluid",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ, //IOobject::NO_READ,IOobject::READ_IF_PRESENT,IOobject::MUST_READ_IF_MODIFIED, 
                    IOobject::AUTO_WRITE //IOobject::NO_WRITE
                ),
				fluidRegions[i],
				dimensionedScalar("dummy", dimensionSet(0,1,0,0,0,0,0), SMALL)
            )
        );
	const cellList& cells = fluidRegions[i].cells();
		
	//volVectorField centres = fluidRegions[i].C();
	const pointField& ctrs = fluidRegions[i].C();
	forAll(cells, cellI)
	{
		scalar minLength = GREAT;
		const labelList neighbours = fluidRegions[i].cellCells()[cellI];		
		const point& main_p = ctrs[cellI];		
		forAll(neighbours, nbI)
		{
			const point& nb_p = ctrs[neighbours[nbI]];
			scalar length = powf((pow(main_p.component(vector::X)-nb_p.component(vector::X),2)+pow(main_p.component(vector::Y)-nb_p.component(vector::Y),2)+pow(main_p.component(vector::Z)-nb_p.component(vector::Z),2)),0.5);
			if (length < minLength)
			{  
				minLength = length;
			}			
		}
		minCellLengthFluid[i][cellI] = minLength;
	}
    }
