/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

#include "incompressibleTwoPhaseMixtureCond.H"
#include "addToRunTimeSelectionTable.H"
#include "surfaceFields.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(incompressibleTwoPhaseMixtureCond, 0);
}

const Foam::word Foam::incompressibleTwoPhaseMixtureCond::dictName("twoPhaseMixtureTransportProperties");

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

//- Calculate and return the laminar viscosity
void Foam::incompressibleTwoPhaseMixtureCond::calcNu()
{
    nuModel1_->correct();
    nuModel2_->correct();

    const volScalarField limitedAlpha1
    (
        "limitedAlpha1",
        min(max(alpha1_, scalar(0)), scalar(1))
    );

    // Average kinematic viscosity calculated from dynamic viscosity
    nu_ = mu()/(limitedAlpha1*rho1_ + (scalar(1) - limitedAlpha1)*rho2_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::incompressibleTwoPhaseMixtureCond::incompressibleTwoPhaseMixtureCond
(
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    IOdictionary
    (
        IOobject
        (
            "transportProperties",
            U.time().constant(),
            U.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    twoPhaseMixture(U.mesh(), *this),

    nuModel1_(viscosityModel::New("nu1", subDict(phase1Name_), U, phi)),
    nuModel2_(viscosityModel::New("nu2", subDict(phase2Name_), U, phi)),

    rho1_("rho", dimDensity, nuModel1_->viscosityProperties().lookup("rho")),
    rho2_("rho", dimDensity, nuModel2_->viscosityProperties().lookup("rho")),

    DT1_("DT", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), nuModel1_->viscosityProperties().lookup("DT")),
    DT2_("DT", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), nuModel2_->viscosityProperties().lookup("DT")),

    Cp1_("Cp", dimLength*dimLength/(dimTime*dimTime*dimTemperature), nuModel1_->viscosityProperties().lookup("Cp")),
    Cp2_("Cp", dimLength*dimLength/(dimTime*dimTime*dimTemperature), nuModel2_->viscosityProperties().lookup("Cp")),

    U_(U),
    phi_(phi),

    nu_
    (
        IOobject
        (
            "nu",
            U_.time().timeName(),
            U_.db()
        ),
        U_.mesh(),
        dimensionedScalar("nu", dimensionSet(0, 2, -1, 0, 0), 0),
        calculatedFvPatchScalarField::typeName
    )
{
    calcNu();
}



Foam::incompressibleTwoPhaseMixtureCond::incompressibleTwoPhaseMixtureCond
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    const dictionary& dict
)
:
    IOdictionary
    (
        IOobject
        (
            "transportProperties",
            U.time().constant(),
            U.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        ),
        dict
    ),
    twoPhaseMixture(U.mesh(), *this),

    nuModel1_(viscosityModel::New("nu1", subDict(phase1Name_), U, phi)),
    nuModel2_(viscosityModel::New("nu2", subDict(phase2Name_), U, phi)),

    rho1_("rho", dimDensity, nuModel1_->viscosityProperties().lookup("rho")),
    rho2_("rho", dimDensity, nuModel2_->viscosityProperties().lookup("rho")),

    DT1_("DT", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), nuModel1_->viscosityProperties().lookup("DT")),
    DT2_("DT", dimMass*dimLength/(dimTime*dimTime*dimTime*dimTemperature), nuModel2_->viscosityProperties().lookup("DT")),

    Cp1_("Cp", dimLength*dimLength/(dimTime*dimTime*dimTemperature), nuModel1_->viscosityProperties().lookup("Cp")),
    Cp2_("Cp", dimLength*dimLength/(dimTime*dimTime*dimTemperature), nuModel2_->viscosityProperties().lookup("Cp")),

    U_(U),
    phi_(phi),

    nu_
    (
        IOobject
        (
            "nu",
            U_.time().timeName(),
            U_.db()
        ),
        U_.mesh(),
        dimensionedScalar("nu", dimensionSet(0, 2, -1, 0, 0), 0),
        calculatedFvPatchScalarField::typeName
    )
{
    calcNu();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::incompressibleTwoPhaseMixtureCond::mu() const
{
    const volScalarField limitedAlpha1
    (
        min(max(alpha1_, scalar(0)), scalar(1))
    );

    return tmp<volScalarField>
    (
        new volScalarField
        (
            "mu",
            limitedAlpha1*rho1_*nuModel1_->nu()
          + (scalar(1) - limitedAlpha1)*rho2_*nuModel2_->nu()
        )
    );
}


Foam::tmp<Foam::surfaceScalarField>
Foam::incompressibleTwoPhaseMixtureCond::muf() const
{
    const surfaceScalarField alpha1f
    (
        min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
    );

    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            "muf",
            alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
          + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
        )
    );
}


Foam::tmp<Foam::surfaceScalarField>
Foam::incompressibleTwoPhaseMixtureCond::nuf() const
{
    const surfaceScalarField alpha1f
    (
        min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
    );

    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            "nuf",
            (
                alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
              + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
            )/(alpha1f*rho1_ + (scalar(1) - alpha1f)*rho2_)
        )
    );
}


bool Foam::incompressibleTwoPhaseMixtureCond::read()
{
    if (regIOobject::read())
    {
        if(
            nuModel1_().read(subDict(phase1Name_ == "1" ? "phase1": phase1Name_))
         && nuModel2_().read(subDict(phase2Name_ == "2" ? "phase2": phase2Name_)))
        {
            nuModel1_->viscosityProperties().lookup("rho") >> rho1_;
            nuModel2_->viscosityProperties().lookup("rho") >> rho2_;

            nuModel1_->viscosityProperties().lookup("DT") >> DT1_;
            nuModel2_->viscosityProperties().lookup("DT") >> DT2_;

            nuModel1_->viscosityProperties().lookup("Cp") >> Cp1_;
            nuModel2_->viscosityProperties().lookup("Cp") >> Cp2_;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
