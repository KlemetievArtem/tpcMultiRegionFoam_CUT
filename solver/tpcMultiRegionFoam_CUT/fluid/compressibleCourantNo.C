/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/


#include "compressibleCourantNo.H"
#include "fvc.H"
#include "scalarField.H"
#include "surfaceFields.H"
#include "fvcSurfaceIntegrate.H"
#include "zeroGradientFvPatchFields.H"
#include "addToRunTimeSelectionTable.H"
#include "IOobject.H"



Foam::scalar Foam::compressibleCourantNo
(
    const fvMesh& mesh,
    const Time& runTime,
    const volScalarField& rho,
    const surfaceScalarField& phi
)
{
    tmp<volScalarField> CoField(new volScalarField(IOobject("CoField",mesh.time().timeName(),mesh,IOobject::NO_READ,IOobject::AUTO_WRITE),
                mesh,
                dimensionedScalar("0", dimless, 0.0),
                zeroGradientFvPatchScalarField::typeName)
                );
	
    //scalarField sumPhi(fvc::surfaceSum(mag(phi))().primitiveField()/ rho.primitiveField());
    scalarField sumPhi(fvc::surfaceSum(mag(phi))().primitiveField());
    
    CoField->ref() = (0.5*mesh.time().deltaT())*fvc::surfaceSum(mag(phi))()()/mesh.V();
    //CoField->ref() = (0.5*mesh.time().deltaT())*sumPhi/mesh.V();
    //CoField->correctBoundaryConditions();
    
    
    //CoField.internalField() = 0.5*(CoField/mesh.V().field())*runTime.deltaTValue();

    scalar CoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();

    scalar meanCoNum =
        0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();

    Info<< "Region: " << mesh.name() << " Courant Number mean: " << meanCoNum
        << " max: " << CoNum << endl;

    if(mesh.time().outputTime()){
         CoField->write();
    }
    //store("CoField", CoField);
    return CoNum;
}


// ************************************************************************* //
