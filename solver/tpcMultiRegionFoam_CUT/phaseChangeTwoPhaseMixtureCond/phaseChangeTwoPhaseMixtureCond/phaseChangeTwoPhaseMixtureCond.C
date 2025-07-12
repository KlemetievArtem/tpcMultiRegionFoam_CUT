/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "phaseChangeTwoPhaseMixtureCond.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(phaseChangeTwoPhaseMixtureCond, 0);
    defineRunTimeSelectionTable(phaseChangeTwoPhaseMixtureCond, components);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::phaseChangeTwoPhaseMixtureCond::phaseChangeTwoPhaseMixtureCond
(
    const word& type,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    immiscibleIncompressibleTwoPhaseMixtureCond(U, phi),
    phaseChangeTwoPhaseMixtureCondCoeffs_(subDict(type + "Coeffs"))//,

{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::Pair<Foam::tmp<Foam::volScalarField> >
Foam::phaseChangeTwoPhaseMixtureCond::vDotAlphal() const
{
    dimensionedScalar alphaCoeff(1.0/rho1());
	Pair<tmp<volScalarField> > mDotAlphal = this->mDotAlphal();
    return Pair<tmp<volScalarField> >(alphaCoeff*mDotAlphal[0],alphaCoeff*mDotAlphal[1]);
}

Foam::Pair<Foam::tmp<Foam::volScalarField> >
Foam::phaseChangeTwoPhaseMixtureCond::vDotP() const
{
    dimensionedScalar pCoeff(1.0/rho1() - 1.0/rho2());
    Pair<tmp<volScalarField> > mDotP = this->mDotP();
    
    return Pair<tmp<volScalarField> >(pCoeff*mDotP[0], pCoeff*mDotP[1]);
}


bool Foam::phaseChangeTwoPhaseMixtureCond::read()
{
    if (incompressibleTwoPhaseMixtureCond::read())
    {
        phaseChangeTwoPhaseMixtureCondCoeffs_ = subDict(type() + "Coeffs");

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
