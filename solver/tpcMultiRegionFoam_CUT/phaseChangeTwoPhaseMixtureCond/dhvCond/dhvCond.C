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

#include "dhvCond.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace phaseChangeTwoPhaseMixtures
{
    defineTypeNameAndDebug(dhvCond, 0);
    addToRunTimeSelectionTable(phaseChangeTwoPhaseMixtureCond, dhvCond, components);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::phaseChangeTwoPhaseMixtures::dhvCond::dhvCond
(
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    phaseChangeTwoPhaseMixtureCond(typeName, U, phi),

    dhv_(phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("dhv")),
    Cc_(phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("Cc")),
    Cv_(phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("Cv")),
    he0_("1", dimensionSet(1,-1,-3,0,0,0,0), 1.0e-13),


    mcCoeff_(Cc_/(dhv_)),
    mvCoeff_(Cv_/(dhv_))

{
    correct();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::Pair<Foam::tmp<Foam::volScalarField> >
Foam::phaseChangeTwoPhaseMixtures::dhvCond::mDotAlphal() const
{
	const volScalarField& dhe = incompressibleTwoPhaseMixtureCond::alpha1_.db().lookupObject<volScalarField>("dhe");


    return Pair<tmp<volScalarField> >
    (
        mcCoeff_*(-min(dhe, he0_))*pos(1-incompressibleTwoPhaseMixtureCond::alpha1_),
        mvCoeff_*(max(dhe, he0_))*pos(incompressibleTwoPhaseMixtureCond::alpha1_)
    );
}

Foam::Pair<Foam::tmp<Foam::volScalarField> >
Foam::phaseChangeTwoPhaseMixtures::dhvCond::mDotP() const
{
    const volScalarField& dhe = incompressibleTwoPhaseMixtureCond::alpha1_.db().lookupObject<volScalarField>("dhe");

    return Pair<tmp<volScalarField> >
    (
        mcCoeff_*neg(dhe)*dhe,
        mvCoeff_*pos(dhe)*dhe
    );

}


void Foam::phaseChangeTwoPhaseMixtures::dhvCond::correct()
{}


bool Foam::phaseChangeTwoPhaseMixtures::dhvCond::read()
{
    if (phaseChangeTwoPhaseMixtureCond::read())
    {
        phaseChangeTwoPhaseMixtureCondCoeffs_ = subDict(type() + "Coeffs");

        phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("dhv") >> dhv_;
        phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("Cc") >> Cc_;
        phaseChangeTwoPhaseMixtureCondCoeffs_.lookup("Cv") >> Cv_;



        mcCoeff_ = Cc_ /(dhv_);
		mvCoeff_ = Cv_ /(dhv_);
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
