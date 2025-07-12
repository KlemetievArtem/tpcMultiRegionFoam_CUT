/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2016-2018 OpenFOAM Foundation
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

#include "myWallHeatFlux.H"
#include "turbulentFluidThermoModel.H"
#include "turbulenceModel.H"
#include "turbulentTransportModel.H"

#include "solidThermo.H"
#include "rhoThermo.H"
#include "phaseChangeTwoPhaseMixtureCond.H"
#include "surfaceInterpolate.H"
#include "fvcSnGrad.H"
#include "wallPolyPatch.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(myWallHeatFlux, 0);
    addToRunTimeSelectionTable(functionObject, myWallHeatFlux, dictionary);
}
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void Foam::functionObjects::myWallHeatFlux::writeFileHeader(const label i)
{
    // Add headers to output data
    writeHeader(file(), "Wall heat-flux");
    writeCommented(file(), "Time");
    writeTabbed(file(), "patch");
    writeTabbed(file(), "min");
    writeTabbed(file(), "max");
    writeTabbed(file(), "integral");
    writeTabbed(file(), "average_q");
    writeTabbed(file(), "average_dT");
    writeTabbed(file(), "average_alpha");
    writeTabbed(file(), "average_ave_alpha");
    file() << endl;
}


Foam::tmp<Foam::volScalarField>
Foam::functionObjects::myWallHeatFlux::calcMyWallHeatFlux
(
    const volScalarField& alpha,
    const volScalarField& he
)
{
    tmp<volScalarField> twallHeatFlux
    (
        new volScalarField
        (
            IOobject
            (
                type(),
                mesh_.time().timeName(),
                mesh_
            ),
            mesh_,
            dimensionedScalar("0", dimMass/pow3(dimTime), 0)
        )
    );


    volScalarField::Boundary& wallHeatFluxBf =
        twallHeatFlux.ref().boundaryFieldRef();

    const volScalarField::Boundary& heBf =
        he.boundaryField();

    const volScalarField::Boundary& alphaBf =
        alpha.boundaryField();

    forAll(wallHeatFluxBf, patchi)
    {
        if (!wallHeatFluxBf[patchi].coupled())
        {
            wallHeatFluxBf[patchi] = alphaBf[patchi]*heBf[patchi].snGrad();
        }
    }

    if (foundObject<volScalarField>("qr"))
    {
        const volScalarField& qr = lookupObject<volScalarField>("qr");

        const volScalarField::Boundary& radHeatFluxBf =
            qr.boundaryField();

        forAll(wallHeatFluxBf, patchi)
        {
            if (!wallHeatFluxBf[patchi].coupled())
            {
                wallHeatFluxBf[patchi] -= radHeatFluxBf[patchi];
            }
        }
    }

    return twallHeatFlux;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::myWallHeatFlux::myWallHeatFlux
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    logFiles(obr_, name),
    writeLocalObjects(obr_, log),
    patchSet_()
{
    read(dict);
    resetName(typeName);
    resetLocalObjectName(typeName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::myWallHeatFlux::~myWallHeatFlux()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::myWallHeatFlux::read(const dictionary& dict)
{
    fvMeshFunctionObject::read(dict);
    writeLocalObjects::read(dict);
    
    
    UName_ = dict.lookupOrDefault<word>("U", "U");
    phiName_ = dict.lookupOrDefault<word>("phi", "phi");
    alphatName_ = dict.lookupOrDefault<word>("alphat", "alphat");

    const polyBoundaryMesh& pbm = mesh_.boundaryMesh();

    patchSet_ =
        mesh_.boundaryMesh().patchSet
        (
            wordReList(dict.lookupOrDefault("patches", wordReList()))
        );

    Info<< type() << " " << name() << ":" << nl;

    if (patchSet_.empty())
    {
        forAll(pbm, patchi)
        {
            if (isA<wallPolyPatch>(pbm[patchi]))
            {
                patchSet_.insert(patchi);
            }
        }

        Info<< "    processing all wall patches" << nl << endl;
    }
    else
    {
        Info<< "    processing wall patches: " << nl;
        labelHashSet filteredPatchSet;
        forAllConstIter(labelHashSet, patchSet_, iter)
        {
            label patchi = iter.key();
            if (isA<wallPolyPatch>(pbm[patchi]))
            {
                filteredPatchSet.insert(patchi);
                Info<< "        " << pbm[patchi].name() << endl;
            }
            else
            {
                WarningInFunction
                    << "Requested wall heat-flux on non-wall boundary "
                    << "type patch: " << pbm[patchi].name() << endl;
            }
        }

        Info<< endl;

        patchSet_ = filteredPatchSet;
    }
    
    dict.lookup("refT") >> refT_;

    return true;
}


bool Foam::functionObjects::myWallHeatFlux::execute()
{
    word name(type());

    if
    (
        foundObject<incompressible::turbulenceModel>
        (
            turbulenceModel::propertiesName
        )
    )
    {
    
        const rhoThermo& thermo =
            lookupObject<rhoThermo>(rhoThermo::dictName);
        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);
        const surfaceScalarField& phi = obr_.lookupObject<surfaceScalarField>(phiName_);
        const volScalarField& alphat = obr_.lookupObject<volScalarField>(alphatName_);
        
        //phaseChangeTwoPhaseMixtureCond mixtureWallHeat(U, phi);
        
	autoPtr<phaseChangeTwoPhaseMixtureCond> mixtureWallHeat(phaseChangeTwoPhaseMixtureCond::New(U, phi));
	
        volScalarField tmpDT
            (
                IOobject
                (
                    "tmpDT",
                    mesh_.time().timeName(),
                    mesh_
                ),
	      mixtureWallHeat->alpha1()*mixtureWallHeat->DT1()+mixtureWallHeat->alpha2()*mixtureWallHeat->DT2() + alphat*(mixtureWallHeat->alpha1()*mixtureWallHeat->Cp1()*mixtureWallHeat->rho1()+mixtureWallHeat->alpha2()*mixtureWallHeat->Cp2()*mixtureWallHeat->rho2())
            );
        
        return store
        (
            name,
            calcMyWallHeatFlux(tmpDT, thermo.T())
        );
    }
    else if
    (
        foundObject<compressible::turbulenceModel>
        (
            turbulenceModel::propertiesName
        )
    )
    {
        const compressible::turbulenceModel& turbModel =
            lookupObject<compressible::turbulenceModel>
            (
                turbulenceModel::propertiesName
            );

        return store
        (
            name,
            calcMyWallHeatFlux(turbModel.alphaEff(), turbModel.transport().he())
        );
    }
    else if (foundObject<solidThermo>(solidThermo::dictName))
    {
        const solidThermo& thermo =
            lookupObject<solidThermo>(solidThermo::dictName);

        return store(name, calcMyWallHeatFlux(thermo.alpha(), thermo.he()));
    }
    else
    {
        FatalErrorInFunction
            << "Unable to find incompressible turbulence model in the "
            << "database" << exit(FatalError);
    }

    return true;
}


bool Foam::functionObjects::myWallHeatFlux::write()
{
    Log << type() << " " << name() << " write:" << nl;

    writeLocalObjects::write();

    logFiles::write();
    
    //
    const rhoThermo& thermo =
        lookupObject<rhoThermo>(rhoThermo::dictName);

    const volScalarField& myTemp = thermo.T();
    //
    

    const volScalarField& myWallHeatFlux =
        obr_.lookupObject<volScalarField>(type());

    const fvPatchList& patches = mesh_.boundary();

    const surfaceScalarField::Boundary& magSf =
        mesh_.magSf().boundaryField();

    forAllConstIter(labelHashSet, patchSet_, iter)
    {
        label patchi = iter.key();
        const fvPatch& pp = patches[patchi];

        const scalarField& hfp = myWallHeatFlux.boundaryField()[patchi];
        //
        const scalarField& Twallfp = myTemp.boundaryField()[patchi];
        //
        const scalar minHfp = gMin(hfp);
        const scalar maxHfp = gMax(hfp);
        const scalar integralHfp = gSum(magSf[patchi]*hfp);
        const scalar aveHfp = integralHfp/gSum(magSf[patchi]);
        
        const scalar avedTfp = refT_-(gSum(magSf[patchi]*Twallfp)/gSum(magSf[patchi]));

        const scalar aveAlphafp = aveHfp/avedTfp;


        const scalar aveAveAlphafp = gSum(hfp/(refT_-Twallfp)*magSf[patchi])/gSum(magSf[patchi]);
        
        if (Pstream::master())
        {
            file()
                << mesh_.time().value()
                << tab << pp.name()
                << tab << minHfp
                << tab << maxHfp
                << tab << integralHfp
                << tab << aveHfp
                << tab << avedTfp
                << tab << aveAlphafp
                << tab << aveAveAlphafp
                << endl;
        }

        Log << "    min/max/integ/ave_q/ave_dT/ave_alpha/ave_ave_alpha(" << pp.name() << ") = "
            << minHfp << ", " << maxHfp << ", " << integralHfp<< ", " << aveHfp<< ", " << avedTfp<< ", " << aveAlphafp<< ", " << aveAveAlphafp << endl;
    }

    Log << endl;

    return true;
}


// ************************************************************************* //
